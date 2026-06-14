#include <stdlib.h>
#include <math.h>

#include <iostream>

#include <opencv2/opencv.hpp>

const int GAUSSIAN_KERNEL_SIZE = 5;
const double GAUSSIAN_SIGMA = 2.0;
const double CANNY_LOW_THRESHOLD = 150.0;
const double CANNY_HIGH_THRESHOLD = 200.0;

const double DEG2RAD = 3.1419 / 180.0;

int main( ) {
  cv::Mat img = cv::imread( "in.jpg", cv::IMREAD_COLOR_BGR );

  cv::Mat gray;
  cv::cvtColor( img, gray, cv::COLOR_BGR2GRAY );

  cv::Mat edges( gray );
  cv::GaussianBlur(
      edges,
      edges,
      cv::Size( GAUSSIAN_KERNEL_SIZE, GAUSSIAN_KERNEL_SIZE ),
      GAUSSIAN_SIGMA
  );
  cv::Canny( edges, edges, CANNY_LOW_THRESHOLD, CANNY_HIGH_THRESHOLD );

  cv::imwrite( "canny.jpg", edges );


  if (!edges.isContinuous())
    edges = edges.clone();
  unsigned char* data = edges.data;
  int w = edges.cols;
  int h = edges.rows;

  // maximum distance in image from origo (origo in middle of image)
  double max_distance = sqrt( 2.0 ) * (h > w ? h : w) / 2.0;
  // r range
  int acc_h = max_distance * 2.0;
  // theta range
  int acc_w = 180;

  // accumulator
  unsigned int* acc = (unsigned int*)calloc( acc_h * acc_w, sizeof( unsigned int ) );

  int center_x = w / 2;
  int center_y = h / 2;

  // Voting phase
  int n_edges = 0;
  for ( int y = 0; y < h; ++y ) {
    for ( int x = 0; x < w; ++x ) {
      if ( data[ y * w + x ] > 250 ) {
        ++n_edges;
        for ( int t = 0; t < 180; ++t ) {
          double r = (x - center_x) * cos( t * DEG2RAD ) + (y - center_y) * sin( t * DEG2RAD );
          acc[ ((int)(r + max_distance)) * 180 + t ] += 1;
        }
      }
    }
  }

  std::cout << "Edge count: " << n_edges << std::endl;


  // Find maximum accumulator value
  unsigned int max_acc = 0;
  for ( int r = 0; r < acc_h; ++r ) {
      for ( int t = 0; t < acc_w; ++t ) {
          if ( acc[ r * acc_w + t ] > max_acc )
              max_acc = acc[ r * acc_w + t ];
      }
  }

  std::cout << "max_acc: " << max_acc << std::endl;

  // Create image (rows = r, cols = theta)
  cv::Mat acc_img( acc_h, acc_w, CV_8UC1 );

  for ( int r = 0; r < acc_h; ++r ) {
    for ( int t = 0; t < acc_w; ++t ) {
        unsigned int value = acc[ r * acc_w + t ];
        acc_img.at<uchar>(r, t) = static_cast<uchar>(255.0 * value / max_acc);
      }
  }

  cv::imwrite("accumulator.jpg", acc_img);


  // Select local maxima
  int threshold = h > w ? h : w;
  threshold /= 4;
  threshold = max_acc * 0.8;

  int max_count = 50;
  int* lines = (int*)malloc( max_count * 2 * sizeof( int ) );
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
          lines[ count * 2 ] = r;
          lines[ count * 2 + 1 ] = t;
          ++count;
        }
      }
    }
  }

  std::cout << count << " lines:\n";
//  for ( int i = 0; i < count; ++i )
//    std::cout << "r = " << lines[ i * 2 ]
//              << ", theta = " << lines[ i * 2 + 1 ] << std::endl;

  cv::Mat result = img.clone();

  for ( int i = 0; i < count; ++i ) {
    int r_idx = lines [i * 2 ];
    int theta_deg = lines[ i * 2 + 1 ];

    double r = r_idx - max_distance;
    double theta = theta_deg * DEG2RAD;

    double a = cos( theta );
    double b = sin( theta );

    // Point on the line
    double x0 = center_x + a * r;
    double y0 = center_y + b * r;

    // Two far-away points defining the line
    cv::Point p1(
        cvRound( x0 + 10000 * (-b) ),
        cvRound( y0 + 10000 * ( a) )
    );

    cv::Point p2(
        cvRound( x0 - 10000 * (-b) ),
        cvRound( y0 - 10000 * ( a) )
    );

    cv::line(
        result,
        p1,
        p2,
        cv::Scalar( 0, 0, 255 ), // red
        5,
        cv::LINE_AA
    );
  }

  cv::imwrite("out.jpg", result);


  free( acc );
  free( lines );

  return 0;
}
