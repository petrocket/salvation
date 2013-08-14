#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// using pch
#include "stdafx.h"

#else

#include "Game.h"
#include "OgreUtil.h"
#include "GameAudioInterface.h"

#endif


#define kNumContactNames 30
static const Ogre::String gContactNames[kNumContactNames] = {
	"Keithua Jonand",
	"Denne Ramas",
	"Cotter Jones",
	"Rice Whomorr",
	"Damy Barnez",
	"Phomy Bramart",
	"Peter Tiner",
	"Johnny Ancher",
	"Jamy Foste",
	"Grusse Morray",
	"Jane Warders",
	"Kimby Patte",
	"Katha Sonand",
	"Michy Bakell",
	"Dora Cookins",
	"Mara Kere",
	"Herie Homas",
	"Cara Pera",
	"Cara Halley",
	"Juda Bailey",
	"Dralbe Voga",
	"Meni",
	"Xani",
	"A'Borga",
	"Foama M'Akler",
	"Troni",
	"Mara",
	"Q'Shalni",
	"Krulle",
	"E'Feri",
};

#define kNumPlanetNames 12
static const Ogre::String gPlanetNames[kNumPlanetNames] = {
	"Caeli Prime",
	"Vorta",
	"New Triton",
	"Rani",
	"A'Vilgi",
	"Andan IV",
	"Aman Prime",
	"Hrist",
	"Zeta V",
	"Arda",
	"Phae",
	"Cerberi"
};

#define kNumCityNames 12
static const Ogre::String gCityNames[kNumCityNames] = {
	"Forme",
	"Dievs",
	"Bren",
	"Octanus",
	"Varda",
	"Eldon III",
	"New Tetra",
	"Vely",
	"Fron Colony",
	"Gornan",
	"Oran V",
	"Nienna",
};

#define kNumStationNames 12
static const Ogre::String gStationNames[kNumStationNames] = {
	"Alpha Prime",
	"New Vini",
	"Solitude",
	"Prosperus IV",
	"Truma 344",
	"Veda II",
	"Antini",
	"Zalda 7",
	"Boto VI",
	"Station 891",
	"Station 37",
	"Meni",
};

template<> Game* Ogre::Singleton<Game>::msSingleton = 0;

//-------------------------------------------------------------------------------------
Game::Game(Ogre::SceneManager *mgr, Ogre::RenderWindow *win, Ogre::Camera* cam) :
	mCamera(cam),
	mConfig(0),
	mCurrentNodeIdx(0),
	mEnemyShip(0),
	mGameNodesSceneNode(0),
	mGameTimeRemaining(0.0),
	mGUI(0),
	mInBattle(false),
	mBattleStarted(false),
	mLensFlare(0),
	mMainMenuLayout(0),
	mNavGridNode(0),
	mNavOpen(false),
	mPlatform(0),
	mPlayerShip(0),
	mPaused(false),
	mShutdown(false),
	mSceneManager(mgr),
	mRenderWindow(win)
{
	mConfig = new Ogre::GameConfig();
	mConfig->load("Game.cfg","=:\t", false);
	mShowGrid = Ogre::StringConverter::parseBool(
		mConfig->getSetting("showGrid","","false"));

	mMaxGameTime = Ogre::StringConverter::parseReal(
		mConfig->getSetting("maxGameTime","","180"));

}

