// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "workflow.h"
#include "gtest\gtest.h"

using namespace Philips::Workflow;
namespace Test
{
	class ImageBufferTest : public ::testing::Test
	{
	public:
		bool compare(Philips::Workflow::Image& first, Philips::Workflow::Image& second) const
		{
			bool result(true);

			result = (first.width() == second.width());
			result = result && (first.height() == second.height());
			result = result && (first.bytesPerPixel() == second.bytesPerPixel());
			result = result && (first.capacityInBytes() == second.capacityInBytes());

			int frameSize(first.width() * first.height() * first.bytesPerPixel());
			for (int i = 0; i < frameSize; ++i)
			{
				result = result && first.pointer()[i] == second.pointer()[i];
			}
			return result;
		}

		void assertMovedConstructor(ImageBuffer moved, int width, int height, int bytesperpixel)
		{
			ASSERT_EQ(moved.width(), width);
			ASSERT_EQ(moved.height(), height);
			ASSERT_EQ(moved.bytesPerPixel(), bytesperpixel);
			ASSERT_EQ(moved.capacityInBytes(), width * height * bytesperpixel);
		}
	};

	TEST_F(ImageBufferTest, Given_Image_When_ConstructorInitialized_Then_VerifyMemberVariablesInitialized)
	{
		int b(1), w(2), h(3);
		ImageBuffer image(w, h, b);

		assertMovedConstructor(image, w, h, b);
		ASSERT_TRUE(image.pointer() != nullptr);
	}

	TEST_F(ImageBufferTest, Given_Image_When_CopyConstructorCopied_Then_CompareMemberVariablesOfCopiedAndOriginalAreSame)
	{
		int b(1), w(2), h(3);
		ImageBuffer image(w, h, b);
		for (int i = 0; i < image.capacityInBytes(); ++i)
		{
			image.pointer()[i] = static_cast<char>(i);
		}

		ImageBuffer copy(image);

		assertMovedConstructor(image, w, h, b);
		ASSERT_NE(image.pointer(), copy.pointer());
		ASSERT_TRUE(compare(image, copy));
	}

	TEST_F(ImageBufferTest, Given_Image_When_AssignedFromDifferentImage_Then_VariablesOfAssignedAndOriginalImageAreSame)
	{
		int b(1), w(2), h(3);
		ImageBuffer image(w, h, b);
		ImageBuffer copy(image);
		ImageBuffer assign(1, 1, 1);
		assign = image;

		assertMovedConstructor(image, w, h, b);
		ASSERT_NE(image.pointer(), copy.pointer());
		ASSERT_TRUE(compare(image, assign));
	}

	TEST_F(ImageBufferTest, Given_Image_When_AssignedTwice_Then_VariablesOfAssignedAndOriginalImageAreSame)
	{
		int b(1), w(2), h(3);
		ImageBuffer image(w, h, b);
		ImageBuffer copy(image);
		ImageBuffer assign(1, 1, 1);
		assign = image;
		assign = assign;
		ASSERT_TRUE(compare(image, assign));
	}

	TEST_F(ImageBufferTest, Given_Image_When_Moved_Then_VerifyMemeberVariablesInitializedAndMovedAreSame)
	{
		int b(1), w(2), h(3);
		ImageBuffer image(w, h, b), reference(w, h, b);
		for (int i = 0; i < image.capacityInBytes(); ++i)
		{
			image.pointer()[i] = static_cast<char>(i);
			reference.pointer()[i] = static_cast<char>(i);
		}

		auto temp = image.pointer();
		ImageBuffer moved1(std::move(image));

		assertMovedConstructor(moved1, w, h, b);
		ASSERT_EQ(moved1.pointer(), temp);
		ASSERT_TRUE(compare(moved1, reference));
	}

	TEST_F(ImageBufferTest, Given_Image_When_MovedTwice_Then_VerifyMemeberVariablesInitializedAndMovedAreSame)
	{
		int b(1), w(2), h(3);
		ImageBuffer image(w, h, b), reference(w, h, b);
		for (int i = 0; i < image.capacityInBytes(); ++i)
		{
			image.pointer()[i] = static_cast<char>(i);
			reference.pointer()[i] = static_cast<char>(i);
		}

		auto temp = image.pointer();
		ImageBuffer moved1(std::move(image));
		ImageBuffer moved2;
		moved2 = std::move(moved1);

		assertMovedConstructor(moved2, w, h, b);
		ASSERT_EQ(moved2.pointer(), temp);
		ASSERT_TRUE(compare(moved2, reference));
	}

	TEST_F(ImageBufferTest, Given_Image_When_Resized_Then_VerifyResizedMemberVariablesAreAppliedToImage)
	{
		int b(1), w(2), h(3);
		ImageBuffer image(w, h, b);

		w = 1;
		h = 4;
		b = 2;
		image.resize(w, h, b);

		assertMovedConstructor(image, w, h, b);
		ASSERT_TRUE(image.pointer() != nullptr);
	}
}