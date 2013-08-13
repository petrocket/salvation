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
#include "LenseFlare.h"

#endif

typedef enum GameState_t
{
	GameStateIntro = 0,
	GameStateMainMenu,

	GameStateSpace,
	GameStateCity,
	GameStateStation,

	GameStateEnd

} GameState;

typedef enum GameNodeType_t
{
	GameNodeTypeSpace = 0,
	GameNodeTypePlanet,
	GameNodeTypeAsteroids
} GameNodeType;

typedef struct Contact
{
	Ogre::String name;
} Contact;

typedef struct GameNode
{
	Ogre::String title;
	Ogre::String description;

	Ogre::SceneNode *scenenode;
	bool visible;
	bool currentNode;

	GameNodeType type;

	Planet *planet;
	
	int sector;

	bool hasCity;
	Ogre::String cityName;

	bool hasStation;
	Ogre::String stationName;

	std::vector<Contact> cityContacts;
	std::vector<Contact> stationContacts;
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

	void depart(); // leave station/city
	void dock(); // dock at station
	void land(); // land on planet
	void exit(); // exit the game
	void play(); // play a game
	void quit(); // return to main menu (quit game)

	void run(MyGUI::WidgetPtr _sender);
	void fight(MyGUI::WidgetPtr _sender);

	float fuelCostToTravelTo(unsigned int i);
	bool canTravelToNodeWithIndex(unsigned int i);
	bool travelToNodeWithIndex(unsigned int i, bool force = false);

	void update(float dt);

	GameState getGameState() { return mGameState; }
	void setGameState(GameState state);

	void closeEndGameDialogPressed(MyGUI::WidgetPtr _sender);
	void setShowNavGrid(bool show);
	void setNavVisible(bool visible);
	void setMaxGameTime(double time);

	MyGUI::Gui *mGUI;
	MyGUI::OgrePlatform *mPlatform;
	MyGUI::VectorWidgetPtr mMainMenuLayout;
	MyGUI::VectorWidgetPtr mInGameMenuLayout;

	Ogre::Camera *mCamera;
	Ogre::Vector3 mNavCamPosition;
	Ogre::Vector3 mSunCamPosition;
	Ogre::SceneManager *mSceneManager;
	Ogre::RenderWindow *mRenderWindow;

	Ship *mPlayerShip;
	Ship *mEnemyShip;

	Salvation::MainMenu *mMainMenu;
	Salvation::InGameMenu *mInGameMenu;

	Ogre::BillboardSet *mDangerZones;
	Ogre::Billboard *mDangerZone;
	Ogre::Billboard *mWarningZone;
	Ogre::SceneNode *mDangerZoneNode;

	Ogre::SceneNode *mNavGridNode;

	float mDangerZoneStart;
	float mDangerZoneEnd;
	float mWarningZoneSize;

	float mPlayerMoney;
	double mMaxGameTime;
	double mGameTimeRemaining;
	bool mPaused;
	bool mNavOpen;
	bool mInBattle;
	bool mBattleDialogOpen;
	bool mShutdown;
	bool mShowGrid;

	int mCurrentNodeIdx;
	std::vector<GameNode *>mGameNodes;

	Ogre::GameConfig *mConfig;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	sf::Music mMusic;
#endif
private:
	GameState mGameState;
	GameState mPrevGameState;

	Ogre::SceneNode *mGameNodesSceneNode;
	Ogre::SceneNode *mSunSceneNode;

	LensFlare *mLensFlare;

	void createGameNodes(int numSectors, int nodesPerSector);
	void updateVisibleNodes();
};

#endif