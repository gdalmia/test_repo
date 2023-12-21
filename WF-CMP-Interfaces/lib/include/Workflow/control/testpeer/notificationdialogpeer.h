// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include "Workflow/control/notificationdialog.h"
#include <Automation.h>

//TICS -INT#002: Test peers needs data member in public

namespace Philips { namespace Workflow { namespace Control { namespace TestPeer {
	class NotificationDialogPeer
	{
	public:
		explicit NotificationDialogPeer(NotificationDialog& notificationDialog)
			: dimmerPanelPeer(notificationDialog.dimmerPanel)
			, windowPanelPeer(notificationDialog.windowPanel)
			, titleBarPanelPeer(notificationDialog.titleBarPanel)
			, titleBarTextLabelPeer(notificationDialog.titleBarTextLabel)
			, messageTitleLabelPeer(notificationDialog.messageTitleLabel)
			, messageLabelPeer(notificationDialog.messageLabel)
			, buttonOnePeer(notificationDialog.buttonOne)
			, buttonTwoPeer(notificationDialog.buttonTwo)
			, buttonThreePeer(notificationDialog.buttonThree)
		{
		}

		Sense::Automation::Peer<Panel>             dimmerPanelPeer;
		Sense::Automation::Peer<Panel>             windowPanelPeer;
		Sense::Automation::Peer<Panel>             titleBarPanelPeer;
		Sense::Automation::Peer<Label>             titleBarTextLabelPeer;
		Sense::Automation::Peer<Label>             messageTitleLabelPeer;
		Sense::Automation::Peer<Label>             messageLabelPeer;
		Sense::Automation::Peer<ButtonPrimary>     buttonOnePeer;
		Sense::Automation::Peer<ButtonQuiet>       buttonTwoPeer;
		Sense::Automation::Peer<ButtonQuiet>       buttonThreePeer;
	};

}}}}