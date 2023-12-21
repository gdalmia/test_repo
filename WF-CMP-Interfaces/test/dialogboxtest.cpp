// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/control/testpeer/dialogboxpeer.h"
#include "automateduitest.h"
#include <gtest/gtest.h>

using namespace Philips::Workflow::Control;


class DialogBoxTestBase : public AutomatedUiTest
{

public:
	DialogBoxTestBase() : AutomatedUiTest(Sense::Rect(Sense::Point(), Sense::Size(960, 810)))
		, dialog(nullptr)
		, peer(nullptr)
	{
	}

	std::unique_ptr<DialogBox>                 dialog;					//TICS !INT#002 Made public for testing purpose
	std::unique_ptr<TestPeer::DialogBoxPeer>   peer;		//TICS !INT#002 Made public for testing purpose
};

class DialogBoxTest : public DialogBoxTestBase, public testing::WithParamInterface<UiStyle>
{
public:
	DialogBoxTest()
	{
		dialog = std::make_unique<DialogBox>(canvas(), GetParam());
		peer = std::make_unique<TestPeer::DialogBoxPeer>(*dialog);
	}
};

TEST_P(DialogBoxTest, Given_EmptyMessage_When_DialogIsShown_Then_CheckDialogState)
{
	const DialogBox::Message message;
	dialog->set(message);
	dialog->show = true;
	ASSERT_EQ(peer->header(), message.header);
	ASSERT_EQ(peer->description(), message.text);
	ASSERT_EQ(peer->accent(), message.accent);
	ASSERT_EQ(peer->primary(), message.primary);
	ASSERT_EQ(peer->secondary(), message.secondary);
	ASSERT_FALSE(peer->accentVisible());
	ASSERT_FALSE(peer->primaryVisible());
	ASSERT_FALSE(peer->secondaryVisible());
	ASSERT_TRUE(peer->visibility());
}

TEST_P(DialogBoxTest, Given_MessageWithHeader_When_DialogIsShown_Then_DialogHeaderIsSet)
{
	DialogBox::Message message;
	message.header = "Header";
	dialog->set(message);
	dialog->show = true;
	ASSERT_EQ(peer->header(), message.header);
}

TEST_P(DialogBoxTest, Given_MessageWithText_When_SetInDialog_Then_DescriptionIsSet)
{
	DialogBox::Message message;
	message.text = "Text";
	dialog->set(message);
	dialog->show = true;
	ASSERT_EQ(peer->description(), message.text);
}

TEST_P(DialogBoxTest, Given_MessageWithAccentButtonText_When_SetInDialog_Then_AccentButtonIsVisible)
{
	DialogBox::Message message;
	message.accent = "Ok";
	dialog->set(message);
	dialog->show = true;
	ASSERT_TRUE(peer->accentVisible());
	ASSERT_EQ(peer->accent(), message.accent);
}

TEST_P(DialogBoxTest, Given_MessageWithEmptyAccentButtonText_When_SetInDialog_Then_AccentButtonIsNotVisible)
{
	DialogBox::Message message;
	message.accent = "";
	dialog->set(message);
	dialog->show = true;
	ASSERT_FALSE(peer->accentVisible());
	ASSERT_EQ(peer->accent(), message.accent);
}

TEST_P(DialogBoxTest, Given_MessageWithPrimaryButtonText_When_SetInDialog_Then_PrimaryButtonIsVisible)
{
	DialogBox::Message message;
	message.primary = "Cancel";
	dialog->set(message);
	dialog->show = true;
	ASSERT_TRUE(peer->primaryVisible());
	ASSERT_EQ(peer->primary(), message.primary);
}

TEST_P(DialogBoxTest, Given_MessageWithEmptyPrimaryButtonText_When_SetInDialog_Then_PrimaryButtonIsNotVisible)
{
	DialogBox::Message message;
	message.primary = "";
	dialog->set(message);
	dialog->show = true;
	ASSERT_FALSE(peer->primaryVisible());
}

TEST_P(DialogBoxTest, Given_MessageWithSecondaryButtonText_When_SetInDialog_Then_SecondaryButtonIsVisible)
{
	DialogBox::Message message;
	message.secondary = "Close";
	dialog->set(message);
	dialog->show = true;
	ASSERT_TRUE(peer->secondaryVisible());
	ASSERT_EQ(peer->secondary(), message.secondary);
}

TEST_P(DialogBoxTest, Given_MessageWithEmptySecondaryButtonText_When_SetInDialog_Then_SecondaryButtonIsNotVisible)
{
	DialogBox::Message message;
	message.secondary = "";
	dialog->set(message);
	dialog->show = true;

	ASSERT_FALSE(peer->secondaryVisible());
}

TEST_P(DialogBoxTest, Given_MessageWithAccentAction_When_AccentButtonIsClicked_Then_AccentClickedEventIsRaised)
{
	DialogBox::Message message;
	message.accent = "Ok";
	auto clicked = false;
	message.accentClicked = [&clicked] { clicked = true; };
	peer->set(message);
	dialog->show = true;
	peer->clickAccent();

	ASSERT_TRUE(clicked);
}

