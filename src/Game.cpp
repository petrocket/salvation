#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// using pch
#include "stdafx.h"

#else

#include "Game.h"

#endif

template<> Game* Ogre::Singleton<Game>::msSingleton = 0;

//-------------------------------------------------------------------------------------
Game::Game(Ogre::SceneManager *mgr, Ogre::RenderWindow *win, Ogre::Camera* cam) :
	mGUI(0),
	mMainMenuLayout(0),
	mPlatform(0),
	mCamera(cam),
	mShutdown(false),
	mSceneManager(mgr),
	mRenderWindow(win)
{
}

//-------------------------------------------------------------------------------------
Game::~Game(void)
{
	if(mGUI) {
		//mGUI->shutdown();
		delete mGUI;
		mGUI = 0; 
	}

	if(mPlatform) {
		//mPlatform->shutdown();
		delete mPlatform;
		mPlatform = 0;
	}
}


//-------------------------------------------------------------------------------------
void Game::createScene(void)
{
	mPlatform = new MyGUI::OgrePlatform();
	mPlatform->initialise(mRenderWindow,mSceneManager);
	mGUI = new MyGUI::Gui();
	mGUI->initialise("MyGUI_BlackOrangeCore.xml"); 

	mMainMenu = new Salvation::MainMenu();
	mInGameMenu = new Salvation::InGameMenu();

	mSceneManager->setSkyBox(true, "Spacescape1024");
}

void Game::exit()
{
	mShutdown = true;
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
 
void Game::play()
{
	//mSceneManager->destroySceneNode(mSceneManager->getRootSceneNode());

	// generate content
	mGameNodes.clear();

	float scale = 1.0f;

	srand(1);

	for(int i = 0; i < 10; i++) {
		GameNode *n = new GameNode();
		n->title = "Node" + Ogre::StringConverter::toString(i);
		Ogre::Vector3 pos;
		pos.x = (float)(rand() % mRenderWindow->getWidth()) * scale;
		pos.y = 0;
		pos.z = (float)(rand() % mRenderWindow->getHeight()) * scale;

		n->scenenode = mSceneManager->getRootSceneNode()->createChildSceneNode(pos);

		Ogre::Entity *ent = mSceneManager->createEntity(n->title,
			Ogre::SceneManager::PT_SPHERE);
		n->scenenode->setScale(0.1,0.1,0.1);
		n->scenenode->attachObject(ent);
		n->scenenode->showBoundingBox(true);

		mGameNodes.push_back(n);
	}

	Ogre::Vector3 camPos = Ogre::Vector3(
		(float)mRenderWindow->getWidth() * scale * 0.5,
		1500.0 * scale,
		(float)mRenderWindow->getWidth() * scale * 0.5);


	mCamera->setNearClipDistance(1.0);
	mCamera->setFarClipDistance(10000.0);
	mCamera->setFixedYawAxis(false);
	mCamera->setAutoTracking(false);

	mCamera->setPosition(camPos);
	mCamera->lookAt(Ogre::Vector3(camPos.x,0,camPos.z));

	setGameState(GameStateNav);
}

void Game::quit()
{
	setGameState(GameStateMainMenu);
}

void Game::setGameState(GameState state)
{
	mPrevGameState = mGameState;
	mGameState = state;

	switch(mGameState) {
		case GameStateIntro:
			break;
		case GameStateMainMenu:
			mMainMenu->setVisible(true);
			mInGameMenu->setVisible(false);
			break;
		default:
		case GameStateNav:
			mMainMenu->setVisible(false);
			mInGameMenu->setVisible(true);
			break;
	};

	mInGameMenu->update();
}

bool Game::keyPressed( const OIS::KeyEvent &arg )
{
    MyGUI::InputManager::getInstance().injectKeyPress(MyGUI::KeyCode::Enum(arg.key), arg.text);

    if (arg.key == OIS::KC_ESCAPE) {
        exit();
    }

	return true;
}
 
bool Game::keyReleased( const OIS::KeyEvent &arg )
{
    MyGUI::InputManager::getInstance().injectKeyRelease(MyGUI::KeyCode::Enum(arg.key));
    //...
	return true;
}

void Game::update(float dt)
{
	if(mShutdown) {
		mShutdown = false;

		mGUI->shutdown();
		mPlatform->shutdown();

		Ogre::Root::getSingleton().queueEndRendering();
		Ogre::Root::getSingleton().shutdown();
	}
}
