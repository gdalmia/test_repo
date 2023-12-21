// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once


namespace Philips { namespace Workflow {

	//! Class which has basic data to represent an image
	class Image
	{
	public:

		Image();
			
		Image(char*					pointer
			, int						bytesPerPixel
			, int						width
			, int						height
		);

		virtual ~Image();
		
		Image(const Image& source);

		Image& operator=(const Image& source);
		
		bool operator== (const Image& image)	const;
		
		bool operator!= (const Image& image)	const;
		
		//! Copies image passed
		void copyFrom(const Image& source);

		//! calculates and returns capacity in bytes
		int capacityInBytes() const;

		//! How many bytes does each pixel has. 
		int bytesPerPixel() const;

		//! Width of the image in pixels. 
		int width() const;

		//! Height of the image in pixels. Must be greater than 0. 
		int height() const;

		//! Pointer to pixel data
		char* pointer() const;

	protected:
		char* _pointer;				//TICS !INT#002: Member variable needs to be protected to be accessed in imagebuffer
		int _bytesPerPixel;			//TICS !INT#002: Member variable needs to be protected to be accessed in imagebuffer
		int _width;					//TICS !INT#002: Member variable needs to be protected to be accessed in imagebuffer
		int _height;				//TICS !INT#002: Member variable needs to be protected to be accessed in imagebuffer

	};
}}