//-------------------------------------------------------------------------------------
Game::~Game(void)
{
	mConfig->save();

	if(mLensFlare) {
		mLensFlare->End();
		delete mLensFlare;
		mLensFlare = 0;
	}

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

bool Game::canTravelToNodeWithIndex(unsigned int i)
{
	if(mGameState != GameStateSpace) {
		return false;
	}

	if(i >= mGameNodes.size()) {
		return false;
	}

	if(mCurrentNodeIdx == i) {
		return false;
	}

	GameNode *curNode = mGameNodes[mCurrentNodeIdx];
	GameNode *destNode = mGameNodes[i];

	Ogre::Vector3 curPos = curNode->scenenode->getPosition();
	Ogre::Vector3 destPos = destNode->scenenode->getPosition();

	// check range
	float rangeSquared = mPlayerShip->mMaxJumpRange * mPlayerShip->mMaxJumpRange;

	if(curPos.squaredDistance(destPos) > rangeSquared) {
		return false;
	}

	float fuelCost = fuelCostToTravelTo(i);
	if(fuelCost > mPlayerShip->mFuel) {
		return false;
	}
	
	return true;
}

float Game::fuelCostToTravelTo(unsigned int i)
{
	GameNode *curNode = mGameNodes[mCurrentNodeIdx];
	GameNode *destNode = mGameNodes[i];

	// check fuel cost
	float dist = curNode->scenenode->getPosition().distance(
		destNode->scenenode->getPosition());

	// distance is relative to playing area size
	float fuelNeeded = dist / (float)mRenderWindow->getWidth();

	return fuelNeeded;
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
void Game::closeEndGameDialogPressed(MyGUI::WidgetPtr _sender)
{
	setGameState(GameStateMainMenu);
}

//-------------------------------------------------------------------------------------
void Game::closeBattleDialogPressed(MyGUI::WidgetPtr _sender)
{
	mInGameMenu->closeDialog(NULL);

	mInBattle = false;

	delete mEnemyShip;
	mEnemyShip = 0;

	mPlayerShip->updateSpecs();
	mPlayerShip->prepForBattle();
	mInGameMenu->update();
}

//-------------------------------------------------------------------------------------
void Game::createGameNodes(int numSectors, int nodesPerSector)
{
	srand(time(NULL));
	
	if(mGameNodesSceneNode) {
		OgreUtil::destroySceneNode(mGameNodesSceneNode);
		mGameNodesSceneNode = NULL;
	}
	
	std::vector<Ogre::String> planetNames;
	for(int i = 0; i < kNumPlanetNames; i++) {
		planetNames.push_back(gPlanetNames[i]);
	}
	std::random_shuffle(planetNames.begin(), planetNames.end());

	std::vector<Ogre::String> cityNames;
	for(int i = 0; i < kNumCityNames; i++)  {
		cityNames.push_back(gCityNames[i]);
	}
	std::random_shuffle(cityNames.begin(), cityNames.end());

	std::vector<Ogre::String> stationNames;
	for(int i = 0; i < kNumStationNames; i++)  {
		stationNames.push_back(gStationNames[i]);
	}
	std::random_shuffle(stationNames.begin(), stationNames.end());

	std::vector<Ogre::String> contactNames;
	for(int i = 0; i < kNumContactNames; i++)  {
		contactNames.push_back(gContactNames[i]);
	}
	std::random_shuffle(contactNames.begin(), contactNames.end());

	// generate content
	mGameNodes.clear();

	mGameNodesSceneNode =  mSceneManager->getRootSceneNode()->createChildSceneNode();


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

					// don't let nodes be too close together
					if(!isWithinMinRadius(pos,sectorRadius * 0.75,&mGameNodes)) {
						break;
					}
				}
			}

			GameNode *n = new GameNode();
			n->currentNode = false;
			n->sector = sector;
			n->visible = false;
			n->hasCity = false;
			n->hasStation = false;
			if(sector == 0 && i == 0) {
				// first node has planet and station
				pos.x = 0;
				n->currentNode = true;
				n->visible = true;
				startPos = Ogre::Vector2(pos.x,pos.y);
				n->type = GameNodeTypePlanet;
				n->hasCity = true;
				n->hasStation = true;
			}
			else if(sector == numSectors - 1 && i == nodesPerSector - 1) {
				// last node is planet
				pos.x = (float)mRenderWindow->getWidth();
				n->hasCity = true;
				n->hasStation = Ogre::Math::UnitRandom() > 0.5;
				n->type = GameNodeTypePlanet;
			}
			else if(i == 0) {
				// first node in sector is always a planet
				n->hasCity = true;
				n->hasStation = Ogre::Math::UnitRandom() > 0.5;
				n->type = GameNodeTypePlanet;
			}
			else {
				float r = Ogre::Math::UnitRandom();
				if(r < 0.25) {
					n->type = GameNodeTypeAsteroids;
					n->hasStation = true;
				}
				else if(r < 0.5) {
					n->type = GameNodeTypeSpace;
					n->hasStation = true;
				}
				else {
					n->type = GameNodeTypePlanet;
					n->hasCity = Ogre::Math::UnitRandom() > 0.5;
					if(!n->hasCity) {
						n->hasStation = true;
					}
					else {
						n->hasStation = Ogre::Math::UnitRandom() > 0.5;
					}
				}
			}

			n->scenenode = mGameNodesSceneNode->createChildSceneNode(Ogre::Vector3(pos.x,0,pos.y));
			//n->scenenode->showBoundingBox(true);
			if(n->type == GameNodeTypePlanet) {
				n->planet = new Planet(n->scenenode);
			}

			//if(n->type == GameNodeTypePlanet) {
				n->title = planetNames[numNodes % planetNames.size()];
			//}
			Contact c;
			if (n->hasCity) {
				n->cityName = cityNames[numNodes %cityNames.size()];
				c.name = contactNames[(numNodes * 4) % contactNames.size()];
				n->cityContacts.push_back(c);
				c.name = contactNames[(numNodes * 4 + 1) % contactNames.size()];
				n->cityContacts.push_back(c);
			}
			if(n->hasStation) {
				n->stationName = stationNames[numNodes % stationNames.size()];
				c.name = contactNames[(numNodes * 4 + 2) % contactNames.size()];
				n->stationContacts.push_back(c);
				c.name = contactNames[(numNodes * 4 + 3) % contactNames.size()];
				n->stationContacts.push_back(c);
			}

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
	
	// randomly rotate the skybox
	Ogre::SceneNode *n = mSceneManager->getSkyBoxNode();
	n->yaw(Ogre::Radian(Ogre::Math::RangeRandom(0.0,Ogre::Math::TWO_PI)));
	n->pitch(Ogre::Radian(Ogre::Math::RangeRandom(0.0,Ogre::Math::TWO_PI)));
	n->roll(Ogre::Radian(Ogre::Math::RangeRandom(0.0,Ogre::Math::TWO_PI)));

	//playBackgroundMusic("music.ogg");


	mDangerZones = mSceneManager->createBillboardSet();
	mDangerZones->setAutoUpdate(true);

	float offset = 800.0f;
	Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName("RangeMaterial");
	mDangerZones->setMaterial(mat);
	mDangerZoneNode =  mSceneManager->getRootSceneNode()->createChildSceneNode();
	mDangerZoneNode->setPosition(-offset * 0.5, 0, mRenderWindow->getHeight() * 0.5f);
	mDangerZoneNode->attachObject(mDangerZones);
	
	mDangerZoneStart = offset;
	mWarningZoneSize = 200.0;
	mDangerZoneEnd = mDangerZoneStart + (float)mRenderWindow->getWidth() - mWarningZoneSize;

	mWarningZone = mDangerZones->createBillboard(Ogre::Vector3::ZERO);
	mWarningZone->setColour(Ogre::ColourValue(1.0,1.0,0.5,1.0));
	mWarningZone->setDimensions(mDangerZoneStart + mWarningZoneSize,
				(mDangerZoneStart * 2.0) + mWarningZoneSize);

	mDangerZone = mDangerZones->createBillboard(Ogre::Vector3::ZERO);
	mDangerZone->setColour(Ogre::ColourValue(1.0,0,0,1.0));
	mDangerZone->setDimensions(mDangerZoneStart,mDangerZoneStart * 2.0);

	// sun
	mSunSceneNode =  mSceneManager->getRootSceneNode()->createChildSceneNode(
		mDangerZoneNode->getPosition());
	Ogre::Entity *sun = Game::getSingleton().mSceneManager->createEntity(Ogre::SceneManager::PT_SPHERE);
	mSunSceneNode->attachObject(sun);	

	setShowNavGrid(mShowGrid);

	mNavCamPosition =  Ogre::Vector3(
		(float)mRenderWindow->getWidth() * 0.5,
		1400.0,
		(float)mRenderWindow->getHeight() * 0.5);
	float sunOffset = 100.0;
	/*
	Ogre::Vector3 sunDir = Ogre::Vector3(
		Ogre::Math::RangeRandom(-1.0,1.0),
		Ogre::Math::RangeRandom(-1.0,1.0),
		Ogre::Math::RangeRandom(-1.0,1.0)
		);
	sunDir->normalise();
	*/
	mSunCamPosition = mSunSceneNode->getPosition() + (Ogre::Vector3::UNIT_X * sunOffset);
	mCamera->setNearClipDistance(1.0);
	mCamera->setFarClipDistance(10000.0);
	mCamera->setFixedYawAxis(false);
	mCamera->setAutoTracking(false);

	mCamera->setPosition(mSunCamPosition);
	mCamera->lookAt(mSunSceneNode->getPosition());

	mLensFlare = new LensFlare();//mSunSceneNode->getPosition(),mCamera,mSceneManager);
	mLensFlare->Init(mSceneManager,mCamera,
		(float)mRenderWindow->getWidth(),
		(float)mRenderWindow->getHeight(),
        200, //The size of the Sun flare 
        25, //The base size of all the other flares
        "LensFlareHalo", "LensFlareCircle", "LensFlareBurst");
	mLensFlare->SetPosition(mSunSceneNode->getPosition());

	/*
	mSafeZones = mSceneManager->createBillboardSet();
	mSafeZones->setAutoextend(true);
	mSafeZones->setDefaultDimensions(offset + 200, offset + 200);
	*/

	setGameState(GameStateMainMenu);
}

