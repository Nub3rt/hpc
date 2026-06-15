#include <stdio.h>
#include <math.h>

#include <mpi.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"

static const double DEG2RAD = 3.1419 / 180.0;
static const double PEAK_THRESHOLD_RATIO = 0.8;

int main( int argc, char** argv ) {
  MPI_Init( NULL, NULL );

  int size, rank;
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );

  if ( argc < 3 ) {
    printf( "usage: %s <img_path> <theta_multiplier> [<print_time>] [<print_lines>]\n", argv[ 0 ] );
    MPI_Abort( MPI_COMM_WORLD, 1 );
  }

  int w, h;
  unsigned char* img = NULL;

  int t_mult;

  // Read inputs
  if ( rank == 0 ) {
    img = stbi_load( argv[ 1 ], &w, &h, NULL, 1 );
    t_mult = atoi( argv[ 2 ] );

    if ( img == NULL ) {
      printf( "Failed to load image: %s\n", argv[ 1 ] );
      MPI_Abort( MPI_COMM_WORLD, 2 );
    }
  }

  // Broadcast dimensions & theta multiplier
  MPI_Bcast( &w, 1, MPI_INT, 0, MPI_COMM_WORLD );
  MPI_Bcast( &h, 1, MPI_INT, 0, MPI_COMM_WORLD );
  MPI_Bcast( &t_mult, 1, MPI_INT, 0, MPI_COMM_WORLD );

  int* counts = malloc( size * sizeof( int ) );
  int* displs = malloc( size * sizeof( int ) );

  // Data for MPI_Scatterv, partitioning the image by rows
  for ( int i = 0; i < size; ++i ) {
    int start_row = i * h / size;
    int end_row = (i + 1) * h / size;

    displs[ i ] = start_row * w;
    counts[ i ] = (end_row - start_row) * w;
  }

  int local_y_start = displs[ rank ] / w;
  int local_rows = counts[ rank ] / w;

  int local_n = counts[ rank ];
  unsigned char* local_img = malloc( local_n * sizeof( unsigned char ) );

  // Scatter image, every process gets local_rows rows
  MPI_Scatterv(
      img,
      counts,
      displs,
      MPI_UNSIGNED_CHAR,
      local_img,
      local_n,
      MPI_UNSIGNED_CHAR,
      0,
      MPI_COMM_WORLD
  );


  int center_x = w / 2;
  int center_y = h / 2;

  // Maximum distance in image from origo (origo in middle of image)
  double max_distance = sqrt( 2.0 ) * (h > w ? h : w) / 2.0;
  // rho range
  int acc_h = max_distance * 2.0;
  // theta range
  int acc_w = 180 * t_mult;

  // accumulator
  int* acc = calloc( acc_h * acc_w, sizeof( int ) );


  // Voting phase
  for ( int local_y = 0; local_y < local_rows; ++local_y ) {
    for ( int x = 0; x < w; ++x ) {

      if ( local_img[ local_y * w + x ] > 250 ) {

        int y = local_y_start + local_y; // real y position needed for calculation
        for ( int t = 0; t < acc_w; ++t ) { // calculate rho for each theta
          double t_deg = t / (double)t_mult;
          double r = (x - center_x) * cos( t_deg * DEG2RAD ) + (y - center_y) * sin( t_deg * DEG2RAD );
          acc[ ((int)(r + max_distance)) * acc_w + t ] += 1; // increase the accumulator for (rho, theta)
        }
      }
    }
  }

  // Collect accumulators
  MPI_Allreduce(
      MPI_IN_PLACE,
      acc,
      acc_w * acc_h,
      MPI_INT,
      MPI_SUM,
      MPI_COMM_WORLD
  );


  // Find maximum accumulator value

  // here not distributed by rows, simply divide
  // continuous data between processes
  int start_r = rank * acc_h / size;
  int end_r = (rank + 1) * acc_h / size;

  int max_acc = 0;
  for ( int r = start_r; r < end_r; ++r ) {
      for ( int t = 0; t < acc_w; ++t ) {
          if ( acc[ r * acc_w + t ] > max_acc )
              max_acc = acc[ r * acc_w + t ];
      }
  }

  // Reduce maximum accumulator value, and broadcast back to all processes
  MPI_Allreduce(
      MPI_IN_PLACE,
      &max_acc,
      1,
      MPI_INT,
      MPI_MAX,
      MPI_COMM_WORLD
  );


  int threshold = max_acc * PEAK_THRESHOLD_RATIO;
  int max_count = 50;
  // per process. This means that we can have
  // max_count * size lines maximum, but because of the
  // global threshold this won't inflate image count

  // We will accumulate maxima into root's arrays, so it needs ample memory
  int root_mult = rank == 0 ? size : 1;
  int* rs = malloc( root_mult * max_count * sizeof( int ) );
  double* ts = malloc( root_mult * max_count * sizeof( double ) );
  int count = 0;

  // Select local maxima
  // we are reusing start_r and end_r from the global maximum search
  for ( int r = start_r; r < end_r; ++r ) {
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

        // only save maximum if we still have space
        if ( count < max_count ) {
          rs[ count ] = r - max_distance;
          ts[ count ] = t / (double)t_mult;
          ++count;
        }
      }
    }
  }

  // Send back the count of maxima we found to root

  // This is needed to know the number of maxima we found but also
  // by using Gather instead of Reduce, we will have the perfect
  // counts array for Gatherv

  // We are reusing counts and displs arrays from the image scatter
  MPI_Gather(
      &count,
      1,
      MPI_INT,
      counts,
      1,
      MPI_INT,
      0,
      MPI_COMM_WORLD
  );

  // We are reusing counts and displs arrays from the image scatter
  displs[ 0 ] = 0;
  for ( int i = 1; i < size; ++i )
    displs[ i ] = displs[ i - 1 ] + counts[ i - 1 ];

  // Gather rhos and thetas back to root
  MPI_Gatherv(
      rank == 0 ? MPI_IN_PLACE : rs,
      count,
      MPI_INT,
      rs,
      counts,
      displs,
      MPI_INT,
      0,
      MPI_COMM_WORLD
  );
  MPI_Gatherv(
      rank == 0 ? MPI_IN_PLACE : ts,
      count,
      MPI_DOUBLE,
      ts,
      counts,
      displs,
      MPI_DOUBLE,
      0,
      MPI_COMM_WORLD
  );


  if ( rank == 0 ) {
    for ( int i = 1; i < size; ++i )
      count += counts[ i ]; // total number of lines found

    if ( argc > 3 && atoi( argv[ 3 ] ) ) {
      // print time
    }

    if ( argc > 4 && atoi( argv[ 4 ] ) ) {
      printf( "%i lines:\n", count );
      for ( int i = 0; i < count; ++i )
        printf( "rho = %5i, theta = %6.2f\n", rs[ i ], ts[ i ] );
    }
  }


  if ( rank == 0 )
    stbi_image_free( img );

  free( counts );
  free( displs );
  free( local_img );
  free( acc );
  free( rs );
  free( ts );

  MPI_Finalize();

  return 0;
}
