// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <gtest/gtest.h>
#include "Workflow/personname.h"

namespace PersonNameTest
{

	using namespace Philips::Workflow;

	typedef std::tuple<PersonName::Component, PersonName::Component, bool> ComponentEqualityParam;
	class Given_TwoPersonNameComponents : public testing::TestWithParam<ComponentEqualityParam>
	{
	};

	INSTANTIATE_TEST_SUITE_P(When_Compared, Given_TwoPersonNameComponents, ::testing::Values(
		ComponentEqualityParam{ PersonName::Component{ "","","","","" }		, PersonName::Component{ "","","","","" } ,				true }
		, ComponentEqualityParam{ PersonName::Component{ "F","","","","" }		, PersonName::Component{ "","","","","" } ,				false }
		, ComponentEqualityParam{ PersonName::Component{ "F","","","","" }		, PersonName::Component{ "F","","","","" } ,			true }
		, ComponentEqualityParam{ PersonName::Component{ "F","","","","" }		, PersonName::Component{ "F1","","","","" } ,			false }
		, ComponentEqualityParam{ PersonName::Component{ "F","G","","","" }		, PersonName::Component{ "F","","","","" } ,			false }
		, ComponentEqualityParam{ PersonName::Component{ "F","G","","","" }		, PersonName::Component{ "F","G","","","" } ,			true }
		, ComponentEqualityParam{ PersonName::Component{ "F","G","","","" }		, PersonName::Component{ "F","G1","","","" } ,			false }
		, ComponentEqualityParam{ PersonName::Component{ "F","G","M","","" }	, PersonName::Component{ "F","G","","","" } ,			false }
		, ComponentEqualityParam{ PersonName::Component{ "F","G","M","","" }	, PersonName::Component{ "F","G","M","","" } ,			true }
		, ComponentEqualityParam{ PersonName::Component{ "F","G","M","","" }	, PersonName::Component{ "F","G","M1","","" } ,			false }
		, ComponentEqualityParam{ PersonName::Component{ "F","G","M","P","" }	, PersonName::Component{ "F","G","M","","" } ,			false }
		, ComponentEqualityParam{ PersonName::Component{ "F","G","M","P","" }	, PersonName::Component{ "F","G","M","P","" } ,			true }
		, ComponentEqualityParam{ PersonName::Component{ "F","G","M","P","" }	, PersonName::Component{ "F","G","M","P1","" } ,		false }
		, ComponentEqualityParam{ PersonName::Component{ "F","G","M","P","S" }	, PersonName::Component{ "F","G","M","P","" } ,			false }
		, ComponentEqualityParam{ PersonName::Component{ "F","G","M","P","S" }	, PersonName::Component{ "F","G","M","P","S" } ,		true }
		, ComponentEqualityParam{ PersonName::Component{ "F","G","M","P","S" }	, PersonName::Component{ "F","G","M","P","S1" } ,		false }
	));

	TEST_P(Given_TwoPersonNameComponents, Then_EqualityResultIsEqualToSpecified)
	{
		const auto params = GetParam();
		const auto& component1 = std::get<0>(params);
		const auto& component2 = std::get<1>(params);
		const auto equal = std::get<2>(params);
		ASSERT_EQ(component1 == component2, equal);
	}

	TEST_P(Given_TwoPersonNameComponents, Then_NotEqualityResultIsNotEqualToSpecified)
	{
		const auto params = GetParam();
		const auto& component1 = std::get<0>(params);
		const auto& component2 = std::get<1>(params);
		const auto equal = std::get<2>(params);
		ASSERT_NE(component1 != component2, equal);
	}

	typedef std::tuple<PersonName, PersonName, bool> PersonNameEqualityParam;
	class Given_TwoPersonNames : public testing::TestWithParam<PersonNameEqualityParam>
	{
	};

