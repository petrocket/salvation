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
	mCamera(cam),
	mCurrentNodeIdx(0),
	mGameNodesSceneNode(0),
	mGUI(0),
	mMainMenuLayout(0),
	mPlatform(0),
	mPlayerShip(0),
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

	if(mPlayerShip) {
		delete mPlayerShip;
		mPlayerShip = 0;
	}

	mGameNodes.clear();
}

void randomPointInCircle(float radius, float &x, float &y)
{
	;
	float t = 2 * Ogre::Math::PI * Ogre::Math::UnitRandom();
	float u = Ogre::Math::UnitRandom() + Ogre::Math::UnitRandom();
	float r = u > 1.0 ? 2.0 - u : u;
	x = r * cos(t) * radius;
	y = r * sin(t) * radius;
}

bool isWithinMinRadius(Ogre::Vector2 pos, float radius, std::vector<GameNode *> *nodes)
{
	float r2 = radius * radius;

	for(unsigned int i = 0; i < nodes->size(); i++) {
		GameNode *n = nodes->at(i);
		Ogre::Vector2 p2 = Ogre::Vector2(
			n->scenenode->getPosition().x,
			n->scenenode->getPosition().z
			);

		if(pos.squaredDistance(p2) < r2) {
			return true;
		}
	}
	return false;
}

//-------------------------------------------------------------------------------------
void Game::createGameNodes(int numSectors, int nodesPerSector)
{
	if(mGameNodesSceneNode) {
		OgreUtil::destroySceneNode(mGameNodesSceneNode);
		mGameNodesSceneNode = NULL;
	}
	
	// generate content
	mGameNodes.clear();

	mGameNodesSceneNode =  mSceneManager->getRootSceneNode()->createChildSceneNode();

	//srand(1);
	
	int numColumns = 0;
	int sector = 0;
	// each sector is wider than the previous by one
	for(sector = 0; sector < numSectors; sector++) {
		numColumns += sector + 1;
	}

	int columnWidth = (int)floorf((float)mRenderWindow->getWidth() / (float)numColumns);
	int columnHeight = mRenderWindow->getHeight();
	Ogre::Vector2 sectorCenter(columnWidth / 2,columnHeight / 2);

	Ogre::Vector2 pos;
	Ogre::Vector2 startPos;

	int numNodes = 0;

	for(sector = 0; sector < numSectors; sector++) {
		int sectorWidth = columnWidth * (sector + 1);
		float sectorRadius = (float)sectorWidth * 0.5;

		for(int i =0;i < nodesPerSector; i++) {
			while(1) {
				randomPointInCircle(sectorRadius * 2.0, pos.x, pos.y);
				pos.x += sectorCenter.x;
				pos.y += sectorCenter.y;
				if(pos.x > 0 && pos.x < (float)mRenderWindow->getWidth() &&
					pos.y > 0 && pos.y < (float)mRenderWindow->getHeight()) {

					if(!isWithinMinRadius(pos,sectorRadius * 0.75,&mGameNodes)) {
						break;
					}
				}
			}

			GameNode *n = new GameNode();
			n->title = "Node" + Ogre::StringConverter::toString(numNodes);
			n->currentNode = false;
			n->visible = false;
			if(sector == 0 && i == 0) {
				pos.x = 0;
				n->currentNode = true;
				n->visible = true;
				startPos = Ogre::Vector2(pos.x,pos.y);
			}
			else if(sector == numSectors - 1 && i == nodesPerSector - 1) {
				pos.x = sectorCenter.x + sectorRadius;
			}
			n->scenenode = mGameNodesSceneNode->createChildSceneNode(Ogre::Vector3(pos.x,0,pos.y));

			Ogre::Entity *ent = mSceneManager->createEntity(n->title,
				Ogre::SceneManager::PT_SPHERE);
			n->scenenode->setScale(0.4f,0.4f,0.4f);
			n->scenenode->attachObject(ent);
			//n->scenenode->showBoundingBox(true);
			n->scenenode->setVisible(n->visible);
			mGameNodes.push_back(n);
			numNodes++;
		}

		int nextSectorWidth = columnWidth * (sector + 1);
		float nextSectorRadius = (float)nextSectorWidth;
		sectorCenter.x += sectorRadius + nextSectorRadius;
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

	playBackgroundMusic("music.ogg");
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
	mCurrentNodeIdx = 0;

	if(!mPlayerShip) {
		mPlayerShip = new Ship();
	}

	mPlayerShip->reset();

	createGameNodes(3,4);

	updateVisibleNodes();

	Ogre::Vector3 camPos = Ogre::Vector3(
		(float)mRenderWindow->getWidth() * 0.5,
		1400.0,
		(float)mRenderWindow->getHeight() * 0.5);

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

bool Game::travelToNodeWithIndex(unsigned int i, bool force)
{
	if(i >= mGameNodes.size()) {
		return false;
	}

	if(mCurrentNodeIdx == i) {
		return false;
	}

	if(!force) {
		GameNode *curNode = mGameNodes[mCurrentNodeIdx];
		GameNode *destNode = mGameNodes[i];

		Ogre::Vector3 curPos = curNode->scenenode->getPosition();
		Ogre::Vector3 destPos = destNode->scenenode->getPosition();

		// check range
		float rangeSquared = mPlayerShip->mMaxJumpRange * mPlayerShip->mMaxJumpRange;

		if(curPos.squaredDistance(destPos) > rangeSquared) {
			return false;
		}
	}

	mCurrentNodeIdx = i;
	
	updateVisibleNodes();
	
	mInGameMenu->update();

	playEffect("warp.ogg");
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

void Game::updateVisibleNodes()
{
	GameNode *curNode = mGameNodes[mCurrentNodeIdx];
	float range =mPlayerShip->maxJumpRangeForEngineLevel(mPlayerShip->mEngineLevel + 1);
	float squaredRange = range * range;

	Ogre::Vector3 curPos = curNode->scenenode->getPosition();

	for(int i = 0; i < mGameNodes.size(); i++) {
		GameNode *n = mGameNodes[i];
		if(n->scenenode->getPosition().squaredDistance(curPos) <= squaredRange) {
			n->visible = true;
			n->scenenode->setVisible(true);
		}
	}
}