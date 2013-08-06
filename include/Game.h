#ifndef __Game_h_
#define __Game_h_

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	// using pch
	#include "stdafx.h"
#else

#include "Ogre.h"

#endif

typedef enum GameState_t
{
	GameStateIntro = 0,
	GameStateMainMenu,
	GameStateNav,
	GameStatePlanet,
	GameStateCity,
	GameStateStation,
	GameStateAsteroid,
	GameStateBattle
} GameState;

typedef struct GameNode
{
	GameNode *parent;
	std::vector<GameNode *>children;
	Ogre::String title;
	Ogre::SceneNode *scenenode;
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

	Salvation::MainMenu *mMainMenu;
	Salvation::InGameMenu *mInGameMenu;

	bool mShutdown;

	std::vector<GameNode *>mGameNodes;

private:
	GameState mGameState;
	GameState mPrevGameState;
};

#endif