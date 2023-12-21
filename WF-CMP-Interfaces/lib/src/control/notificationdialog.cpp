// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow\control\notificationdialog.h"

namespace Philips { namespace Workflow { namespace Control {

	const Color NoticeBarColor = Sense::Color::fromRgb(43, 174, 254);
	const Color NoticeTextColor = Sense::Color::fromRgb(255, 255, 255);
	const Color WarningBarColor = Sense::Color::fromRgb(253, 149, 31);
	const Color WarningTextColor = Sense::Color::fromRgb(153, 66, 41);

	NotificationDialog::NotificationDialog(Canvas & parent)
		: Ui::NotificationDialog(parent)
	{
		modal = true;
	}

	void NotificationDialog::showNotice(const Info& info)
	{
		titleBarPanel.color = NoticeBarColor;
		titleBarTextLabel.color = NoticeTextColor;
		setInfo(info);
	}

	void NotificationDialog::showWarning(const Info& info)
	{
		titleBarPanel.color = WarningBarColor;
		titleBarTextLabel.color = WarningTextColor;
		setInfo(info);
	}

	void NotificationDialog::hide()
	{
		visible = false;
	}

	void NotificationDialog::setInfo(const Info& info)
	{

		titleBarTextLabel.text	= info.title;

		messageTitleLabel.text	= info.messageTitle;
		messageLabel.text		= info.message;

		buttonOne.text			= info.buttonOneText;
		buttonOne.eventClicked	= info.buttonOneClickedCallback;

		buttonTwo.text			= info.buttonTwoText;
		buttonTwo.visible		= (!info.buttonTwoText.empty());
		buttonTwo.eventClicked	= info.buttonTwoClickedCallback;

		buttonThree.text		 = info.buttonThreeText;
		buttonThree.visible		 = (!info.buttonThreeText.empty());
		buttonThree.eventClicked = info.buttonThreeClickedCallback;

		visible = true;
		bringToFront();
	}

}}}