// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include "Ui\notificationdialog.ui.h"

namespace Philips { namespace Workflow { namespace Control {
	
	//! This class provides notification dialog to be used to show the
	//! warning and notification message to the user
	class NotificationDialog : public Ui::NotificationDialog
	{
	public:
		//! Notification details
		struct Info
		{
			std::wstring			title;							//!< Window title displayed in the color corresponding to the notification type
			std::wstring			messageTitle;					//!< Message title displayed in bold
			std::wstring			message;						//!< Message body
			std::wstring			buttonOneText;					//!< Text displayed on the right-most button; button is hidden if empty
			std::function<void()>	buttonOneClickedCallback;		//!< Call-back function to be called when the right-most button is clicked
			std::wstring			buttonTwoText;					//!< Text displayed on the second button from the right; button is hidden if empty
			std::function<void()>	buttonTwoClickedCallback;		//!< Call-back function to be called when the second button from the right is clicked
			std::wstring			buttonThreeText;				//!< Text displayed on the lest-most button; button is hidden if empty
			std::function<void()>	buttonThreeClickedCallback;		//!< Call-back function to be called when the left-most button is clicked
		};

		explicit NotificationDialog(Canvas & parent);
		~NotificationDialog() override = default;

		//! Show a notification to the user
		void showNotice(const Info & info);

		//! Show a warning to the user
		void showWarning(const Info & info);

		//! Hide the notification 
		void hide();

		friend class NotificationDialogPeer;
	private:
		void setInfo(const Info& info);
	};

}}}