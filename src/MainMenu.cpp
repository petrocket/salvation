#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// using pch
#include "stdafx.h"

#else

#include "MainMenu.h"
#include "GameAudioInterface.h"
#include "Game.h"

#endif

namespace Salvation
{

	MainMenu::MainMenu(MyGUI::Widget* _parent)
	{
		initialiseByAttributes(this, _parent);

		mHelpButtonButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);
		mHelpButtonButton->eventMouseButtonClick += MyGUI::newDelegate(playButtonClick);

		mQuitButtonButton->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::MainMenu::exit);
		mQuitButtonButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);
		mQuitButtonButton->eventMouseButtonClick += MyGUI::newDelegate(playButtonClick);

		mPlayButtonButton->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::MainMenu::play);
		mPlayButtonButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);
		mPlayButtonButton->eventMouseButtonClick += MyGUI::newDelegate(playButtonClick);
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
