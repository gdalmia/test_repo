// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/image.h"
#include <string>

namespace Philips {	namespace Workflow {

		Image::Image()
			: _pointer(nullptr)
			, _bytesPerPixel(0)
			, _width(0)
			, _height(0)
		{

		}

		Image::Image(char*					pointer
			, int						bytesPerPixel
			, int						width
			, int						height
		) : _pointer(pointer)
			, _bytesPerPixel(bytesPerPixel)
			, _width(width)
			, _height(height)
		{

		}

		Image::~Image()
		{

		}

		Image::Image(const Image& source) : _pointer(source._pointer)
			, _bytesPerPixel(source._bytesPerPixel)
			, _width(source._width)
			, _height(source._height)
		{

		}

		Image& Image::operator=(const Image& source)
		{
			_width = source._width;
			_height = source._height;
			_bytesPerPixel = source._bytesPerPixel;
			_pointer = source._pointer;
			return *this;
		}

		int Image::capacityInBytes() const
		{
			return _width * _height * _bytesPerPixel;
		}

		bool Image::operator== (const Image& image)	const
		{
			return	((_width == image._width)
				&& (_height == image._height)
				&& (_bytesPerPixel == image._bytesPerPixel)
				&& (_pointer == image._pointer)
				);
		}

		bool Image::operator!= (const Image& image)	const
		{
			return	((_width != image._width)
				|| (_height != image._height)
				|| (_bytesPerPixel != image._bytesPerPixel)
				|| (_pointer != image._pointer)
				);
		}

		void Image::copyFrom(const Image& source)
		{
			_width = source._width;
			_height = source._height;
			_bytesPerPixel = source._bytesPerPixel;
			int frameSize = source._width * source._height * source._bytesPerPixel;
			memcpy(_pointer, source._pointer, frameSize);
		}

		int Image::bytesPerPixel() const
		{
			return _bytesPerPixel;
		}

		int Image::width() const
		{
			return _width;
		}

		int Image::height() const
		{
			return _height;
		}

		char* Image::pointer() const
		{
			return _pointer;
		}
	}}
