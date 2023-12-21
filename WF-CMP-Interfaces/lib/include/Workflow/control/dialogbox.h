// Copyright Koninklijke Philips N.V., 2020. All Right Reserved
#pragma once

#include "uistyle.h"
#include <Sense.h>
#include <DLS.h>
#include <string>
#include <functional>

namespace Philips { namespace Workflow { namespace Control {

	namespace TestPeer { class DialogBoxPeer; }

	//! A modal dialog that appears at the top of the main content.
	//! It is always placed at the center of the screen in both touch and mouse devices.
	//! It draws a strong dim layer on top of the parent control space.
	class DialogBox : public Sense::Control
	{
		friend class TestPeer::DialogBoxPeer;
	public:
		
		explicit DialogBox(Sense::Control& parent, UiStyle style); //!< Constructor
		~DialogBox();	//!< Destructor

		//! Define the message for the dialog.
		struct Message
		{
			//! Defines message type
			enum class Type { Success = 0, Notice, Warning, Caution, Critical};
			Message();

			Type			type;						//!< Message type

			std::string	header;							//!< Header text
			std::string	text;							//!< Message description

			std::string	accent;							//!< Accent button text
			std::string	primary;						//!< Primary button text
			std::string	secondary;						//!< Secondary button text

			std::function<void()> accentClicked;		//!< Accent button click action
			std::function<void()> primaryClicked;		//!< Primary button click action
			std::function<void()> secondaryClicked;		//!< Secondary button click action
		};
		
		void set(const Message& message);				//!< Set the message on the dialog.

		Sense::Property<bool> show;                   //!< To show or hide the dialog box

	private:
		void onResize() override;
		void render(Sense::IRenderer& renderer) const override;
		void onShowChanged();

	private:
		const UiStyle					_style;
		std::unique_ptr<Sense::Control>	_dialog;
		struct Private;
		std::unique_ptr<Private>		_p;
	};

}}}
