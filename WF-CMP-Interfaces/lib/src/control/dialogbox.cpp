// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include <Workflow/control/dialogbox.h>
#include "dialogboxviewmodel.h"
#include "dialogboxview.h"

namespace Philips { namespace Workflow { namespace Control {

	using namespace Sense;
	using namespace Sense::DLS;

	struct DialogBox::Private
	{
		DialogBoxViewModel viewModel;
	};

	DialogBox::Message::Message()
		: type(Type::Notice)
		, header("")
		, text("")
		, accent("")
		, primary("")
		, secondary("")
		, accentClicked(nullptr)
		, primaryClicked(nullptr)
		, secondaryClicked(nullptr)
	{
	}

	DialogBox::DialogBox(Sense::Control& parent, UiStyle style)
		: Control(parent)
		, show(false, [this]() { onShowChanged(); })
		, _style(style)
		, _p(std::make_unique<Private>())
	{
		modal = true;
		visible = false;

		if (style == UiStyle::Integrated)
		{
			_dialog = std::make_unique<DialogBoxIntegrated>(*this, _p->viewModel, style);
		}
		else
		{
			_dialog = std::make_unique<DialogBoxTouch>(*this, _p->viewModel, style);
		}
		
		this->size = parent.size;
		DialogBox::onResize();
	}

	DialogBox::~DialogBox()
	{
		_dialog.reset();
		_p.reset();
	}

	void DialogBox::set(const Message& message)
	{
		_p->viewModel.header = message.header;
		_p->viewModel.text = message.text;
		_p->viewModel.accent = message.accent;
		_p->viewModel.primary = message.primary;
		_p->viewModel.accentVisible = !message.accent.empty();
		_p->viewModel.primary = message.primary;
		_p->viewModel.primaryVisible = !message.primary.empty();
		_p->viewModel.secondary = message.secondary;
		_p->viewModel.secondaryVisible = !message.secondary.empty();
		_p->viewModel.type = message.type;

		_p->viewModel.accentClicked.execute = message.accentClicked;
		_p->viewModel.primaryClicked.execute = message.primaryClicked;
		_p->viewModel.secondaryClicked.execute = message.secondaryClicked;

		if (show)
			onShowChanged();
		onResize();		
	}

	void DialogBox::onResize()
	{
		size = getParentSpace().bounds.size();
		Control::onResize();
		const Space space = getSpace();

		// By default, the dialog is positioned to the center of the parent control
		Point pos = space.bounds.center() - Point(_dialog->size->width, _dialog->size->height) / 2.0;
		_dialog->position = pos;
		bringToFront();
		_dialog->bringToFront();
	}

	void DialogBox::render(IRenderer &renderer) const
	{
		const Color backgroundColor = Color::fromRgb(5, 5, 5)*0.8;

		//render a semi-transparent black background
		const Space space = getSpace();
		renderer.render(space, space.bounds, backgroundColor);

		Control::render(renderer);
	}

	void DialogBox::onShowChanged()
	{
		if (show)
		{
			// To show the dialog animation, parent control should be visible
			this->visible = true;
			_p->viewModel.showDialog.update(true);
		}
		else
		{
			_p->viewModel.showDialog = false;
			this->visible = false;
		}
	}
}}}
