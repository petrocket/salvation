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

		mHelpButtonButton->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::MainMenu::help);
		mHelpButtonButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);
		mHelpButtonButton->eventMouseButtonClick += MyGUI::newDelegate(playButtonClick);

		mQuitButtonButton->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::MainMenu::exit);
		mQuitButtonButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);
		mQuitButtonButton->eventMouseButtonClick += MyGUI::newDelegate(playButtonClick);

		mPlayButtonButton->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::MainMenu::play);
		mPlayButtonButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);
		mPlayButtonButton->eventMouseButtonClick += MyGUI::newDelegate(playButtonClick);

		mHelpWindow->eventWindowButtonPressed += MyGUI::newDelegate(this, &Salvation::MainMenu::closeHelp);
	}

	MainMenu::~MainMenu()
	{

	}

	void MainMenu::closeHelp(MyGUI::Window* _sender, const std::string& _name)
	{
		mHelpWindow->setVisibleSmooth(false);
	}

	void MainMenu::exit(MyGUI::WidgetPtr _sender)
	{
		Game::getSingleton().exit();
	}

	void MainMenu::help(MyGUI::WidgetPtr _sender)
	{
		mHelpWindow->setVisibleSmooth(true);
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
