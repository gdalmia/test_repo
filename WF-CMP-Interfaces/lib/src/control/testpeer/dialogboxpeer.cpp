// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/control/testpeer/dialogboxpeer.h"
#include "control/dialogboxview.h"

namespace Philips { namespace Workflow { namespace Control { namespace TestPeer {

	using namespace Sense;
	using namespace  Sense::DLS;

	namespace
	{
		template<typename TView>
		TView& view(Sense::Control* control)
		{
			return *dynamic_cast<TView*>(control);
		}
	}

	DialogBoxPeer::DialogBoxPeer(DialogBox & control)
		:
		 _dialogBox(control)
	{
		if (control._style == UiStyle::Integrated)
		{
			auto& viewIntegrated = view<DialogBoxIntegrated>(_dialogBox._dialog.get());
			_header      = std::make_unique<Peer<Label >>(viewIntegrated.message);
			_description = std::make_unique<Peer<Label >>(viewIntegrated.description.text);
			_accent      = std::make_unique<Peer<Button>>(viewIntegrated.ok);
			_primary     = std::make_unique<Peer<Button>>(viewIntegrated.cancel);
			_secondary   = std::make_unique<Peer<Button>>(viewIntegrated.third);
		}
		else
		{
			auto& viewTouch = view<DialogBoxTouch>(_dialogBox._dialog.get());
			_header      = std::make_unique<Peer<Label >>(viewTouch.message);
			_description = std::make_unique<Peer<Label >>(viewTouch.description.text);
			_accent      = std::make_unique<Peer<Button>>(viewTouch.ok);
			_primary     = std::make_unique<Peer<Button>>(viewTouch.cancel);
			_secondary   = std::make_unique<Peer<Button>>(viewTouch.third);
		}
	}

	void DialogBoxPeer::set(const DialogBox::Message & message) const
	{
		_dialogBox.set(message);
	}

	bool DialogBoxPeer::visibility() const
	{
		return _dialogBox.isVisible();
	}

	bool DialogBoxPeer::accentVisible() const
	{
		return _accent->control.isVisible();
	}

	bool DialogBoxPeer::primaryVisible() const
	{
		return _primary->control.isVisible();
	}

	bool DialogBoxPeer::secondaryVisible() const
	{
		return _secondary->control.isVisible();
	}

	DialogBox::Message::Type DialogBoxPeer::type() const
	{
		Gradient colorHeader;
		if (_dialogBox._style == UiStyle::Integrated)
		{
			auto& dialog = view<DialogBoxIntegrated>(_dialogBox._dialog.get());
			colorHeader = dialog.colorHeader;
		}
		else
		{
			auto& dialog = view<DialogBoxTouch>(_dialogBox._dialog.get());
			colorHeader = dialog.colorHeader;
		}
		
		if (colorHeader == Palette::S_Blue60)	return DialogBox::Message::Type::Notice;
		if (colorHeader == Palette::S_Green60)	return DialogBox::Message::Type::Success;
		if (colorHeader == Palette::S_Orange60)	return DialogBox::Message::Type::Warning;
		if (colorHeader == Palette::S_Yellow45)	return DialogBox::Message::Type::Caution;
		if (colorHeader == Palette::S_Red60)	return DialogBox::Message::Type::Critical;

		return DialogBox::Message::Type::Notice;
	}

	std::string DialogBoxPeer::header() const
	{
		return convert(_header->control.text);
	}

	std::string DialogBoxPeer::description() const
	{
		return convert(_description->control.text);
	}

	std::string DialogBoxPeer::accent() const
	{
		return convert(_accent->control.text);
	}

	std::string DialogBoxPeer::primary() const
	{
		return convert(_primary->control.text);
	}

	std::string DialogBoxPeer::secondary() const
	{
		return convert(_secondary->control.text);
	}

	void DialogBoxPeer::clickAccent() const
	{
		if (accentVisible()) _accent->mouseClick();
	}

	void DialogBoxPeer::clickPrimary() const
	{
		if (primaryVisible()) _primary->mouseClick();
	}

	void DialogBoxPeer::clickSecondary() const
	{
		if (secondaryVisible()) _secondary->mouseClick();
	}

}}}}
