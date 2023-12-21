// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/physicianrepository.h"
#include "src/physician.h"
#include <gtest/gtest.h>

using namespace ::testing;
using namespace Philips::Workflow;

namespace Philips {namespace Workflow {namespace Test {

	class PhysicianRepositoryTest : public Test, public testing::WithParamInterface<PhysicianRepository::Physician::Type>
	{
	public:
		PhysicianRepositoryTest() :settings(std::filesystem::temp_directory_path(), true)
		{
		}

		void TearDown() override
		{
			std::filesystem::remove_all(settings.path());
		}

		void addPhysician(const int physicianCount, const PhysicianRepository::Physician::Type&  type, std::string uid = "")
		{
			for (int index = 1; index <= physicianCount; index++)
			{
				PhysicianRepository::Physician physician;
				PersonName::Component name;
				name.givenName = "AlphabeticGivenName" + std::to_string(index);
				name.familyName = "AlphabeticFamilyName" + std::to_string(index);
				name.middleName = "AlphabeticMiddleName" + std::to_string(index);
				physician.name.components.clear();
				physician.name.components.insert({ PersonName::Group::Alphabetic, name });

				name.givenName = "IdeoGraphicGivenName" + std::to_string(index);
				name.familyName = "IdeoGraphicFamilyName" + std::to_string(index);
				name.middleName = "IdeoGraphicMiddleName" + std::to_string(index);
				physician.name.components.insert({ PersonName::Group::Ideographic, name });

				name.givenName = "PhoneticGivenName" + std::to_string(index);
				name.familyName = "PhoneticFamilyName" + std::to_string(index);
				name.middleName = "PhoneticMiddleName" + std::to_string(index);
				physician.name.components.insert({ PersonName::Group::Phonetic, name });
				physician.type = type;
				physicians.emplace_back(physician);
				physicianRepository.add(physician);
			}
		}

		void assertAlphabeticName(const std::vector<PhysicianRepository::Physician>& retrievedPhysician , int index)
		{
			auto physician = physicians.at(index);
			ASSERT_EQ(retrievedPhysician.at(index).name.components.at(PersonName::Group::Alphabetic).givenName,
				physician.name.components.at(PersonName::Group::Alphabetic).givenName);
			ASSERT_EQ(retrievedPhysician.at(index).name.components.at(PersonName::Group::Alphabetic).middleName,
				physician.name.components.at(PersonName::Group::Alphabetic).middleName);
			ASSERT_EQ(retrievedPhysician.at(index).name.components.at(PersonName::Group::Alphabetic).familyName,
				physician.name.components.at(PersonName::Group::Alphabetic).familyName);
		}

		void assertIdeographicName(const std::vector<PhysicianRepository::Physician>& retrievedPhysician, int index)
		{
			auto physician = physicians.at(index);
			ASSERT_EQ(retrievedPhysician.at(index).name.components.at(PersonName::Group::Ideographic).givenName,
				physician.name.components.at(PersonName::Group::Ideographic).givenName);
			ASSERT_EQ(retrievedPhysician.at(index).name.components.at(PersonName::Group::Ideographic).middleName,
				physician.name.components.at(PersonName::Group::Ideographic).middleName);
			ASSERT_EQ(retrievedPhysician.at(index).name.components.at(PersonName::Group::Ideographic).familyName,
				physician.name.components.at(PersonName::Group::Ideographic).familyName);
		}

		void assertPhoneticName(const std::vector<PhysicianRepository::Physician>& retrievedPhysician, int index)
		{
			auto physician = physicians.at(index);
			ASSERT_EQ(retrievedPhysician.at(index).name.components.at(PersonName::Group::Phonetic).givenName,
				physician.name.components.at(PersonName::Group::Phonetic).givenName);
			ASSERT_EQ(retrievedPhysician.at(index).name.components.at(PersonName::Group::Phonetic).middleName,
				physician.name.components.at(PersonName::Group::Phonetic).middleName);
			ASSERT_EQ(retrievedPhysician.at(index).name.components.at(PersonName::Group::Phonetic).familyName,
				physician.name.components.at(PersonName::Group::Phonetic).familyName);
		}
		
