/** @file SimpleGrayImage.cc
 *  Image Processing Lecture, WS 07/08, Uni Koblenz.
 *  Simple image class to read and write PGM Images.
 *
 *  @author     Detlev Droege
 *  @author     Frank Schmitt
 *  @created    November 2007
 */
#include <cassert>
#include <cctype>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string.h>

#include "SimpleGrayImage.h"


void SimpleGrayImage::alloc_mem(int wid, int hig)
{
	if (pixels) delete[] pixels; //delete old pixel data
	if (rows) delete[] rows; //delete old row pointers
	pixels = new byte[wid * hig];	// get memory for pixels
	rows = new byte*[h];		// get memory for row pointers
	byte *pp = pixels;			// let pp point to 1. pixel row
	for (int i = 0; i < h; i++) {	// for every row i
		rows[i] = pp;			// make rows[i] point to it
		pp += w;			// advance pp to next row
	}
}

void SimpleGrayImage::init_attributes()
{
	w = h = 0;
	pixels = NULL;
	rows = NULL;
}

SimpleGrayImage::SimpleGrayImage()
{
	init_attributes();
}

SimpleGrayImage::~SimpleGrayImage()
{
	delete [] pixels;
	delete [] rows;
	pixels = NULL;
	rows = NULL;
}

SimpleGrayImage::SimpleGrayImage(int wid, int hig)
{
	init_attributes();
	resize(wid, hig);
}

void SimpleGrayImage::resize(int wid, int hig)
{
	assert ((wid > 0) && (hig > 0));
	w = wid;
	h = hig;
	alloc_mem(wid, hig);
}


SimpleGrayImage::SimpleGrayImage(const std::string& filename)
{
	init_attributes();

	if (filename == "") throw std::runtime_error("Unable to load image from file: Empty filename");
	std::ifstream in(filename.c_str(),std::ios::binary);
	if (!in.good()) throw std::runtime_error("Unable to open file "+filename);
	// check for file type
	std::string filetype;
	std::getline(in,filetype);
	if (filetype.find("P2",0)==0 || filetype.find("P5",0)==0)
	{
		// A comment can follow; or the size of the image
		std::string curline;
		do {
			std::getline(in,curline);
		} while (curline[0] == '#' || curline.length()==0);
		// The last line wasn't a comment, extract the image size
		std::istringstream iss(curline);
		iss >> w;
		iss >> h;
		if (w<=0 || h<=0) throw std::runtime_error("Invalid image size in file "+filename);
		// allocate data for the bitmap
		alloc_mem(w, h);
		// read maxvalue
		std::getline(in,curline);
		if (filetype.find("P2",0)==0)
		{
			for (int p = 0; p < w*h; p++)
			{
				//We can't read directly into image.data[p] as we
				//would only get one char and not the whole number,
				//e.g. 2 instead of 255
				int d;
				in >> d;
				pixels[p]=d;
			}
		}
		else
		{
			// data in binary format
			in.read((char*)pixels, w*h);
		}
	}
	else
	{
		throw std::runtime_error("Unsupported image type in file: "+filename);
	}
	in.close();
}

const byte * SimpleGrayImage::operator [] (int i) const
{
	if (! ((i >= 0) && (i < h))){
		std::cerr << "oops: access to row " << i << ", but " << h-1 << " is maximum\n" << std::endl;
	}
	assert ((i >= 0) && (i < h));
	return rows[i];
}

byte * SimpleGrayImage::operator [] (int i)
{
	if (! ((i >= 0) && (i < h))){
		std::cerr << "oops: access to row " << i << ", but " << h-1 << " is maximum\n" << std::endl;
	}
	assert ((i >= 0) && (i < h));
	return rows[i];
}

void SimpleGrayImage::writeToFile (const std::string& filename) const
{
	// store image in PGM format.
	if (filename == "") throw std::runtime_error("Unable to save image to file: Empty filename");
	std::ofstream out(filename.c_str(),std::ios::binary);
	if (!out.good()) throw std::runtime_error("Unable to open file "+filename+" for writing");
	// check for file type
	out << "P5\n" << w << " " << h << "\n255\n";
	out.write((char*)pixels, w*h);
	out.close();
}



