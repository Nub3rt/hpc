#include <stdlib.h>
#include <opencv2/opencv.hpp>

const int GAUSSIAN_KERNEL_SIZE = 5;
const double GAUSSIAN_SIGMA = 2.0;
const double CANNY_LOW_THRESHOLD = 150.0;
const double CANNY_HIGH_THRESHOLD = 200.0;

int main( int argc, char** argv ) {
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

  cv::imwrite( "out.jpg", edges );
  return 0;
}
