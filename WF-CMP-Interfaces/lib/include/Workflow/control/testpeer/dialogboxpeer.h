// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once

#include "Workflow/control/dialogbox.h"
#include <Automation.h>

namespace Philips { namespace Workflow { namespace Control { namespace TestPeer {

	using namespace Sense;
	
	class DialogBoxPeer 
	{
	public:
		explicit DialogBoxPeer(DialogBox &control);
		~DialogBoxPeer() = default;

		void set(const DialogBox::Message& message) const;

		bool			visibility()		const;
		bool			accentVisible()		const;
		bool			primaryVisible()	const;
		bool			secondaryVisible()	const;

		DialogBox::Message::Type	type()	const;

		std::string		header()			const;
		std::string		description()		const;
		std::string		accent()			const;
		std::string		primary()			const;
		std::string		secondary()			const;

		void			clickAccent()		const;
		void			clickPrimary()		const;
		void			clickSecondary()	const;
		
	private:
		DialogBox& _dialogBox;

		std::unique_ptr<Peer<DLS::Label>>	_header;
		std::unique_ptr<Peer<DLS::Label>>	_description;
		std::unique_ptr<Peer<DLS::Button>>	_accent;
		std::unique_ptr<Peer<DLS::Button>>	_primary;
		std::unique_ptr<Peer<DLS::Button>>	_secondary;
	};

}}}}
