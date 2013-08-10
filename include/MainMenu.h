#ifndef _MAIN_MENU_H_
#define _MAIN_MENU_H_

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	// using pch
	#include "stdafx.h"
#else

#include "MyGUI.h"
#include "BaseLayout/BaseLayout.h"

#endif


namespace Salvation
{

	ATTRIBUTE_CLASS_LAYOUT(MainMenu, "mainmenu.layout");
	class MainMenu :
		public wraps::BaseLayout
	{
	public:
		MainMenu(MyGUI::Widget* _parent = nullptr);
		virtual ~MainMenu();

		void exit(MyGUI::WidgetPtr _sender);
		void play(MyGUI::WidgetPtr _sender);

		void setVisible(bool visible);

	private:
	//%LE Widget_Declaration list start
		ATTRIBUTE_FIELD_WIDGET_NAME(MainMenu, mQuitButtonButton, "QuitButton");
		MyGUI::Button* mQuitButtonButton;
		ATTRIBUTE_FIELD_WIDGET_NAME(MainMenu, mHelpButtonButton, "HelpButton");
		MyGUI::Button* mHelpButtonButton;
		ATTRIBUTE_FIELD_WIDGET_NAME(MainMenu, mPlayButtonButton, "PlayButton");
		MyGUI::Button* mPlayButtonButton;
	//%LE Widget_Declaration list end
	};

} // namespace Salvation

#endif // _MAIN_MENU_H_
