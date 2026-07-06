#include <stdio.h>
#include <math.h>

#ifdef _OPENMP
#include <omp.h>
#endif // _OPENMP

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"

static const int RUN_COUNT = 20;

static const double DEG2RAD = 3.1419 / 180.0;
static const double PEAK_THRESHOLD_RATIO = 0.8;

void hough_transform( int cores, char* img_path, int t_mult, int print_times, int print_lines ) {

  int w, h;
  unsigned char* img = stbi_load( img_path, &w, &h, NULL, 1 );
  if ( img == NULL ) {
    printf( "Failed to load image: %s\n", img_path );
    exit( 2 );
  }


  int center_x = w / 2;
  int center_y = h / 2;

  // Maximum distance in image from origo (origo in middle of image)
  double max_distance = sqrt( 2.0 ) * (h > w ? h : w) / 2.0;
  // rho range
  int acc_h = max_distance * 2.0;
  // theta range
  int acc_w = 180 * t_mult;

  // accumulator
  unsigned short int* acc = calloc( acc_h * acc_w, sizeof( unsigned short int ) );


double _time_vote_start = omp_get_wtime();


  // Voting phase
# pragma omp parallel for num_threads( cores ) collapse( 2 ) \
    default( none ) \
    shared( h, w, img, acc_w, t_mult, center_x, center_y, max_distance, acc )
  for ( int y = 0; y < h; ++y ) {
    for ( int x = 0; x < w; ++x ) {

      if ( img[ y * w + x ] > 250 ) {

        for ( int t = 0; t < acc_w; ++t ) { // calculate rho for each theta
          double t_deg = t / (double)t_mult;
          double r = (x - center_x) * cos( t_deg * DEG2RAD ) + (y - center_y) * sin( t_deg * DEG2RAD );

          int index = ((int)(r + max_distance)) * acc_w + t;
# pragma omp atomic
          acc[ index ] += 1; // increase the accumulator for (rho, theta)
        }
      }
    }
  }


double _time_vote_end = omp_get_wtime();


  // Find maximum accumulator value
  int max_acc = 0;
#pragma omp parallel for num_threads( cores ) collapse( 2 ) reduction( max: max_acc ) \
    default( none ) \
    shared( acc_h, acc_w, acc )
  for ( int r = 0; r < acc_h; ++r ) {
      for ( int t = 0; t < acc_w; ++t ) {
          if ( acc[ r * acc_w + t ] > max_acc )
              max_acc = acc[ r * acc_w + t ];
      }
  }


  int threshold = max_acc * PEAK_THRESHOLD_RATIO;

  int max_count = 50;
  int* rs = malloc( max_count * sizeof( int ) );
  double* ts = malloc( max_count * sizeof( double ) );
  int count = 0;


double _time_peaks_start = omp_get_wtime();


  // Select local maxima
#pragma omp parallel for num_threads( cores ) collapse( 2 ) \
    default( none ) \
    shared( acc_h, acc_w, acc, threshold, count, max_count, rs, ts, max_distance, t_mult )
  for ( int r = 0; r < acc_h; ++r ) {
    for ( int t = 0; t < acc_w; ++t ) {

      if ( acc[ r * acc_w + t ] >= threshold ) {

        // Check if local maximum (9x9)
        int max = acc[ r * acc_w + t ];
        for ( int dr = -4; dr <= 4; ++dr ) {
          for ( int dt = -4; dt <= 4; ++dt ) {

            if ( r + dr >= 0 && r + dr < acc_h && t + dt >= 0 && t + dt < acc_w ) {
              int current = acc[ (r + dr) * acc_w + t + dt ];
              if ( current > max ) {
                max = current;
                dr = 5;
                dt = 5;
              }
            }
          }
        }

        if ( max > acc[ r * acc_w + t ] )
          continue; // was not local maximum

// we only detect a few lines, so this is not a performance bottleneck
#pragma omp critical
{
        // only save maximum if we still have space
        if ( count < max_count ) {
          rs[ count ] = r - max_distance;
          ts[ count ] = t / (double)t_mult;
          ++count;
        }
}
      }
    }
  }


double _time_peaks_end = omp_get_wtime();


  if ( print_lines ) {
    printf( "%i lines:\n", count );
    for ( int i = 0; i < count; ++i )
      printf( "rho = %5i, theta = %6.2f\n", rs[ i ], ts[ i ] );
  }

  if ( print_times )
      // run_start,run_end,vote_start,vote_end,peaks_start,peaks_end
    printf( "%.6f,%.6f,%.6f,%.6f,%.6f,%.6f\n",
        _time_vote_start, // == _time_run_start
        _time_peaks_end,  // == _time_run_end
        _time_vote_start,
        _time_vote_end,
        _time_peaks_start,
        _time_peaks_end
    );

  stbi_image_free( img );
  free( acc );
  free( rs );
  free( ts );

  return;
}

int main( int argc, char** argv ) {
  if ( argc < 4 ) {
    printf( "usage: %s <cores> <img_path> <theta_multiplier> [<print_times>] [<print_lines>]\n", argv[ 0 ] );
    return 1;
  }

  int cores = atoi( argv[ 1 ] );
  char* img_path = argv[ 2 ];
  int theta_multiplier = atoi( argv[ 3 ] );

  int print_times =                 argc > 4 && atoi( argv[ 4 ] );
  int print_lines = !print_times && argc > 5 && atoi( argv[ 5 ] );

  if ( print_times )
    printf( "run_start,run_end,vote_start,vote_end,peaks_start,peaks_end\n" );

  hough_transform( cores, img_path, theta_multiplier, print_times, print_lines );
  if ( print_times )
    for ( int i = 1; i < RUN_COUNT; ++i )
      hough_transform( cores, img_path, theta_multiplier, print_times, 0 );

  return 0;
}


