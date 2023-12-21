// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <gtest/gtest.h>

#include <workflow.h>

using namespace Philips::Workflow;

//! Tests the behavior of Study class
class IRepositoryTest : public testing::Test
{
	
};

TEST_F(IRepositoryTest, Given_ModelWithKey_WhenExplicitConverterToStringCalled_Then_KeyIsReturned)
{
	const auto key("key");
	const IRepository::Model model(key);
	ASSERT_EQ(static_cast<std::string>(model), key);
	ASSERT_EQ(std::string(model), key);
}

TEST(ModelTest, When_MoveConstructorCalledWithGivenModel_Then_CreatedModelEqualToOriginalModel)
{
	const auto key = "key";
	IRepository::Model model(key);
	auto copy(std::move(model));
	
	ASSERT_EQ(static_cast<std::string>(copy), key) << "Keys are not equal";
}

TEST_F(IRepositoryTest, Given_AStudyCreatedWithKey_When_KeyIsAccessedInDerivedRepository_Then_SameKeyIsReturned)
{
	const auto key = "key";
	Study study(key);
	
	ASSERT_EQ(static_cast<std::string>(study), key) << "Keys are not equal";
}

TEST_F(IRepositoryTest, Given_TwoModels_When_KeysAreSame_Then_VerifyModelsAreEqual)
{
    const IRepository::Model model1("key1");
    const IRepository::Model model2("key1");
    ASSERT_EQ(model1, model2) << "Models are not equal";
}

TEST_F(IRepositoryTest, Given_TwoModels_When_KeysAreDifferent_Then_VerifyModelsAreNotEqual)
{
    const IRepository::Model model1("key1");
    const IRepository::Model model2("key2");
    ASSERT_NE(model1, model2) << "Models are equal";
}