	INSTANTIATE_TEST_SUITE_P(When_Compared, Given_TwoPersonNames, ::testing::Values(
		PersonNameEqualityParam
		{
			{{
				{ PersonName::Group::Alphabetic,	PersonName::Component{ "","","","","" } }
			,	{ PersonName::Group::Phonetic,		PersonName::Component{ "","","","","" } }
			,	{ PersonName::Group::Ideographic,	PersonName::Component{ "","","","","" } }
			}}
		,	{{
				{ PersonName::Group::Alphabetic,	PersonName::Component{ "","","","","" } }
			,	{ PersonName::Group::Phonetic,		PersonName::Component{ "","","","","" } }
			,	{ PersonName::Group::Ideographic,	PersonName::Component{ "","","","","" } }
			}}
		,	true
		}
		, PersonNameEqualityParam
		{
			{{
				{ PersonName::Group::Alphabetic,	PersonName::Component{ "A11","A12","A13","A14","A15" } }
			,	{ PersonName::Group::Phonetic,		PersonName::Component{ "P11","P12","P13","P14","P15" } }
			,	{ PersonName::Group::Ideographic,	PersonName::Component{ "I11","I12","I13","I14","I15" } }
			}}
		,	{{
				{ PersonName::Group::Alphabetic,	PersonName::Component{ "A11","A12","A13","A14","A15" } }
			,	{ PersonName::Group::Phonetic,		PersonName::Component{ "P11","P12","P13","P14","P15" } }
			,	{ PersonName::Group::Ideographic,	PersonName::Component{ "I11","I12","I13","I14","I15" } }
			}}
		,	true
		}
		, PersonNameEqualityParam
		{
			{{
				{ PersonName::Group::Alphabetic,	PersonName::Component{ "A21","A22","A23","A24","A25" } }
			,	{ PersonName::Group::Phonetic,		PersonName::Component{ "P21","P22","P23","P24","P25" } }
			,	{ PersonName::Group::Ideographic,	PersonName::Component{ "I21","I22","I23","I24","I25" } }
			}}
		,	{{
				{ PersonName::Group::Alphabetic,	PersonName::Component{ "B21","B22","B23","B24","B25" } }
			,	{ PersonName::Group::Phonetic,		PersonName::Component{ "P21","P22","P23","P24","P25" } }
			,	{ PersonName::Group::Ideographic,	PersonName::Component{ "I21","I22","I23","I24","I25" } }
			}}
		,	false
		}
		, PersonNameEqualityParam
		{
			{{
				{ PersonName::Group::Alphabetic,	PersonName::Component{ "A31","A32","A33","A34","A35" } }
			,	{ PersonName::Group::Phonetic,		PersonName::Component{ "P31","P32","P33","P34","P35" } }
			,	{ PersonName::Group::Ideographic,	PersonName::Component{ "I31","I32","I33","I34","I35" } }
			}}
		,	{{
				{ PersonName::Group::Alphabetic,	PersonName::Component{ "A31","A32","A33","A34","A35" } }
			,	{ PersonName::Group::Phonetic,		PersonName::Component{ "Q31","Q32","Q33","Q34","Q35" } }
			,	{ PersonName::Group::Ideographic,	PersonName::Component{ "I31","I32","I33","I34","I35" } }
			}}
		,	false
		}
		, PersonNameEqualityParam
		{
			{{
				{ PersonName::Group::Alphabetic,	PersonName::Component{ "A41","A42","A43","A44","A45" } }
			,	{ PersonName::Group::Phonetic,		PersonName::Component{ "P41","P42","P43","P44","P45" } }
			,	{ PersonName::Group::Ideographic,	PersonName::Component{ "I41","I42","I43","I44","I45" } }
			}}
		,	{{
				{ PersonName::Group::Alphabetic,	PersonName::Component{ "A41","A42","A43","A44","A45" } }
			,	{ PersonName::Group::Phonetic,		PersonName::Component{ "P41","P42","P43","P44","P45" } }
			,	{ PersonName::Group::Ideographic,	PersonName::Component{ "J41","J42","J43","J44","J45" } }
			}}
		,	false
		}
	));

	TEST_P(Given_TwoPersonNames, Then_EqualityResultIsEqualToSpecified)
	{
		const auto params = GetParam();
		const auto& name1 = std::get<0>(params);
		const auto& name2 = std::get<1>(params);
		const auto equal = std::get<2>(params);
		ASSERT_EQ(name1 == name2, equal);
	}

	TEST_P(Given_TwoPersonNames, Then_NotEqualityResultIsNotEqualToSpecified)
	{
		const auto params = GetParam();
		const auto& name1 = std::get<0>(params);
		const auto& name2 = std::get<1>(params);
		const auto equal = std::get<2>(params);
		ASSERT_NE(name1 != name2, equal);
	}

	typedef std::tuple<PersonName::Component, std::string> PersonNameFormat;
	class Given_PersonName : public testing::TestWithParam<PersonNameFormat>
	{
	};

	INSTANTIATE_TEST_SUITE_P(When_PersonNameCastToString, Given_PersonName, ::testing::Values(
			PersonNameFormat{ PersonName::Component{ "","","","","" }, "" }
		,	PersonNameFormat{ PersonName::Component{ "fn","","","","" }, "fn" }
		,	PersonNameFormat{ PersonName::Component{ "fn","gn","","","" }, "gn fn" }
		,	PersonNameFormat{ PersonName::Component{ "fn","gn","mn","","" }, "gn mn fn" }
		,	PersonNameFormat{ PersonName::Component{ "fn","gn","mn","pre","" }, "pre gn mn fn" }
		,	PersonNameFormat{ PersonName::Component{ "fn","gn","mn","pre","suf" }, "pre gn mn fn suf" }
		,	PersonNameFormat{ PersonName::Component{ "","gn","","","" }, "gn" }
		,	PersonNameFormat{ PersonName::Component{ "","","mn","","" }, "mn" }
		,	PersonNameFormat{ PersonName::Component{ "","","","pre","" }, "pre" }
		,	PersonNameFormat{ PersonName::Component{ "","","","suf","" }, "suf" }
	));

	TEST_P(Given_PersonName, Then_EqualityResultIsEqualToSpecified)
	{
		const auto params = GetParam();
		const auto& expected = std::get<1>(params);
		PersonName name;
		name.components.emplace(PersonName::Group::Alphabetic, std::get<0>(params));
		ASSERT_EQ(name.toString(), expected);
	}

	class PersonNameTest : public testing::Test
	{
		
	};

	TEST_F(PersonNameTest, Given_NoComponent_When_ToStringCalled_Then_ReturnEmptyString)
	{
		PersonName name;
		ASSERT_EQ(name.toString(), "");
	}

	TEST_F(PersonNameTest, Given_NoAlphabeticGroup_When_ToStringCalled_Then_ReturnEmptyString)
	{
		PersonName name;
		name.components.emplace(PersonName::Group::Ideographic, PersonName::Component{ "fn","gn","","","" });
		ASSERT_EQ(name.toString(), "");
	}

}