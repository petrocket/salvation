#include "stdafx.h"

//-------------------------------------------------------------------------------------
Game::Game(Ogre::SceneManager *mgr, Ogre::RenderWindow *win) :
	mGUI(0),
	mMainMenuLayout(0),
	mPlatform(0),
	mSceneManager(mgr),
	mRenderWindow(win)
{
}

//-------------------------------------------------------------------------------------
Game::~Game(void)
{
	mGUI->shutdown();
	delete mGUI;
	mGUI = 0;   
	mPlatform->shutdown();
	delete mPlatform;
	mPlatform = 0;
}


//-------------------------------------------------------------------------------------
void Game::createScene(void)
{
	mPlatform = new MyGUI::OgrePlatform();
	mPlatform->initialise(mRenderWindow,mSceneManager);
	mGUI = new MyGUI::Gui();
	//mGUI->initialise();
	mGUI->initialise("MyGUI_BlackOrangeCore.xml"); 

	mMainMenuLayout = MyGUI::LayoutManager::getInstance().loadLayout("mainmenu.layout");
	//mInGameMenuLayout = MyGUI::LayoutManager::getInstance().loadLayout("ingame.layout");
	

	MyGUI::ButtonPtr button = mGUI->findWidget<MyGUI::Button>("QuitButton");
	button->eventMouseButtonClick += MyGUI::newDelegate(this, &Game::exit);

	mSceneManager->setSkyBox(true, "Spacescape1024");
}

void Game::exit(MyGUI::WidgetPtr _sender)
{
	Ogre::Root::getSingleton().shutdown();
	Ogre::Root::getSingleton().queueEndRendering();
}

bool Game::mouseMoved( const OIS::MouseEvent &arg )
{
    MyGUI::InputManager::getInstance().injectMouseMove(arg.state.X.abs, arg.state.Y.abs, arg.state.Z.abs);
    //...
	return true;
}
 
bool Game::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    MyGUI::InputManager::getInstance().injectMousePress(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
    //...
	return true;
}
 
bool Game::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    MyGUI::InputManager::getInstance().injectMouseRelease(arg.state.X.abs, arg.state.Y.abs, MyGUI::MouseButton::Enum(id));
    //...
	return true;
}
 
bool Game::keyPressed( const OIS::KeyEvent &arg )
{
    MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(arg.key), arg.text);

    if (arg.key == OIS::KC_ESCAPE) {
        exit(NULL);
    }

	return true;
}
 
bool Game::keyReleased( const OIS::KeyEvent &arg )
{
    MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(arg.key));
    //...
	return true;
}