void Game::depart()
{
	if(mGameState == GameStateCity || mGameState == GameStateStation) {
		setGameState(GameStateSpace);
	}
}

void Game::dock()
{
	if(mGameState != GameStateStation) {
		setGameState(GameStateStation);
	}
}

void Game::land()
{
	if(mGameState != GameStateCity) {
		setGameState(GameStateCity);
	}
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

	mPlayerMoney =  Ogre::StringConverter::parseReal(
		mConfig->getSetting("startFunds","","1000.0"));

	// randomly rotate the skybox
	Ogre::SceneNode *n = mSceneManager->getSkyBoxNode();
	n->yaw(Ogre::Radian(Ogre::Math::RangeRandom(0.0,Ogre::Math::TWO_PI)));
	n->pitch(Ogre::Radian(Ogre::Math::RangeRandom(0.0,Ogre::Math::TWO_PI)));
	n->roll(Ogre::Radian(Ogre::Math::RangeRandom(0.0,Ogre::Math::TWO_PI)));

	if(!mPlayerShip) {
		mPlayerShip = new Ship();
	}

	mPlayerShip->reset();

	int numSectors = Ogre::StringConverter::parseInt(
		mConfig->getSetting("numSectors","","3"));
	int numNodesPerSector = Ogre::StringConverter::parseInt(
		mConfig->getSetting("numNodesPerSector","","4"));

	createGameNodes(numSectors,numNodesPerSector);
	mGameNodesSceneNode->setVisible(true);

	mPlayerShip->mRangeBillboard->setPosition(
		mGameNodes[mCurrentNodeIdx]->scenenode->getPosition());
	mPlayerShip->mRangeBillboardSet->setDefaultDimensions(
		mPlayerShip->mMaxJumpRange * 2.0,
		mPlayerShip->mMaxJumpRange * 2.0);

	updateVisibleNodes();

	setGameState(GameStateCity);
	
	setNavVisible(false);

	mInBattle = false;
	mBattleDialogOpened = false;

	mGameTimeRemaining = mMaxGameTime;
}

