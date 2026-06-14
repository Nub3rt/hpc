#include <iostream>

#include <opencv2/opencv.hpp>

#define IN_PATH "in.jpg"
#define OUT_PATH "canny.png"

const int GAUSSIAN_KERNEL_SIZE = 5;
const double GAUSSIAN_SIGMA = 2.0;
const double CANNY_LOW_THRESHOLD = 150.0;
const double CANNY_HIGH_THRESHOLD = 200.0;

int main() {
  cv::Mat_<uchar> img = cv::imread( IN_PATH, cv::IMREAD_GRAYSCALE );
  if ( img.empty() ) {
    std::cout << "Could not load image " << IN_PATH << std::endl;
    return( 1 );
  }

  cv::GaussianBlur(
      img,
      img,
      cv::Size( GAUSSIAN_KERNEL_SIZE, GAUSSIAN_KERNEL_SIZE ),
      GAUSSIAN_SIGMA
  );
  cv::Canny( img, img, CANNY_LOW_THRESHOLD, CANNY_HIGH_THRESHOLD );

  cv::imwrite( OUT_PATH, img );


  if (!img.isContinuous())
    img = img.clone();

  int n_edges = 0;
  for ( auto& pixel : img )
    if ( pixel > 250 )
      ++n_edges;

  std::cout << "Number of edges: " << n_edges << std::endl;

  return 0;
}
