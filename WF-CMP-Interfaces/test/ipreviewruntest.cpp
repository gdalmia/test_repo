// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/ipreviewrun.h"
#include <gtest/gtest.h>


namespace Test
{
	using namespace Philips::Workflow;

	//! Tests the behavior of IPreviewRun class
	class IPreviewRunTest : public testing::Test
	{
	};

	TEST_F(IPreviewRunTest, Given_TwoIPreviewRunAttributes_When_KeysAndValuesAreSame_Then_VerifyAtrributesAreEqual)
	{
		const IPreviewRun::Meta::Attribute attribute1{ "key", "value" };
		const IPreviewRun::Meta::Attribute attribute2{ "key", "value" };
		
		ASSERT_TRUE(attribute1 == attribute2);
		ASSERT_FALSE(attribute1 != attribute2);
	}

	TEST_F(IPreviewRunTest, Given_TwoIPreviewRunAttributes_When_KeysAreSameAndValuesAreNotSame_Then_VerifyAtrributesAreNotEqual)
	{
		const IPreviewRun::Meta::Attribute attribute1 { "key", "value"};
		const IPreviewRun::Meta::Attribute attribute2{ "key", "newValue" };

		ASSERT_TRUE(attribute1 != attribute2);
		ASSERT_FALSE(attribute1 == attribute2);
	}

	TEST_F(IPreviewRunTest, Given_TwoIPreviewRunBookmarks_When_IndexesAndTypeAreSame_Then_VerifyBookmarksAreEqual)
	{
		const IPreviewRun::Meta::Bookmark bookmark1{ IPreviewRun::Meta::Bookmark::Type::Bookmark, 5 };
		const IPreviewRun::Meta::Bookmark bookmark2{ IPreviewRun::Meta::Bookmark::Type::Bookmark, 5 };

		ASSERT_TRUE(bookmark1 == bookmark2);
		ASSERT_FALSE(bookmark1 != bookmark2);
	}

	TEST_F(IPreviewRunTest, Given_TwoIPreviewRunBookmarks_When_IndexesAndTypeAreNotSame_Then_VerifyBookmarksAreNotEqual)
	{
		const IPreviewRun::Meta::Bookmark bookmark1{ IPreviewRun::Meta::Bookmark::Type::Bookmark, 10 };
		const IPreviewRun::Meta::Bookmark bookmark2{ IPreviewRun::Meta::Bookmark::Type::Bookmark, 5 };

		ASSERT_FALSE(bookmark1 == bookmark2);
		ASSERT_TRUE(bookmark1 != bookmark2);
	}
	
}