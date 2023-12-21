// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
//TICS -INT#027 : Parameterized so cannot override
//TICS -OAL#011 : using smart pointers to create but still throws error so suppressing

#include <workflow.h>
#include <gtest/gtest.h>

using namespace Philips::Workflow;
namespace Test
{
	typedef std::tuple<int, int, int> TestParams;
	//Tests the behaviour of image
	class ImageTest : public testing::TestWithParam<TestParams>
	{

	};

	INSTANTIATE_TEST_CASE_P(test, ImageTest, ::testing::Values(TestParams{ 1, 2, 3 }, TestParams{ 2,2, 3 }, TestParams{ 1, 3, 3 }, TestParams{ 1,2,4 }));

	TEST_P(ImageTest, Given_Image_When_Copied_Then_VerifyMemberVariablesOfCopiedAndOriginalAreSame)
	{
		int b(1), w(2), h(3);
		std::unique_ptr<char[]> pointer(std::make_unique<char[]>(w*b*h));
		Philips::Workflow::Image image(pointer.get(), b, w, h);
		Philips::Workflow::Image image1(image);

		ASSERT_EQ(image.width(), w);
		ASSERT_EQ(image.height(), h);
		ASSERT_EQ(image.bytesPerPixel(), b);
		ASSERT_TRUE(image == image1);
		ASSERT_FALSE(image != image1);

	}

	TEST_P(ImageTest, Given_Image_WhenComparedWithDifferentDimension_Then_ImagesAreNotSame)
	{
		int b(1), w(2), h(3);
		std::unique_ptr<char[]> pointer(std::make_unique<char[]>(w*b*h));
		Philips::Workflow::Image image(pointer.get(), b, w, h);

		const auto params = GetParam();
		const auto component1 = std::get<0>(params);
		const auto component2 = std::get<1>(params);
		const auto component3 = std::get<2>(params);
		int b1(component1), w1(component2), h1(component3);
		std::unique_ptr<char[]> pointer1(std::make_unique<char[]>(w1 * h1 * b1));
		Philips::Workflow::Image image1(pointer1.get(), b1, w1, h1);

		ASSERT_FALSE(image == image1);
		ASSERT_TRUE(image != image1);
	}

	TEST_P(ImageTest, Given_Image_WhenCopyFromCalled_Then_PixelDataIsSameAndReturnsTrue)
	{
		int b(1), w(2), h(3);
		std::unique_ptr<char[]> pointer(std::make_unique<char[]>(w * h * b));
		{
			for (int i = 0; i < w * h * b; ++i)
			{
				pointer.get()[i] = static_cast<char>(i);
			}
		}
		Philips::Workflow::Image image(pointer.get(), b, w, h);

		Philips::Workflow::Image image1(image);
		image1.copyFrom(image);

		bool result(true);
		{
			for (int i = 0; i < w * h * b; ++i)
			{
				result = result && image.pointer()[i] == image1.pointer()[i];
			}
		}
		ASSERT_TRUE(result);
	}
}