void Game::quit()
{
	setGameState(GameStateMainMenu);
}

void Game::setGameState(GameState state)
{
	mPrevGameState = mGameState;
	mGameState = state;

	mLensFlare->SetFlareVisible(true);
	mSunSceneNode->setScale(0.05f,0.05f,0.05f);

	switch(mGameState) {
		case GameStateIntro:
			break;
		case GameStateMainMenu:
			mMainMenu->setVisible(true);
			mInGameMenu->setVisible(false);

			setNavVisible(false);

			mCamera->setFixedYawAxis(true,Ogre::Vector3::UNIT_Y);
			mCamera->setPosition(mSunCamPosition);
			mCamera->lookAt(mSunSceneNode->getPosition());

			// hide all the game nodes
			if(mGameNodesSceneNode) {
				mGameNodesSceneNode->setVisible(false);
			}

			// no flare in title
			mLensFlare->SetFlareVisible(false);
			mSunSceneNode->setScale(0.3f,0.3f,0.3f);
			break;
		case GameStateEnd:
			{
				if(mPlayerShip->mHullStrength <= 0) {
					mInGameMenu->displayDialog("Game Over",
						"Your ship was destroyed",
						"lose.png", 
						"",
						NULL,
						"CLOSE",
						MyGUI::newDelegate(this,&Game::closeEndGameDialogPressed));
				}
				else {
					GameNode *node = mGameNodes[mCurrentNodeIdx];
					bool survive = false;
					float x = node->scenenode->getPosition().x;
					if( x > mRenderWindow->getWidth() - 100) {
						survive = true;
					}
					else if(x > mRenderWindow->getWidth() - mWarningZoneSize) {
						survive = Ogre::Math::UnitRandom() > 0.5;
					}
					if(survive) {
						mInGameMenu->displayDialog("Game Over","You Survived!","win.png",
							"",
							NULL,
							"CLOSE",
							MyGUI::newDelegate(this,&Game::closeEndGameDialogPressed));
					}
					else {
						mInGameMenu->displayDialog("Game Over","Your didn't make it","lose.png", 
							"",
							NULL,
							"CLOSE",
							MyGUI::newDelegate(this,&Game::closeEndGameDialogPressed));
					}
				}

				break;
			}
		case GameStateSpace:
		default:
			if(mNavOpen) {
				if(mDangerZoneNode) {
					mDangerZoneNode->setVisible(true);
				}
				if(mPlayerShip->mRangeBillboardSet) {
					mPlayerShip->mRangeBillboardSet->setVisible(true);
				}
			}
			else {
				if(mPlayerShip->mRangeBillboardSet) {
					mPlayerShip->mRangeBillboardSet->setVisible(false);
				}
				if(mDangerZoneNode) {
					mDangerZoneNode->setVisible(false);
				}
			}
			mMainMenu->setVisible(false);
			mInGameMenu->setVisible(true);
			break;
	};

	mInGameMenu->update();
}

