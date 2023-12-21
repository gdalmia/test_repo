// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "dialogboxview.h"
#include "Workflow/binding.h"
#include <Sense.h>
#include <algorithm>

namespace Philips { namespace Workflow { namespace Control {

	using namespace Binding;
	using namespace Sense;
	using namespace Sense::DLS;

	const double defaultButtonWidthIntegrated = 160;
	const double defaultButtonWidthTouch      = 200;

	template<class T>
	DialogBoxView<T>::DialogBoxView(Sense::Control & parent, DialogBoxViewModel & model, const UiStyle style)
		: T(parent)
		, _model(model)
		, _style(style)
		, _animation(this->scheduler, DialogBoxView::AnimationState::FadeOut, [this]() { animate(); })
	{
		bind();
		subscribe();
	}

	template <class T>
	void DialogBoxView<T>::bind()
	{
		bind_text(this->message, _model.header, StringToWstring());
		bind_text(this->ok, _model.accent, StringToWstring());
		bind_text(this->cancel, _model.primary, StringToWstring());
		bind_text(this->third, _model.secondary, StringToWstring());
		bind_visible(this->ok, _model.accentVisible);
		bind_visible(this->cancel, _model.primaryVisible);
		bind_visible(this->third, _model.secondaryVisible);
		
		Binding::bind_command(this->ok, _model.accentClicked);
		Binding::bind_command(this->cancel, _model.primaryClicked);
		Binding::bind_command(this->third, _model.secondaryClicked);
	}

	template <class T>
	void DialogBoxView<T>::subscribe()
	{
		_model.showDialog.changed += [this](bool value)
		{
			onShowDialogChanged(value);
		};

		_animation.eventStarted = [this]() {
			if (_animation.getState() == DialogBoxView::AnimationState::FadeIn)
			{
				this->visible = true;
			}
		};

		_animation.eventStopped = [this]() {
			if (_animation.getState() == DialogBoxView::AnimationState::FadeOut)
			{
				this->visible = false;
			}

		};
	}

	template <class T>
	void DialogBoxView<T>::onShowDialogChanged(bool show)
	{
		show ? updateDialog() : stopAnimation();
	}

	template <class T>
	void DialogBoxView<T>::updateDialog()
	{
		const auto defaultWidth = _style == UiStyle::Integrated ? defaultButtonWidthIntegrated : defaultButtonWidthTouch;
		
		this->ok.size     = Size(defaultWidth, this->ok.size->height);
		this->cancel.size = Size(defaultWidth, this->cancel.size->height);
		this->third.size  = Size(defaultWidth, this->third.size->height);

		this->message.visible     = true;
		this->bodySpacer.visible  = true;
		this->description.visible = true;

		updateDescription();
		updateButtonSize(defaultWidth);
		updateHeaderColor();

		if(_model.accent.value().empty() && _model.primary.value().empty() && _model.secondary.value().empty())
		{
			this->footer.visible = false;
			this->size = Size(this->size->width, this->size->height - this->footer.size->height);
		}
		else
		{
			this->footer.visible = true;
		}

		startAnimation();
	}

	template <class T>
	void DialogBoxView<T>::updateButtonSize(const double buttonWidth)
	{
		if (this->ok.measureSize().width > buttonWidth)
		{
			this->ok.size = Size(this->ok.measureSize().width, this->ok.size->height);
		}

		if (this->cancel.measureSize().width > buttonWidth)
		{
			this->cancel.size = Size(this->cancel.measureSize().width, this->cancel.size->height);
		}

		if (this->third.measureSize().width > buttonWidth)
		{
			this->third.size = Size(this->third.measureSize().width, this->third.size->height);
		}
	}

	template <class T>
	void DialogBoxView<T>::updateHeaderColor()
	{
		switch (_model.type.value())
		{
		case DialogBox::Message::Type::Notice:	    this->colorHeader = Palette::S_Blue60  ;    break;
		case DialogBox::Message::Type::Success:     this->colorHeader = Palette::S_Green60 ;    break;
		case DialogBox::Message::Type::Caution:     this->colorHeader = Palette::S_Yellow45;    break;
		case DialogBox::Message::Type::Warning:	    this->colorHeader = Palette::S_Orange60;    break;
		case DialogBox::Message::Type::Critical:	this->colorHeader = Palette::S_Red60   ;    break;
		}
	}

