#ifndef SIMPLEGRAYIMAGE_H_
#define SIMPLEGRAYIMAGE_H_

#include <string>
#include <stdexcept>
#include <cassert>              // useful

typedef unsigned char byte;     // usual type for (gray) pixels

class SimpleGrayImage
{
	int	w;		///< width of the image
	int	h;		///< height of the image
	byte	*pixels;	///< pointer to the pixel memory
	byte	**rows;		///< pointer array to rows

	void alloc_mem (int wid, int hig);	// allocate and init memory (internal)
	void init_attributes();		// initialize (internal)

public:
	/// Default constructor: creates empty image.
	/// not normally used.
	SimpleGrayImage();

	/// Constructor: create image of given size.
	/// The pixels are not initialized.
	/// @param wid  width of the desired image
	/// @param hid  heigth of the desired image
	SimpleGrayImage(int wid, int hig);

	/// Destructor: releases all data.
	/// Free all allocated memory.
	virtual ~SimpleGrayImage ();		// destructor

	/// return the width of the image
	int width() const  { return w; }

	/// return the height of the image
	int height() const { return h; }

	/// return pointer to array of row pointers in byte**p = (byte**)img;
	operator byte ** () { return rows; }

	/// constructor: Read PGM image (depth 255) from file.
	/// Reads a PGM file into this object.
	/// The value range for the pixels must be 0..255.
	/// @param filename   name of image file
	SimpleGrayImage(const std::string& filename);

	/// read only index operator
	/// image[i] will return a pointer to the image row i
	/// thuss offering access to pixel[y][x]
	const byte * operator [] (int i) const;	// access row[i]

	/// index operator
	/// image[i] will return a pointer to the image row i
	/// thuss offering access to pixel[y][x]
	byte * operator [] (int i);		// access row[i]

	/// Write image to file in PGM format.
	/// @param filename   name of image file to be written
	void writeToFile (const std::string& filename) const;

	/// Resize the image to the given size.
	/// The pixels are not initialized.
	/// @param wid  new desired width
	/// @param hid  new desired heigth
	void resize(int wid, int hig);

};

#endif /*SIMPLEGRAYIMAGE_H_*/