void Game::setMaxGameTime(double time)
{
	mMaxGameTime = time;
	mConfig->setSetting(Ogre::String("maxGameTime"),
		Ogre::StringConverter::toString((float)mMaxGameTime));
	mConfig->save();
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

void Game::setNavVisible(bool visible) 
{
	mNavOpen = visible;
	if(mPlayerShip && mPlayerShip->mRangeBillboardSet) {
		mPlayerShip->mRangeBillboardSet->setVisible(visible);
	}
	if(mDangerZoneNode) {
		mDangerZoneNode->setVisible(visible);
	}

	if(visible) {
		mCamera->setFixedYawAxis(true,-Ogre::Vector3::UNIT_Z);
		mCamera->setPosition(mNavCamPosition);
		mCamera->lookAt(Ogre::Vector3(mNavCamPosition.x,0,mNavCamPosition.z));
	}
	else {
		if(mGameState > GameStateMainMenu) {
			Ogre::Vector3 nodePos = mGameNodes[mCurrentNodeIdx]->scenenode->getPosition();
			Ogre::Vector3 dir = mDangerZoneNode->getPosition() - nodePos;
			dir.normalise();


			float offset = 10.0f;
			Ogre::Vector3 camPos = nodePos - dir * offset;

			Ogre::Vector3 xDir = dir.crossProduct(Ogre::Vector3::UNIT_Y);
			camPos += xDir * offset * 0.5;
			mCamera->setPosition(camPos);
			mCamera->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);
			mCamera->lookAt(nodePos);
		}
	}
}