TEST_P(DialogBoxTest, Given_MessageWithPrimaryAction_When_PrimaryButtonIsClicked_Then_PrimaryClickedEventIsRaised)
{
	DialogBox::Message message;
	message.primary = "Cancel";
	auto clicked = false;
	message.primaryClicked = [&clicked] { clicked = true; };
	peer->set(message);
	dialog->show = true;
	peer->clickPrimary();

	ASSERT_TRUE(clicked);
}

TEST_P(DialogBoxTest, Given_MessageWithSecondaryAction_When_SecondaryButtonIsClicked_Then_SecondaryClickedEventIsRaised)
{
	DialogBox::Message message;
	message.secondary = "Close";
	auto clicked = false;
	message.secondaryClicked = [&clicked] { clicked = true; };
	peer->set(message);
	dialog->show = true;
	peer->clickSecondary();

	ASSERT_TRUE(clicked);
}

TEST_P(DialogBoxTest, Given_MessageAndShowIsCalled_When_HideIsCalled_Then_DialogIsNotShown)
{
	const DialogBox::Message message;
	peer->set(message);
	dialog->show = true;

	dialog->show = false;

	ASSERT_FALSE(peer->visibility());
}

TEST_P(DialogBoxTest, Given_EmptyMessageAndShowIsCalled_When_NewMessageIsSet_Then_DialogIsUpdated)
{
	const DialogBox::Message message;
	peer->set(message);
	dialog->show = true;

	DialogBox::Message newMessage;
	newMessage.type = DialogBox::Message::Type::Success;
	newMessage.header = "dialog header";
	newMessage.text = "dialog text";
	newMessage.accent = "save";
	newMessage.primary = "Discard";
	newMessage.secondary = "cancel";

	peer->set(newMessage);

	ASSERT_EQ(peer->header()     , newMessage.header);
	ASSERT_EQ(peer->description(), newMessage.text);
	ASSERT_EQ(peer->accent()     , newMessage.accent);
	ASSERT_EQ(peer->primary()    , newMessage.primary);
	ASSERT_EQ(peer->secondary()  , newMessage.secondary);
	ASSERT_TRUE(peer->accentVisible());
	ASSERT_TRUE(peer->primaryVisible());
	ASSERT_TRUE(peer->secondaryVisible());
	ASSERT_TRUE(peer->visibility());
}

TEST_P(DialogBoxTest, Given_EmptyMessageIsShown_When_MessageWithAccentActionIsSetAndAccentButtonIsClicked_Then_AccentClickedEventIsRaised)
{
	const DialogBox::Message message;
	peer->set(message);
	dialog->show = true;
	
	DialogBox::Message newMessage;
	newMessage.accent = "Ok";
	auto clicked = false;
	newMessage.accentClicked = [&clicked] { clicked = true; };
	peer->set(newMessage);
	peer->clickAccent();

	ASSERT_TRUE(clicked);
}

INSTANTIATE_TEST_CASE_P(Given_Dialog, DialogBoxTest, testing::Values(UiStyle::Integrated, UiStyle::Cart, UiStyle::Tsm));


typedef std::pair<UiStyle, DialogBox::Message::Type> TestParams;
class DialogBoxTypeTest : public DialogBoxTestBase, public testing::WithParamInterface<TestParams>
{
public:
	DialogBoxTypeTest()
	{
		dialog = std::make_unique<DialogBox>(canvas(), GetParam().first);
		peer = std::make_unique<TestPeer::DialogBoxPeer>(*dialog);
	}
};

INSTANTIATE_TEST_SUITE_P(Given_DialogBox, DialogBoxTypeTest, ::testing::Values(
	TestParams{ UiStyle::Integrated, DialogBox::Message::Type::Notice },
	TestParams{ UiStyle::Integrated, DialogBox::Message::Type::Warning },
	TestParams{ UiStyle::Integrated, DialogBox::Message::Type::Success },
	TestParams{ UiStyle::Integrated, DialogBox::Message::Type::Critical },
	TestParams{ UiStyle::Integrated, DialogBox::Message::Type::Caution },
	TestParams{ UiStyle::Cart, DialogBox::Message::Type::Notice },
	TestParams{ UiStyle::Cart, DialogBox::Message::Type::Warning },
	TestParams{ UiStyle::Cart, DialogBox::Message::Type::Success },
	TestParams{ UiStyle::Cart, DialogBox::Message::Type::Critical },
	TestParams{ UiStyle::Cart, DialogBox::Message::Type::Caution },
	TestParams{ UiStyle::Tsm, DialogBox::Message::Type::Notice },
	TestParams{ UiStyle::Tsm, DialogBox::Message::Type::Warning },
	TestParams{ UiStyle::Tsm, DialogBox::Message::Type::Success },
	TestParams{ UiStyle::Tsm, DialogBox::Message::Type::Critical },
	TestParams{ UiStyle::Tsm, DialogBox::Message::Type::Caution }));

TEST_P(DialogBoxTypeTest, Given_MessageType_When_SetInDialog_Then_SameTypeDialogShown)
{
	DialogBox::Message message;
	message.type = GetParam().second;
	dialog->set(message);
	dialog->show = true;
	ASSERT_EQ(peer->type(), message.type);
}