// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/control/testpeer/notificationdialogpeer.h"
#include "automateduitest.h"

using namespace Philips::Workflow::Control;

const Color NoticeBarColor = Sense::Color::fromRgb(43, 174, 254);
const Color NoticeTextColor = Sense::Color::fromRgb(255, 255, 255);
const Color WarningBarColor = Sense::Color::fromRgb(253, 149, 31);
const Color WarningTextColor = Sense::Color::fromRgb(153, 66, 41);

	class NotificationDialogTest : public AutomatedUiTest 
	{
	public:
		NotificationDialogTest():AutomatedUiTest(Rect(Point(), Size(1026, 850)))
		{
			view.reset(new NotificationDialog(canvas()));
			view->size = canvas().size;
			peer.reset(new TestPeer::NotificationDialogPeer(*view));
		}

	public:
		std::unique_ptr<NotificationDialog> view;
		std::unique_ptr<TestPeer::NotificationDialogPeer> peer;
	};

	TEST_F(NotificationDialogTest, When_WarningMsgSent_Then_WarningDialogDisplayed)
	{
		std::wstring warningMsg = L"This is a warning Msg";
		const NotificationDialog::Info warningInfo{
			L"Warning",
			L"Invalid Input ", warningMsg,
			L"", nullptr,
			L"", nullptr,
			L"", nullptr };

		view->showWarning(warningInfo);

		ASSERT_EQ(peer->titleBarTextLabelPeer.control.text, L"Warning");
		ASSERT_EQ(peer->messageTitleLabelPeer.control.text, L"Invalid Input ");
		ASSERT_EQ(peer->messageLabelPeer.control.text, warningMsg);
		ASSERT_EQ(peer->titleBarPanelPeer.control.color, WarningBarColor);
		ASSERT_EQ(peer->titleBarTextLabelPeer.control.color, WarningTextColor);
		ASSERT_EQ(view->visible, true);

	}

	TEST_F(NotificationDialogTest, When_NoticeMsgSent_Then_NoticeDialogDisplayed)
	{
		std::wstring noticeMsg = L"This is Notice Msg";
		const NotificationDialog::Info noticeInfo{
			L"Notice",
			L"Notification", noticeMsg,
			L"", nullptr,
			L"", nullptr ,
			L"", nullptr };

		view->showNotice(noticeInfo);

		ASSERT_EQ(peer->titleBarTextLabelPeer.control.text, L"Notice");
		ASSERT_EQ(peer->messageTitleLabelPeer.control.text, L"Notification");
		ASSERT_EQ(peer->messageLabelPeer.control.text, noticeMsg);
		ASSERT_EQ(peer->titleBarPanelPeer.control.color, NoticeBarColor);
		ASSERT_EQ(peer->titleBarTextLabelPeer.control.color, NoticeTextColor);
		ASSERT_EQ(view->visible, true);
	}

	TEST_F(NotificationDialogTest, When_ButtonOneClicked_Then_ButtonOneCallBackCalled)
	{
		bool callBackOneCalled = false;
		std::wstring noticeMsg = L"This is Notice Msg";
		const NotificationDialog::Info noticeInfo{
			L"Notice",
			L"Notification", noticeMsg,
			L"Button1", [&callBackOneCalled]() { callBackOneCalled = true; },
			L"", nullptr,
			L"", nullptr };

		view->showNotice(noticeInfo);
		peer->buttonOnePeer.mouseClick();

		ASSERT_EQ(callBackOneCalled, true);
	}

	TEST_F(NotificationDialogTest, When_ButtonTwoClicked_Then_ButtonTwoCallBackCalled)
	{
		bool callBackTwoCalled = false;
		std::wstring noticeMsg = L"This is Notice Msg";
		const NotificationDialog::Info noticeInfo{
			L"Notice",
			L"Notification", noticeMsg,
			L"", nullptr,
			L"Button2", [&callBackTwoCalled]() { callBackTwoCalled = true; },
			L"", nullptr };

		view->showNotice(noticeInfo);
		peer->buttonTwoPeer.mouseClick();

		ASSERT_EQ(callBackTwoCalled, true);
	}

	TEST_F(NotificationDialogTest, When_ButtonThreeClicked_Then_ButtonThreeCallBackCalled)
	{
		bool callBackThreeCalled = false;
		std::wstring noticeMsg = L"This is Notice Msg";
		const NotificationDialog::Info noticeInfo{
			L"Notice",
			L"Notification", noticeMsg,
			L"", nullptr,
			L"", nullptr,
			L"Button3", [&callBackThreeCalled]() { callBackThreeCalled = true; } };

		view->showNotice(noticeInfo);
		peer->buttonThreePeer.mouseClick();

		ASSERT_EQ(callBackThreeCalled, true);
	}

	TEST_F(NotificationDialogTest, When_HideTheDialog_Then_TheDialogIsHidden)
	{
		std::wstring warningMsg = L"This is a warning Msg";
		const NotificationDialog::Info warningInfo{
			L"Warning",
			L"Invalid Input ", warningMsg,
			L"", nullptr,
			L"", nullptr,
			L"", nullptr };

		view->showWarning(warningInfo);
		view->hide();
		ASSERT_EQ(view->visible, false);
	}

	TEST_F(NotificationDialogTest, When_CallBackIsNull_Then_OnButtonOneClickTheDialogThrowNoException)
	{
		std::wstring warningMsg = L"This is a warning Msg";
		const NotificationDialog::Info warningInfo{
			L"Warning",
			L"Invalid Input ", warningMsg,
			L"Button1", nullptr,
			L"", nullptr,
			L"", nullptr };

		view->showWarning(warningInfo);
		ASSERT_NO_THROW(peer->buttonOnePeer.mouseClick());
	}

	TEST_F(NotificationDialogTest, When_CallBackIsNull_Then_OnButtonTwoClickTheDialogThrowNoException)
	{
		std::wstring warningMsg = L"This is a warning Msg";
		const NotificationDialog::Info warningInfo{
			L"Warning",
			L"Invalid Input ", warningMsg,
			L"", nullptr,
			L"Button2", nullptr,
			L"", nullptr };

		view->showWarning(warningInfo);
		ASSERT_NO_THROW(peer->buttonTwoPeer.mouseClick());
	}

	TEST_F(NotificationDialogTest, When_CallBackIsNull_Then_OnButtonThreeClickTheDialogThrowNoException)
	{
		std::wstring warningMsg = L"This is a warning Msg";
		const NotificationDialog::Info warningInfo{
			L"Warning",
			L"Invalid Input ", warningMsg,
			L"", nullptr,
			L"", nullptr,
			L"Button3", nullptr };

		view->showWarning(warningInfo);
		ASSERT_NO_THROW(peer->buttonThreePeer.mouseClick());
	}