void Game::setShowNavGrid(bool show)
{
	mShowGrid = show;

	if(mShowGrid) {
		if(!mNavGridNode) {
			Ogre::Entity *p = mSceneManager->createEntity(Ogre::SceneManager::PT_PLANE);
			mNavGridNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
			mNavGridNode->attachObject(p);
			mNavGridNode->lookAt( Ogre::Vector3::UNIT_Y, Ogre::Node::TS_WORLD,Ogre::Vector3::UNIT_Z );
			mNavGridNode->setPosition((float)mRenderWindow->getWidth() * 0.5,
				0,
				(float)mRenderWindow->getHeight() * 0.5);
			mNavGridNode->setScale(Ogre::Vector3(
				(float)mRenderWindow->getWidth() * 0.01 * 0.5,
				(float)mRenderWindow->getHeight() * 0.01 * 0.5,
				(float)mRenderWindow->getHeight() * 0.01 * 0.5));
		}
	}

	if(mNavGridNode) {
		mNavGridNode->setVisible(mShowGrid);
	}

	mConfig->setSetting(Ogre::String("showGrid"),Ogre::StringConverter::toString(mShowGrid));
	mConfig->save();
}

bool Game::travelToNodeWithIndex(unsigned int i, bool force)
{
	if(!force && !canTravelToNodeWithIndex(i)) return false;

	mPlayerShip->mFuel -= fuelCostToTravelTo(i);
	if(mPlayerShip->mFuel < 0) mPlayerShip->mFuel = 0;

	mCurrentNodeIdx = i;
	
	mPlayerShip->mRangeBillboard->setPosition(
		mGameNodes[mCurrentNodeIdx]->scenenode->getPosition());
	mPlayerShip->mRangeBillboardSet->setDefaultDimensions(
		mPlayerShip->mMaxJumpRange * 2.0,
		mPlayerShip->mMaxJumpRange * 2.0);
	updateVisibleNodes();
	
	setGameState(GameStateSpace);

	if(Ogre::Math::UnitRandom() > 0.5 || 1) {
		mInBattle = true;
		mBattleDialogOpened = false;
		mBattleStarted = false;
	}

	setNavVisible(false);

	mInGameMenu->update();

	playEffect("warp.ogg");
	return true;
}

void Game::run(MyGUI::WidgetPtr _sender)
{
	mInGameMenu->closeDialog(NULL);
	mInBattle = false;
	if(mEnemyShip) {
		delete mEnemyShip;
		mEnemyShip = 0;
	}
	mBattleStarted = false;

	mPlayerShip->updateSpecs();
	mPlayerShip->prepForBattle();
}

void Game::fight(MyGUI::WidgetPtr _sender)
{
	mInGameMenu->closeDialog(NULL);
	mBattleStarted = true;
	mInBattle = true;
	mInGameMenu->update();

	mPlayerShip->prepForBattle();
	mEnemyShip->prepForBattle();
}

