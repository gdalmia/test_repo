// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <gtest/gtest.h>

#include <workflow.h>

using namespace Philips::Workflow;

//! Tests the behavior of Study class
class StudyTest : public testing::Test
{
	
};

TEST_F(StudyTest, Given_TwoStudies_When_IdsAreSame_Then_StudiesAreEqual)
{
	Study study1;
	study1.id = "id";

	Study study2;
	study2.id = "id";
	
	ASSERT_EQ(study1, study2) << "Studies are not equal";
}

TEST_F(StudyTest, Given_TwoStudies_When_IdsAreDifferentThen_StudiesAreNotEqual)
{
	Study study1;
	study1.id = "id1";

	Study study2;
	study2.id = "id2";
	
	ASSERT_NE(study1, study2) << "Studies are equal";
}

TEST_F(StudyTest, Given_TwoStudies_When_InstaneUIDsAreSame_Then_StudiesAreEqual)
{
	Study study1;
	study1.id = "id";
	study1.instanceUid = "uid";

	Study study2;
	study2.id = "id";
	study2.instanceUid = "uid";

	ASSERT_EQ(study1, study2) << "Studies are not equal";
}

TEST_F(StudyTest, Given_TwoStudies_When_InstaneUIDsAreDifferentThen_StudiesAreNotEqual)
{
	Study study1;
	study1.id = "id";
	study1.instanceUid = "uid1";

	Study study2;
	study2.id = "id";
	study2.instanceUid = "uid2";

	ASSERT_NE(study1, study2) << "Studies are equal";
}

TEST_F(StudyTest, Given_TwoStudies_When_InstaneUIDsAreDifferentAndOneIsEmptyThen_StudiesAreNotEqual)
{
	Study study1;
	study1.id = "id";
	study1.instanceUid = "uid1";

	Study study2;
	study2.id = "id";
	study2.instanceUid = {};

	ASSERT_NE(study1, study2) << "Studies are equal";
}

TEST_F(StudyTest, Given_TwoStudiesWithDifferentKeys_When_IdsAreSame_Then_StudiesAreEqual)
{
	Study study1("key1");
	study1.id = "id";

	Study study2("key2");
	study2.id = "id";

	ASSERT_EQ(study1, study2) << "Studies are not equal";
}

TEST_F(StudyTest, Given_TwoStudiesWithDifferentKeys_When_IdsAreDifferentThen_StudiesAreNotEqual)
{
	Study study1("key1");
	study1.id = "id1";

	Study study2("key2");
	study2.id = "id2";

	ASSERT_NE(study1, study2) << "Studies are equal";
}

TEST_F(StudyTest, Given_TwoStudiesWithDifferentKeys_When_InstaneUIDsAreSame_Then_StudiesAreEqual)
{
	Study study1("key1");
	study1.id = "id";
	study1.instanceUid = "uid";

	Study study2("key2");
	study2.id = "id";
	study2.instanceUid = "uid";

	ASSERT_EQ(study1, study2) << "Studies are not equal";
}

TEST_F(StudyTest, Given_TwoStudiesWithDifferentKeys_When_InstaneUIDsAreDifferentThen_StudiesAreNotEqual)
{
	Study study1("key1");
	study1.id = "id1";
	study1.instanceUid = "uid1";

	Study study2("key2");
	study2.id = "id2";
	study2.instanceUid = "uid2";

	ASSERT_NE(study1, study2) << "Studies are equal";
}

TEST_F(StudyTest, Given_TwoStudiesWithDifferentKeys_When_InstaneUIDsAreDifferentAndOneIsEmptyThen_StudiesAreNotEqual)
{
	Study study1("key1");
	study1.id = "id1";
	study1.instanceUid = "uid1";

	Study study2("key2");
	study2.id = "id";
	study2.instanceUid = {};

	ASSERT_NE(study1, study2) << "Studies are equal";
}

TEST_F(StudyTest, Given_TwoImportedStudies_When_StudyIdIsSame_Then_StudiesAreEqual)
{
	Study study1("key1");
	study1.id = "id1";
	study1.instanceUid = "uid1";
	study1.importStatus = Study::ImportStatus::InProgress;

	Study study2("key2");
	study2.id = "id1";
	study2.instanceUid = "uid1";
	study2.importStatus = Study::ImportStatus::Completed;

	ASSERT_EQ(study1, study2) << "Studies are equal";
	ASSERT_EQ(study2, study1) << "Studies are equal";
}

TEST_F(StudyTest, Given_TwoImportedStudies_When_StudyIdIsDifferent_Then_StudiesAreDifferent)
{
	Study study1("key1");
	study1.id = "id1";
	study1.instanceUid = "uid1";
	study1.importStatus = Study::ImportStatus::InProgress;

	Study study2("key2");
	study2.id = "id2";
	study2.instanceUid = "uid1";
	study2.importStatus = Study::ImportStatus::Completed;

	ASSERT_NE(study1, study2) << "Studies are equal";
	ASSERT_NE(study2, study1) << "Studies are equal";
}

TEST_F(StudyTest, Given_TwoLocalStudies_When_InstanceUidsAreSame_Then_StudiesAreSame)
{
	Study study1("key1");
	study1.id = "id1";
	study1.instanceUid = "uid1";
	study1.importStatus = Study::ImportStatus::None;

	Study study2("key2");
	study2.id = "id2";
	study2.instanceUid = "uid1";
	study2.importStatus = Study::ImportStatus::None;

	ASSERT_EQ(study1, study2) << "Studies are equal";
	ASSERT_EQ(study2, study1) << "Studies are equal";
}

TEST_F(StudyTest, Given_TwoLocalStudies_When_InstanceUidsAreDifferent_Then_StudiesAreDifferent)
{
	Study study1("key1");
	study1.id = "id1";
	study1.instanceUid = "uid1";
	study1.importStatus = Study::ImportStatus::None;

	Study study2("key2");
	study2.id = "id2";
	study2.instanceUid = "uid2";
	study2.importStatus = Study::ImportStatus::None;

	ASSERT_NE(study1, study2) << "Studies are equal";
	ASSERT_NE(study2, study1) << "Studies are equal";
}

TEST_F(StudyTest, Given_OneLocalAndOneImportedStudy_When_InstanceUidsAreSame_Then_StudiesAreDifferent)
{
	Study study1("key1");
	study1.id = "id1";
	study1.instanceUid = "uid1";
	study1.importStatus = Study::ImportStatus::None;

	Study study2("key2");
	study2.id = "id2";
	study2.instanceUid = "uid1";
	study2.importStatus = Study::ImportStatus::InProgress;

	ASSERT_NE(study1, study2) << "Studies are equal";
	ASSERT_NE(study2, study1) << "Studies are equal";
}