		void assertPhysicians(const int physicianCount, PhysicianRepository::Physician::Type type)
		{
			const auto retrievedPhysician = physicianRepository.physicians(type);

			for (int physician = 0; physician < physicianCount; physician++)
			{
				assertAlphabeticName(retrievedPhysician, physician);
				assertIdeographicName(retrievedPhysician, physician);
				assertPhoneticName(retrievedPhysician, physician);
			}

		}

	protected:
		Settings settings;
		PhysicianRepository physicianRepository;
		std::vector<PhysicianRepository::Physician> physicians;
	};

	TEST_P(PhysicianRepositoryTest, Given_Physician_When_Add_ThenVerifySavedPhysician)
	{
		addPhysician(1, GetParam());
		assertPhysicians(1, GetParam());
	}
	
	TEST_P(PhysicianRepositoryTest, Given_TwoPhysicians_When_ComponentsAreSame_Then_PhysiciansAreEqual)
	{
		PhysicianRepository::Physician physician("Key");
		PhysicianRepository::Physician physician1("Key");
		ASSERT_EQ(physician1, physician);
	}

	TEST_P(PhysicianRepositoryTest, Given_TwoPhysicians_When_ComponentsAreDifferent_Then_PhysiciansAreNotEqual)
	{
		PhysicianRepository::Physician physician("Key");
		PhysicianRepository::Physician physician1("Key123");
		PersonName::Component name;
		name.givenName = "AlphabeticGivenName";
		name.familyName = "AlphabeticFamilyName";
		name.middleName = "AlphabeticMiddleName";
		physician.name.components.clear();
		physician.name.components.insert({ PersonName::Group::Alphabetic, name });
		ASSERT_NE(physician1, physician);
	}

	TEST_P(PhysicianRepositoryTest, Given_MoreThanTwentyPhysicians_ThenOnlyTwentyPhysicianSavedToDB)
	{
		addPhysician(21, GetParam());
		ASSERT_EQ(20, physicianRepository.physicians(GetParam()).size());
	}

	TEST_P(PhysicianRepositoryTest, Given_DuplicatePhysicians_ThenDuplicatePhysicianNotSaveToDB)
	{
		addPhysician(2, GetParam());
		physicianRepository.add(physicianRepository.physicians(GetParam()).at(0));
		ASSERT_EQ(2, physicianRepository.physicians(GetParam()).size());
	}

	TEST_P(PhysicianRepositoryTest, Given_TwentyPhysicians_When_NextPhysicianIsAdded_ThenOldPhysicianIsRemoved)
	{
		addPhysician(21, GetParam());

		PersonName::Component removedName;
		removedName.givenName = "AlphabeticGivenName1";
		removedName.familyName = "AlphabeticFamilyName1";
		removedName.middleName = "AlphabeticMiddleName1";
		auto retrievedPhysician = physicianRepository.physicians(GetParam());
		for(auto index = 0; index < retrievedPhysician.size(); index++)
		{
			ASSERT_NE(retrievedPhysician.at(index).name.components.at(PersonName::Group::Alphabetic), removedName);
		}
	}

	TEST_P(PhysicianRepositoryTest, When_DuplicatePhysicianAddedWithDifferentCase_Then_NewEntryReplacesExistingOne)
	{
		addPhysician(3, GetParam());

		PhysicianRepository::Physician physician;
		PersonName::Component caseinsensitiveName;
		caseinsensitiveName.givenName = "ALPHABETICGivenName1";
		caseinsensitiveName.familyName = "AlphabeticFaMilyName1";
		caseinsensitiveName.middleName = "AlphabeticMiddleName1";
		physician.name.components.insert({ PersonName::Group::Alphabetic, caseinsensitiveName });
		physician.type = GetParam();
		physicianRepository.add(physician);
		ASSERT_EQ(3, physicianRepository.physicians(GetParam()).size());
		ASSERT_EQ(physician.name.toString(), physicianRepository.physicians(GetParam()).at(0).name.toString());
	}
	INSTANTIATE_TEST_CASE_P(test, PhysicianRepositoryTest, testing::Values(PhysicianRepository::Physician::Type::Performing, PhysicianRepository::Physician::Type::Operating, PhysicianRepository::Physician::Type::Referring));

}}}