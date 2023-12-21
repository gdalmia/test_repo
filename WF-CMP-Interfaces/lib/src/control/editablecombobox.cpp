// Copyright Koninklijke Philips N.V., 2020. All Right Reserved

#include "Workflow/control/editablecombobox.h"

namespace Philips { namespace Workflow { namespace Control
{
    EditBoxArea::EditBoxArea(EditableComboBox& parent, VirtualKeyboard &keyboard)
        : TouchEditBox(parent,keyboard, ClearButtonVisible::Hide)
		, _parent(parent)
    {
        std::fill_n(defaultBorder, 8, Palette::Transparent);
        colorBorder = &defaultBorder;
		margin		= Margin(4, 0, 0, 0);
    }

    void EditBoxArea::onResize()
    {
        TouchEditBox::onResize();
        Rect client(getBounds());
        client.right = client.left;
        client.left -= 10;
    }

	bool EditBoxArea::onKeyboardDown(IKeyboard::Button button)
	{
		if (button == IKeyboard::Button::Tab || button == IKeyboard::Button::Enter) keyboardDown(button);
        return TouchEditBox::onKeyboardDown(button);
	}

    void EditBoxArea::onKeyboardEnter(bool argShowFocus)
    {
        TouchEditBox::onKeyboardEnter(argShowFocus);
        if (!enableWarning)
            colorBorder = &defaultBorder;
    }

    void EditBoxArea::onEnabledChanged()
    {
        TouchEditBox::onEnabledChanged();
        if (!enableWarning)
            colorBorder = &defaultBorder;
    }

    void EditBoxArea::onKeyboardLeave()
    {
        TouchEditBox::onKeyboardLeave();
        if (!enableWarning)
            colorBorder = &defaultBorder;
    }

    EditableComboBox::EditableComboBox(Control &parent, VirtualKeyboard& keyboard) :
        ComboBox(parent),
        editBox(*this, keyboard)
		, customArrow(gpu)
    {
        tabStop = false;
        itemImageSpacing = 0;
        editBox.eventChanged = [this]
        {
            const auto & items = getItems();
            auto index = -1;
            auto it = std::find(items.begin(), items.end(), static_cast<std::wstring>(editBox.text));
            if (it != items.end())
            {
                index = static_cast<int>(std::distance(items.begin(), it));
            }
            setSelectedItem(index);
            editBox.font = this->font;
			if (eventSelectionChanged) eventSelectionChanged();
			editboxTextChanged();
        };

		editBox.keyboardDown += [this](const IKeyboard::Button button) 
		{
			// Pass the event to parent, if Tab or Enter key is pressed.
			if (button == IKeyboard::Button::Tab || button == IKeyboard::Button::Enter)	onKeyboardDown(button); 
		};
    }

    void EditableComboBox::onResize()
    {
		ComboBox::onResize();
		editBox.setBounds(Rect(0, 0, getBounds().width() - buttonWidth - 6, getBounds().height()));
    }

    void EditableComboBox::onVisibleChanged()
    {
        ComboBox::onVisibleChanged();
        editBox.visible = isVisible();
    }

    void EditableComboBox::onSelectionChanged()
    {
        ComboBox::onSelectionChanged();
        const int activeIndex = getSelectedItem();
        if (activeIndex > -1)
        {
            editBox.text = getItem(activeIndex).text;
			editBox.setCaretPos(Caret(static_cast<int>(getItem(activeIndex).text.length()), Caret::Type::Pre));
        }
    }

	void EditableComboBox::render(IRenderer& renderer) const
	{
		const auto space = getSpace();
		if (isInvalidated() || !customArrow.isValid(space))
		{
            customArrow.open(space);
			const double arrowRadius = arrowWidth / 2.0;
			const Point center = getArrowPos();
			Matrix mat = Matrix::identity();
			if (!autoDirection) switch (direction)
			{
			case Direction::Down:	mat = Matrix::rotate(0);	break;
			case Direction::Left:	mat = Matrix::rotate(90);	break;
			case Direction::Up:		mat = Matrix::rotate(180);	break;
			case Direction::Right:	mat = Matrix::rotate(270);	break;
			}

			Point point[] =
			{
				center + mat * Point(-arrowRadius, -arrowRadius / 2.0),
				center + mat * Point(arrowRadius, -arrowRadius / 2.0),
				center + mat * Point(0.0,		    arrowRadius / 2.0),
			};

            if(isEnabled())
			    Polygon::draw(customArrow, center, point, 0.0, Palette::Gray15 * 0.55);
            else
                Polygon::draw(customArrow, center, point, 0.0, Palette::Gray65 * 1.30);
            customArrow.close();
		}

		ComboBoxCustom::render(renderer);
        customArrow.render(space, renderer);
	}

    bool EditableComboBox::onKeyboardDown(IKeyboard::Button button)
    {
		const auto result = ComboBox::onKeyboardDown(button);

    	if (button == IKeyboard::Button::Tab)
			tabPressed();

    	return result;
    }

    void EditableComboBox::onEnabledChanged()
    {
        ComboBox::onEnabledChanged();
        editBox.enabled = isEnabled();
    }
	
    std::wstring EditableComboBox::text() const
    {
        return editBox.text;
    }

}}}
