#ifndef __Game_h_
#define __Game_h_

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	// using pch
	#include "stdafx.h"
#else

#include "Ogre.h"
#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"

#include <OIS.h>

#ifdef OGRE_IS_IOS
#   include <OISMultiTouch.h>
#endif

#include "Ship.h"
#include "InGameMenu.h"
#include "MainMenu.h"

#endif

typedef enum GameState_t
{
	GameStateIntro = 0,
	GameStateMainMenu,
	GameStateNav,

	GameStateSpace,

	GameStateCity,
	GameStateStation,
	GameStateAsteroid,

	GameStateBattle
} GameState;

typedef struct GameNode
{
	Ogre::String title;
	Ogre::SceneNode *scenenode;
	bool visible;
	bool currentNode;
} GameNode;

class Game : public OIS::MouseListener , public OIS::KeyListener , public Ogre::Singleton<Game>
{
public:
    Game(Ogre::SceneManager *mgr, Ogre::RenderWindow *win, Ogre::Camera *cam);
    virtual ~Game(void);

    void createScene(void);

    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	void exit(); // exit the game
	void play(); // play a game
	void quit(); // return to main menu (quit game)

	bool travelToNodeWithIndex(unsigned int i, bool force = false); // attempt to travel
	void update(float dt);

	GameState getGameState() { return mGameState; }
	void setGameState(GameState state);

	MyGUI::Gui *mGUI;
	MyGUI::OgrePlatform *mPlatform;
	MyGUI::VectorWidgetPtr mMainMenuLayout;
	MyGUI::VectorWidgetPtr mInGameMenuLayout;

	Ogre::Camera *mCamera;
	Ogre::SceneManager *mSceneManager;
	Ogre::RenderWindow *mRenderWindow;

	Ship *mPlayerShip;

	Salvation::MainMenu *mMainMenu;
	Salvation::InGameMenu *mInGameMenu;

	bool mShutdown;
	int mCurrentNodeIdx;
	std::vector<GameNode *>mGameNodes;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	sf::Music mMusic;
#endif
private:
	GameState mGameState;
	GameState mPrevGameState;

	Ogre::SceneNode *mGameNodesSceneNode;

	void createGameNodes(int numSectors, int nodesPerSector);
	void updateVisibleNodes();
};

#endif