	template <>
	void DialogBoxView<Ui::DialogBoxIntegrated>::updateDescription()
	{
		this->description.text.size  = Size(720, 640);
		this->description.text.text  = convert(_model.text.value());
		this->description.text.color = Color(1.0) * 0.80;
		
		this->description.text.font = Font(L"CentraleSansBook", 16, false, false, false, false, Font::Antialias::High);
		this->footerSpacer.size         = Size(800, 32);
		double descriptionFullHeight    = this->description.text.measureTextSize().height + 8.0;
		double descriptionVisibleHeight = std::min(descriptionFullHeight, 640.0);
		double dialogHeight             = 168 + descriptionVisibleHeight;

		if (_model.text.value().empty())
		{
			this->message.visible        = false;                  
			this->bodySpacer.visible     = false;               
			this->description.position   = this->message.position;
			this->description.text.text  = convert(_model.header.value());
			this->description.text.color = DLS::Palette::White;
			
			this->description.text.font = Font(L"CentraleSansBold", 20, false, false, false, false, Font::Antialias::High);
			
			this->footerSpacer.size  = Size(800, 32);
			descriptionFullHeight    = this->description.text.measureTextSize().height;
			descriptionVisibleHeight = std::min(descriptionFullHeight, 640.0);
			dialogHeight             = descriptionVisibleHeight + 124.0;
		}

		this->description.size            = Size(760, descriptionVisibleHeight);
		this->description.scrollArea.size = Size(760, descriptionVisibleHeight);
		this->description.scrollArea.area = Size(760, descriptionFullHeight);
		this->description.text.size       = Size(720, descriptionFullHeight);
		this->size                        = Size(800, dialogHeight);
	}

	template <class T>
	void DialogBoxView<T>::startAnimation()
	{
		_animation.set(DialogBoxView::AnimationState::FadeOut);

		if (this->getCanvas().isAnimated())
			_animation.start(DialogBoxView::AnimationState::FadeIn, 0.2);
		else
			_animation.set(DialogBoxView::AnimationState::FadeIn);
	}

	template <class T>
	void DialogBoxView<T>::stopAnimation()
	{
		_animation.set(DialogBoxView::AnimationState::FadeIn);

		if (this->getCanvas().isAnimated())
			_animation.start(DialogBoxView::AnimationState::FadeOut, 0.1);
		else
			_animation.set(DialogBoxView::AnimationState::FadeOut);
	}

	template <class T>
	void DialogBoxView<T>::animate()
	{
		static const double opacity[2] = { 0.0, 1.0 };
		this->opacityBuffer.color = _animation.get(opacity);
	}

	template <>
	void DialogBoxView<Ui::DialogBoxTouch>::updateDescription()
	{

		const auto bodyMaximumHeight = _style == UiStyle::Cart ? 531.0 : 363.0 ;
		
		this->description.text.size  = Size(880, bodyMaximumHeight); 
		this->description.text.text  = convert(_model.text.value());
		this->description.text.color = Color(1.0) * 0.80;
		this->description.text.font = Font(L"CentraleSansBook", 24, false, false, false, false, Font::Antialias::High);
		this->footerSpacer.size = Size(960, 40);
		double descriptionFullHeight = this->description.text.measureTextSize().height + 8.0;
		double descriptionVisibleHeight = std::min(descriptionFullHeight, bodyMaximumHeight);
		double dialogHeight = 240 + descriptionVisibleHeight;

		if (_model.text.value().empty())
		{
			this->message.visible        = false;                  
			this->bodySpacer.visible     = false;                  
			this->description.position   = this->message.position; 
			this->description.text.text  = convert(_model.header.value());
			this->description.text.color = DLS::Palette::White;
			
			this->description.text.font = Font(L"CentraleSansBold", 32, false, false, false, false, Font::Antialias::High);
			this->footerSpacer.size  = Size(960, 40);
			descriptionFullHeight    = this->description.text.measureTextSize().height;
			descriptionVisibleHeight = std::min(descriptionFullHeight, bodyMaximumHeight);
			dialogHeight             = descriptionVisibleHeight + 168.0;
		}

		this->description.size            = Size(920, descriptionVisibleHeight);
		this->description.scrollArea.size = Size(920, descriptionVisibleHeight);
		this->description.scrollArea.area = Size(920, descriptionFullHeight);
		this->description.text.size       = Size(880, descriptionFullHeight);
		this->size                        = Size(960, dialogHeight);
		
	}

	template class DialogBoxView<Ui::DialogBoxIntegrated>;
	template class DialogBoxView<Ui::DialogBoxTouch>;

}}}
