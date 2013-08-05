#ifndef _MAIN_MENU_PANEL_H_
#define _MAIN_MENU_PANEL_H_

#include "BaseLayout/BaseLayout.h"

namespace Salvation
{

	ATTRIBUTE_CLASS_LAYOUT(MainMenuPanel, "gui|0");
	class MainMenuPanel :
		public wraps::BaseLayout
	{
	public:
		MainMenuPanel(MyGUI::Widget* _parent = nullptr);
		virtual ~MainMenuPanel();

	private:
	//%LE Widget_Declaration list start
		ATTRIBUTE_FIELD_WIDGET_NAME(MainMenuPanel, mTestWindow, "Test");
		MyGUI::Window* mTestWindow;
	//%LE Widget_Declaration list end
	};

} // namespace Salvation

#endif // _MAIN_MENU_PANEL_H_
