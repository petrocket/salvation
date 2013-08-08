#include "stdafx.h"

namespace MyGUI
{
	GameButton::GameButton(void)
	{
	}


	GameButton::~GameButton(void)
	{
	}


	void GameButton::onMouseSetFocus(Widget* _old)
	{
		Button::onMouseSetFocus(_old);
		playEffect("hover.ogg");
	}

	void GameButton::onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		Button::onMouseButtonPressed(_left,_top,_id);
		//playEffect("click2");
	}

	void GameButton::onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		Button::onMouseButtonReleased(_left,_top,_id);
		playEffect("click.ogg");
	}

}