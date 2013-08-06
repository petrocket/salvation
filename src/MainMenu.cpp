#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// using pch
#include "stdafx.h"

#else

#include "MainMenu.h"

#endif

namespace Salvation
{

	MainMenu::MainMenu(MyGUI::Widget* _parent)
	{
		initialiseByAttributes(this, _parent);

		mQuitButtonButton->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::MainMenu::exit);
		mPlayButtonButton->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::MainMenu::play);
	}

	MainMenu::~MainMenu()
	{
	}

	void MainMenu::exit(MyGUI::WidgetPtr _sender)
	{
		Game::getSingleton().exit();
	}

	void MainMenu::play(MyGUI::WidgetPtr _sender)
	{
		Game::getSingleton().play();
	}

	void MainMenu::setVisible(bool visible)
	{
		mMainWidget->setVisible(visible);
	}
} // Salvation
