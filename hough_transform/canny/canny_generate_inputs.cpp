#include <iostream>
#include <cstdio>

#include <opencv2/opencv.hpp>

#define IN_PATH "canny.png"
#define OUT_PATH "../image/canny%i.png"

int main( int argc, char** argv ) {
  if ( argc < 4 ) {
    std::cout << "usage: canny_generate_inputs.out <n_edges> <most_edges> <scaledown_count>\n";
    return( 1 );
  }

  cv::Mat_<uchar> img = cv::imread( IN_PATH, cv::IMREAD_GRAYSCALE );
  if ( img.empty() ) {
    std::cout << "Could not load image " << IN_PATH << std::endl;
    return( 2 );
  }

  if (!img.isContinuous())
    img = img.clone();

  int n_edges = atoi( argv[ 1 ] );
  int target = atoi( argv[ 2 ] );
  int images = atoi( argv[ 3 ] );

  if ( n_edges < target ) {
    std::cout << "n_edges must be more than or equal to most_edges\n";
    return 3;
  }

  // delete edges uniformly to have target amount of edges
  int steps = 0, deleted = 0, steps_to_take = n_edges / (n_edges - target);
  for ( auto& pixel : img ) {
    if ( pixel > 250 ) {
      if ( ++steps == steps_to_take ) {
        pixel = 0;
        ++deleted;
        steps = 0;
        if ( deleted == n_edges - target )
          break;
      }
    }
  }

  char name[ 32 ];
  sprintf( name, OUT_PATH, target );
  cv::imwrite( name, img );

  while ( --images != 0 ) {
    // half edges uniformly
    bool to_delete = false;
    for ( auto& pixel : img ) {
      if ( pixel > 250 ) {
        if ( to_delete ) {
          pixel = 0;
          to_delete = !to_delete;
        } else {
          to_delete = !to_delete;
        }
      }
    }

    target /= 2;
    sprintf( name, OUT_PATH, target );
    cv::imwrite( name, img );
  }

  return 0;
}
