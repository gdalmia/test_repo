// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "image.h"
#include <memory>


namespace Philips { namespace Workflow {

	//! Class which has image pixel data
	class ImageBuffer : public Image
	{
	public:
		ImageBuffer();
		ImageBuffer(int width, int height, int bytesPerPixel);
		ImageBuffer(const ImageBuffer& source);
		explicit ImageBuffer(ImageBuffer&& source) noexcept;
		virtual ~ImageBuffer();

		ImageBuffer& operator=		(const ImageBuffer& source);
		ImageBuffer& operator=		(ImageBuffer&& source) noexcept;

		//! resizes the image
		virtual void resize(int width, int height, int bytesPerPixel);

	protected:
		void initialize(int width, int height, int bytesPerPixel);

	private:
		std::unique_ptr<char[]>			m_buffer;
	};
}}
