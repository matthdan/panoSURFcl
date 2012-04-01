/*
* This file is part of Parallel SURF, which implements the SURF algorithm
* using multi-threading.
*
* Copyright (C) 2010 David Gossow
*
* It is based on the SURF implementation included in Pan-o-matic 0.9.4,
* written by Anael Orlinski.
*
* Parallel SURF is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* Parallel SURF is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "KeyPointDetector.h"
#include "KeyPointDescriptor.h"
#include "Image.h"

#include "SimpleGrayImage.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <stdlib.h>


//define insertor class which collects keypoints in a vector
class PanoKeyPointVectInsertor : public libsurf::KeyPointInsertor
{
public:
  PanoKeyPointVectInsertor ( std::vector<libsurf::KeyPoint>& keyPoints ) : m_KeyPoints ( keyPoints ) {};
  inline virtual void operator() ( const libsurf::KeyPoint &keyPoint )
  {
    m_KeyPoints.push_back ( keyPoint );
  }
private:
  std::vector<libsurf::KeyPoint>& m_KeyPoints;
};

void computePanoKeyPoints( SimpleGrayImage &image, std::vector<libsurf::KeyPoint> &keyPoints )
{
  unsigned int width = image.width();
  unsigned int height = image.height();
  double **pixels = libsurf::Image::AllocateImage( width, height );
  
  //convert image to double, normalize to [0..1]
  static const double norm=100.0 / 255.0;
  
  for(unsigned int y = 0; y < height; ++y)
    {
      for(unsigned int x = 0; x < width; ++x)
	{
	  pixels[y][x] = norm*double( image[y][x] );
	}
    }
  
  //Construct integral image
  libsurf::Image intImage ( pixels, width, height );
  
  //Construct detector & descriptor
  libsurf::KeyPointDetector detector;
  libsurf::KeyPointDescriptor descriptor ( intImage, false );

  //Insertor inserts into keyPoints
  PanoKeyPointVectInsertor insertor ( keyPoints );

  //Detect & describe
  std::cout << "Computing keypoints with Panomatic SURF.. ";
  
  detector.detectKeypoints( intImage, insertor );
  
  for (std::vector<libsurf::KeyPoint>::iterator it = keyPoints.begin(); it != keyPoints.end(); ++it)
    {
      descriptor.assignOrientation( *it );
      descriptor.makeDescriptor( *it );
    }
  
  std::cout << "Found " << keyPoints.size() << " keypoints." << std::endl;
  
  libsurf::Image::DeallocateImage( pixels, height );
}



int main( )
{
  srand( clock() );
  
  //Read image
  SimpleGrayImage image ( "data/test.pgm" );
  std::cout << "Image size: " << image.width() << "x" << image.height() << std::endl;
  
  std::vector<libsurf::KeyPoint> panoKeyPoints;
  computePanoKeyPoints( image, panoKeyPoints );
  
  return 0;
}
