#include <stdio.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

const double DEG2RAD = 3.1419 / 180.0;

int main( int argc, char** argv ) {
  if ( argc < 3 ) {
    printf( "usage: %s <img_path> <theta_multiplier> [<print>]\n", argv[ 0 ] );
    return 1;
  }

  int w, h;
  unsigned char* img = stbi_load( argv[ 1 ], &w, &h, NULL, 1 );
  if ( img == NULL ) {
    printf( "Failed to load image: %s\n", argv[ 1 ] );
    return 2;
  }

  int center_x = w / 2;
  int center_y = h / 2;

  // maximum distance in image from origo (origo in middle of image)
  double max_distance = sqrt( 2.0 ) * (h > w ? h : w) / 2.0;
  // rho range
  int acc_h = max_distance * 2.0;
  // theta range
  int t_mult = atoi( argv[ 2 ] );
  int acc_w = 180 * t_mult;

  // accumulator
  int* acc = calloc( acc_h * acc_w, sizeof( int ) );

  // Voting phase
  for ( int y = 0; y < h; ++y ) {
    for ( int x = 0; x < w; ++x ) {
      if ( img[ y * w + x ] > 250 ) {
        for ( int t = 0; t < acc_w; ++t ) {
          double t_deg = t / (double)t_mult;
          double r = (x - center_x) * cos( t_deg * DEG2RAD ) + (y - center_y) * sin( t_deg * DEG2RAD );
          acc[ ((int)(r + max_distance)) * acc_w + t ] += 1;
        }
      }
    }
  }


  // Find maximum accumulator value
  int max_acc = 0;
  for ( int r = 0; r < acc_h; ++r ) {
      for ( int t = 0; t < acc_w; ++t ) {
          if ( acc[ r * acc_w + t ] > max_acc )
              max_acc = acc[ r * acc_w + t ];
      }
  }

  // Select local maxima
  int threshold = max_acc * 0.8;

  int max_count = 50;
  int* rs = malloc( max_count * sizeof( int ) );
  double* ts = malloc( max_count * sizeof( double ) );
  int count = 0;


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
          continue;

        if ( count < max_count ) {
          rs[ count ] = r - max_distance;
          ts[ count ] = t / (double)t_mult;
          ++count;
        }
      }
    }
  }


  if ( argc > 3 && atoi( argv[ 3 ] ) ) {
    printf( "%i lines:\n", count );
    for ( int i = 0; i < count; ++i )
      printf( "rho = %5i, theta = %6.2f\n", rs[ i ], ts[ i ] );
  }

  stbi_image_free( img );
  free( acc );
  free( rs );
  free( ts );

  return 0;
}
