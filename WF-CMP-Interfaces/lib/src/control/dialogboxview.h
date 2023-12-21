// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#pragma once
#include "Ui\dialogboxintegrated.ui.h"
#include "Ui\dialogboxtouch.ui.h"
#include "dialogboxviewmodel.h"

namespace Philips { namespace Workflow { namespace Control {

	template <class T>
	class DialogBoxView : public T
	{
	public:
		DialogBoxView(Sense::Control &parent, DialogBoxViewModel &model, const UiStyle style);
		~DialogBoxView() = default;

	private:

		enum class AnimationState
		{
			FadeOut = 0,
			FadeIn
		};
		
		void bind();
		void subscribe();
		void onShowDialogChanged(bool show);
		void updateDialog();
		void updateButtonSize(const double buttonWidth);
		void updateHeaderColor();
		void updateDescription();
		void startAnimation();
		void stopAnimation();
		void animate();
		
		DialogBoxViewModel&                        _model    ;
		const UiStyle                              _style    ;
		Sense::Animation<2, AnimationState>        _animation;
	};

	typedef DialogBoxView<Ui::DialogBoxIntegrated>		DialogBoxIntegrated;
	typedef DialogBoxView<Ui::DialogBoxTouch>			DialogBoxTouch;

}}}