void Game::update(float dt)
{
	if(mShutdown) {
		mShutdown = false;

		mGUI->shutdown();
		mPlatform->shutdown();

		Ogre::Root::getSingleton().queueEndRendering();
		Ogre::Root::getSingleton().shutdown();
		return;
	}

	if(mPaused) {
		return;
	}

	// update lense flare
	if(mLensFlare) {
		mLensFlare->Update();
	}

	if(mGameState <= GameStateMainMenu) {
		return;
	}

	mGameTimeRemaining -= dt;
	if(mGameTimeRemaining <= 0.0) {
		mGameTimeRemaining = 0.0;
		setGameState(GameStateEnd);
	}

	mInGameMenu->updateTimeLeft(mGameTimeRemaining);

	float timeLeft = mGameTimeRemaining / mMaxGameTime;
	float delta = (mDangerZoneEnd - mDangerZoneStart) * 2.0;
	float dangerZoneSize = mDangerZoneStart + (delta * (1.0 - timeLeft));

	mDangerZone->setDimensions(dangerZoneSize,dangerZoneSize * 2.0);
	mWarningZone->setDimensions(dangerZoneSize + mWarningZoneSize * 2.0,
			(dangerZoneSize * 2.0) + mWarningZoneSize * 2.0);

	if(mInBattle) {
		if(!mBattleDialogOpened) {
			mEnemyShip = new Ship(true);

			GameNode *n = mGameNodes[mCurrentNodeIdx];
			mEnemyShip->setSpecsForSector(n->sector);
			// give the ship random specs

			mInGameMenu->displayDialog(
					"COMBAT",
					"A hostile ship is approaching! Do we run or fight?\n\
					Enemy Ship Stats: \n\
					Shield Level: " + Ogre::StringConverter::toString(mEnemyShip->mShieldLevel) +
					"Weapon Level: " + Ogre::StringConverter::toString(mEnemyShip->mWeaponsLevel) +
					"Hull Level: " + Ogre::StringConverter::toString(mEnemyShip->mHullLevel)
					,
					"",
					"RUN",
					MyGUI::newDelegate(this,&Game::run),
					"FIGHT",
					MyGUI::newDelegate(this, &Game::fight),
					true
				);
			mBattleDialogOpened = true;
			mUpdateBattleStatsCooldown = 0;
		}

		if(mBattleStarted) {
			mPlayerShip->rechargeShields(dt);
			mEnemyShip->rechargeShields(dt);

			mEnemyShip->takeDamage(mPlayerShip->fire(dt));
			if(mEnemyShip->mHullStrength > 0.0) {
				mPlayerShip->takeDamage(mEnemyShip->fire(dt));

				if(mPlayerShip->mHullStrength <= 0.0) {
					mBattleStarted = false;
					setGameState(GameStateEnd);
				}
			}
			else {
				// enemy destroyed
				mBattleStarted = false;
				GameNode *n = mGameNodes[mCurrentNodeIdx];
				float reward = 500.0 * ((float)n->sector + 1.0);
				mPlayerMoney += reward;
				mInGameMenu->displayDialog(
						"COMBAT",
						"The hostile ship has been destroyed.\nYou have been rewarded " +
						Ogre::StringConverter::toString(reward),
						"",
						"",
						NULL,
						"CLOSE",
						MyGUI::newDelegate(this, &Game::closeBattleDialogPressed),
						true
					);
			}
			mUpdateBattleStatsCooldown -= dt;
			if(mUpdateBattleStatsCooldown <= 0) {
				mUpdateBattleStatsCooldown = 0.5;
				mInGameMenu->updateBattleStats();
			}
		}
	}

	/*
	mDangerZoneChildNode->setScale(
		dangerZoneSize * 0.01,
		1.0,
		dangerZoneSize * 2.0 * 0.01
		);
		*/
}

void Game::updateVisibleNodes()
{
	GameNode *curNode = mGameNodes[mCurrentNodeIdx];
	float range =mPlayerShip->maxJumpRangeForEngineLevel(mPlayerShip->mEngineLevel + 1);
	float squaredRange = range * range;

	Ogre::Vector3 curPos = curNode->scenenode->getPosition();

	for(unsigned int i = 0; i < mGameNodes.size(); i++) {
		GameNode *n = mGameNodes[i];
		if(n->scenenode->getPosition().squaredDistance(curPos) <= squaredRange) {
			n->visible = true;
			n->scenenode->setVisible(true);
		}
	}
}