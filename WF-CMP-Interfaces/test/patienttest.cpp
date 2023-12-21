#include <gtest/gtest.h>

#include <workflow.h>

using namespace Philips::Workflow;

//! Tests the behavior of Study class
class PatientTest : public testing::Test
{
	
};

PersonName name()
{
	return { {
			{ PersonName::Group::Alphabetic,	PersonName::Component{ "","","","","" } }
		,	{ PersonName::Group::Phonetic,		PersonName::Component{ "","","","","" } }
		,	{ PersonName::Group::Ideographic,	PersonName::Component{ "","","","","" } }
		} };
}

TEST_F(PatientTest, Given_TwoPatients_WhenIdIsSame_ThenPatientsAreEqual)
{
	Patient patient1;
	patient1.id = "id";

	Patient patient2;
	patient2.id = "id";

	ASSERT_EQ(patient1, patient2);
}

TEST_F(PatientTest, Given_TwoPatients_WhenNameAreSame_ThenPatientsAreEqual)
{
	Patient patient1;
	patient1.id = "id";
	patient1.name = name();

	Patient patient2;
	patient2.id = "id";
	patient2.name = name();

	ASSERT_EQ(patient1, patient2);
}

TEST_F(PatientTest, Given_TwoPatients_WhenBirthDateAreSame_ThenPatientsAreEqual)
{
	const auto date = DateTime::now();

	Patient patient1;
	patient1.id = "id";
	patient1.name = name();
	patient1.birthDate = date;

	Patient patient2;
	patient2.id = "id";
	patient2.name = name();
	patient2.birthDate = date;

	ASSERT_EQ(patient1, patient2);
}

TEST_F(PatientTest, Given_TwoPatients_WhenGendersAreSame_ThenPatientsAreEqual)
{
	const auto date = DateTime::now();

	Patient patient1;
	patient1.id = "id";
	patient1.name = name();
	patient1.birthDate = date;
	patient1.gender = Patient::Gender::Male;

	Patient patient2;
	patient2.id = "id";
	patient2.name = name();
	patient2.birthDate = date;
	patient2.gender = Patient::Gender::Male;

	ASSERT_EQ(patient1, patient2);
}

TEST_F(PatientTest, Given_TwoPatients_WhenIdAreDifferent_ThenPatientsAreNotEqual)
{
	Patient patient1;
	patient1.id = "id1";

	Patient patient2;
	patient2.id = "id2";

	ASSERT_NE(patient1, patient2);
}

TEST_F(PatientTest, Given_TwoPatients_WhenNameAreDifferent_ThenPatientsAreNotEqual)
{
	Patient patient1;
	patient1.id = "id";
	patient1.name = name();

	Patient patient2;
	patient2.id = "id";

	ASSERT_NE(patient1, patient2);
}

TEST_F(PatientTest, Given_TwoPatients_WhenBirthDateAreDifferent_ThenPatientsAreNotEqual)
{
	Patient patient1;
	patient1.id = "id";
	patient1.name = name();
	patient1.birthDate = DateTime::now();

	Patient patient2;
	patient2.id = "id";
	patient2.name = name();

	ASSERT_NE(patient1, patient2);
}

TEST_F(PatientTest, Given_TwoPatients_WhenGendersAreDifferent_ThenPatientsAreNotEqual)
{
	const auto date = DateTime::now();

	Patient patient1;
	patient1.id = "id";
	patient1.name = name();
	patient1.birthDate = date;
	patient1.gender = Patient::Gender::Male;

	Patient patient2;
	patient2.id = "id";
	patient2.name = name();
	patient2.birthDate = date;
	patient2.gender = Patient::Gender::Female;

	ASSERT_NE(patient1, patient2);
}

