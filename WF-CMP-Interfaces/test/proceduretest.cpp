// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
//TICS -INT#027 -OAL#011 -ORG#011
// INT#027 OAL#011 occur due to Google test Parameterized test framework. Hence, they are being suppressed
// ORG#011 occurs because the tests are not enclosed in a namespace. Hence it is being suppressed

#include <gtest/gtest.h>

#include <workflow.h>

using namespace Philips::Workflow;

//! Tests the behavior of Procedure class
class ProcedureTest : public testing::Test
{
};

TEST_F(ProcedureTest, Given_TwoProcedures_When_IdsAreSame_Then_VerifyProceduresAreEqual)
{
    Procedure procedure1;
    procedure1.id = "id";

    Procedure procedure2;
    procedure2.id = "id";

    ASSERT_EQ(procedure1, procedure2) << "Procedures are not equal";
}

TEST_F(ProcedureTest, Given_TwoProcedures_When_IdsAreDifferent_Then_VerifyProceduresAreNotEqual)
{
    Procedure procedure1;
    procedure1.id = "id1";

    Procedure procedure2;
    procedure2.id = "id2";

    ASSERT_NE(procedure1, procedure2) << "Procedures are equal";
}

TEST_F(ProcedureTest, Given_TwoProceduresWithDifferentKeys_When_IdsAreSame_Then_VerifyProceduresAreEqual)
{
    Procedure procedure1("key1");
    procedure1.id = "id";

    Procedure procedure2("key2");
    procedure2.id = "id";

    ASSERT_EQ(procedure1, procedure2) << "Procedures are not equal";
}

TEST_F(ProcedureTest, Given_TwoProceduresWithDifferentKeys_When_IdsAreDifferentThen_VerifyProceduresAreNotEqual)
{
    Procedure procedure1("key1");
    procedure1.id = "id1";

    Procedure procedure2("key2");
    procedure2.id = "id2";

    ASSERT_NE(procedure1, procedure2) << "Procedures are equal";
}

TEST_F(ProcedureTest, Given_TwoProceduresWithSameParent_When_IdsAreDifferent_Then_VerifyProceduresAreNotEqual)
{
    Study study1("key1");
    study1.id = "id1";

    Procedure procedure1(study1);
    procedure1.id = "id2";
    Procedure procedure2(study1);
    procedure2.id = "id3";

    ASSERT_NE(procedure1, procedure2) << "Procedures are equal";
}

typedef std::pair<std::string, Procedure::Type> TestParams;

class ValidProcedureTypeStringConversionTest : public ::testing::TestWithParam<TestParams>
{
};

INSTANTIATE_TEST_CASE_P(Given_NValidFormats, ValidProcedureTypeStringConversionTest, ::testing::Values(
	TestParams{ "Coronary", Procedure::Type::Coronary },
	TestParams{ "Peripheral", Procedure::Type::Peripheral },
	TestParams{ "Unknown", Procedure::Type::Unknown }));

TEST_P(ValidProcedureTypeStringConversionTest, When_ToStringCalledWithProcedureType_Then_ExpectedStringReturned)
{
	ASSERT_EQ(Procedure::toString(GetParam().second), GetParam().first);
}

TEST_P(ValidProcedureTypeStringConversionTest, When_ParseTypeCalled_Then_ExpectedProcedureTypeReturned)
{
	ASSERT_EQ(Procedure::parseType(GetParam().first), GetParam().second);
}

class InvalidFromStringProcedureTypeTest : public ::testing::TestWithParam<std::string>
{
};

INSTANTIATE_TEST_CASE_P(Given_NInvalidFormats, InvalidFromStringProcedureTypeTest, ::testing::Values("", "Coro nary", "coronary"));

TEST_P(InvalidFromStringProcedureTypeTest, When_ParseTypeCalled_Then_UnknownProcedureTypeReturned)
{
	ASSERT_EQ(Procedure::parseType(GetParam()), Procedure::Type::Unknown);
}