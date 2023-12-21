// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/imagebuffer.h"

namespace Philips { namespace Workflow {
	
	ImageBuffer::ImageBuffer()
	{
		
	}

	ImageBuffer::ImageBuffer(
			int					width
		,	int					height
		,	int					bytesPerPixel
		):	Image			()
	{
		initialize(width, height, bytesPerPixel);
	}

	ImageBuffer::~ImageBuffer()
	{
		if(m_buffer != nullptr)
		{
			m_buffer.reset(nullptr);
			_pointer= nullptr;
		}
	}

	ImageBuffer::ImageBuffer(const ImageBuffer& source
		):	Image(source)
	{
		m_buffer = std::make_unique<char[]>(source.capacityInBytes());
		_pointer =m_buffer.get();
		memcpy(_pointer, source._pointer, source.capacityInBytes());
	}

	ImageBuffer& ImageBuffer::operator=(const ImageBuffer& source)
	{
		if (this != &source)
		{
			m_buffer.reset(nullptr);

			Image::operator=(source);
			m_buffer= std::make_unique<char[]>(source.capacityInBytes());
			_pointer = m_buffer.get();
			memcpy(_pointer, source._pointer, source.capacityInBytes());
		}
		return *this;
	}

	ImageBuffer::ImageBuffer(ImageBuffer&& source) noexcept
		: Image(std::move(source))
	{
		//Copy the pointer
		m_buffer = std::move(source.m_buffer);
		_pointer= m_buffer.get();

		//Set source to null
		source.m_buffer.reset(nullptr);
		source._pointer= nullptr;
		source._width = 0;
		source._height = 0;
		source._bytesPerPixel = 0;
	}

	ImageBuffer& ImageBuffer::operator=(ImageBuffer&& source) noexcept
	{
		if (this != &source)
		{
			//delete the existing data
			m_buffer.reset(nullptr);

			Image::operator=(std::move(source));

			//Copy the pointer
			m_buffer = std::move(source.m_buffer);
			_pointer = m_buffer.get();

			//Set source to null
			source.m_buffer.reset(nullptr);
			source._pointer= nullptr;
			source._width= 0;
			source._height =0 ;
			source._bytesPerPixel= 0;
		}

		return *this;
	}

	void ImageBuffer::resize(int width
		,	int					height
		,	int					bytesPerPixel)
	{
		if(m_buffer != nullptr)
		{
			m_buffer.reset(nullptr);
		}

		initialize(width, height, bytesPerPixel);
	}

	void ImageBuffer::initialize(int width_, int height_, int bytesPerPixel_)
	{
		_width = width_;
		_height = height_;
		_bytesPerPixel =bytesPerPixel_;
		m_buffer = std::make_unique<char[]>(capacityInBytes());
		_pointer = m_buffer.get();

		memset(_pointer, 0, sizeof(unsigned char)* width() * height());
	}
}}