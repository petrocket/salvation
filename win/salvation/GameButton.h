#ifndef __MYGUI_GAME_BUTTON_H__
#define __MYGUI_GAME_BUTTON_H__

#include "stdafx.h"

namespace MyGUI
{

	class GameButton :
		public Button
	{
	public:
		GameButton(void);
		virtual ~GameButton(void);

		virtual void onMouseSetFocus(Widget* _old);
		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);
	};

}

#endif