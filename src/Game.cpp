#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// using pch
#include "stdafx.h"

#else

#include "Game.h"
#include "OgreUtil.h"
#include "GameAudioInterface.h"

#endif


static const Ogre::String gSpecialistTypes[5] = {
	"None",
	"Engine Systems",
	"Weapon Systems",
	"Shield Systems",
	"Hull Repair"
};

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
	playButtonClick(NULL);
	setGameState(GameStateMainMenu);
}

//-------------------------------------------------------------------------------------
void Game::closeBattleDialogPressed(MyGUI::WidgetPtr _sender)
{
	playButtonClick(NULL);
	mInGameMenu->closeDialog(NULL);

	mInBattle = false;

	delete mEnemyShip;
	mEnemyShip = 0;

	mPlayerShip->updateSpecs();
	mPlayerShip->prepForBattle();
	mInGameMenu->update();
}

//-------------------------------------------------------------------------------------
void Game::completeMissionAtCity(int idx, MissionType type)
{
	for(unsigned int i = 0; i < mMissions.size(); i++) {
		Mission *m = mMissions[i];
		if(m->type != type || m->objectiveNodeIdx != idx) {
			continue;
		}
		if(!m->objectiveInCity) continue;

		if(m->status == MissionStatusAccepted) {
			m->status = MissionStatusCompleted;
			break;
		}
	}
}

//-------------------------------------------------------------------------------------
void Game::completeMissionAtStation(int idx, MissionType type)
{
	for(unsigned int i = 0; i < mMissions.size(); i++) {
		Mission *m = mMissions[i];
		if(m->type != type || m->objectiveNodeIdx != idx) {
			continue;
		}
		if(!m->objectiveInStation) continue;

		if(m->status == MissionStatusAccepted) {
			m->status = MissionStatusCompleted;
			break;
		}
	}
}

//-------------------------------------------------------------------------------------
void Game::completeMissionAtNode(int idx, MissionType type)
{
	for(unsigned int i = 0; i < mMissions.size(); i++) {
		Mission *m = mMissions[i];
		if(m->type != type || m->objectiveNodeIdx != idx) {
			continue;
		}
		if(m->objectiveInCity || m->objectiveInStation) {
			continue;
		}

		if(m->status == MissionStatusAccepted) {
			m->status = MissionStatusCompleted;
			if(type == MissionTypeDestroyEnemy) {
				mGameNodes[idx]->hasHostileShip = false;
			}

			break;
		}
	}
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
			n->planet = NULL;
			n->visible = false;
			n->hasCity = false;
			n->hasStation = false;
			n->playerHasMap = false;
			n->mapRequired = false;
			n->hasHostileShip = false;

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
				// second node in sector is always hidden (requires a map)
				if(i == 1) {
					n->mapRequired = true;
				}

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
			c.mission.type = MissionTypeNone;
			c.mission.status = MissionStatusDefault;
			c.imageName = "";
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

	/*
	const MyGUI::IntSize size(mRenderWindow->getWidth(), mRenderWindow->getHeight());
	mTestImageBox = MyGUI::Gui::getInstance().createWidget<MyGUI::ImageBox>("ImageBox",MyGUI::IntCoord(0,0, size.width, size.height), MyGUI::Align::Default, "Back");
	mTestImageBox->setImageTexture
	*/
//		mJumpRange = mMainWidget->createWidget<MyGUI::ImageBox>("ImageBox", MyGUI::IntCoord(0,0, size.width, size.height), MyGUI::Align::Default);
	//mJumpRange->setImageTexture("white-circle.png");

	//mSceneManager->setSkyBox(true, "Spacescape1024");
	mSceneManager->setSkyBox(true, "Spacescape2048");
	
	// randomly rotate the skybox
	Ogre::SceneNode *n = mSceneManager->getSkyBoxNode();
	n->yaw(Ogre::Radian(Ogre::Math::RangeRandom(0.0,Ogre::Math::TWO_PI)));
	n->pitch(Ogre::Radian(Ogre::Math::RangeRandom(0.0,Ogre::Math::TWO_PI)));
	n->roll(Ogre::Radian(Ogre::Math::RangeRandom(0.0,Ogre::Math::TWO_PI)));

	playBackgroundMusic("music.ogg");


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
	/*
	Ogre::Entity *sun = Game::getSingleton().mSceneManager->createEntity(Ogre::SceneManager::PT_SPHERE);
	mSunSceneNode->attachObject(sun);
	sun->setCastShadows(false);
	*/
	mSceneManager->setAmbientLight(Ogre::ColourValue(0.01f, 0.01f, 0.05f));
	//mSceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	Ogre::Light *l = mSceneManager->createLight();
	l->setType(Ogre::Light::LT_POINT);
	l->setDiffuseColour(1.0, 1.0, 1.0);
	l->setPosition(mDangerZoneNode->getPosition() + Ogre::Vector3(300,0,0));
	l->setLightMask(1 << 0);
	//mSunSceneNode->attachObject(l);

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

	mPlayerShip = new Ship();
	/*
	mPlayerShip->mSceneNode->setPosition(mCamera->getPosition() +
		Ogre::Vector3(-20.0,0,0));
	mPlayerShip->mSceneNode->setScale(0.7f,0.7f,0.7f);
	mPlayerShip->mSceneNode->yaw(Ogre::Degree(30.f));
	mPlayerShip->mSceneNode->pitch(Ogre::Degree(30.f));
	*/
	setGameState(GameStateMainMenu);

	mBokehOverlay = new Ogre::Rectangle2D(true);
	mBokehOverlay->setCorners(-1.0f, 1.0f, 1.0f, -1.0f);
	mBokehOverlay->setMaterial("OverlayBokeh");
	mBokehOverlay->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY - 1);
	mBokehOverlay->setBoundingBox(Ogre::AxisAlignedBox(-100000.0*Ogre::Vector3::UNIT_SCALE, 100000.0*Ogre::Vector3::UNIT_SCALE));
	Ogre::SceneNode* node =mSceneManager->getRootSceneNode()->createChildSceneNode();
	node->attachObject(mBokehOverlay);

	mLinesOverlay = new Ogre::Rectangle2D(true);
	mLinesOverlay->setCorners(-1.0f, 1.0f, 1.0f, -1.0f);
	mLinesOverlay->setMaterial("OverlayLines");
	mLinesOverlay->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY - 1);
	mLinesOverlay->setBoundingBox(Ogre::AxisAlignedBox(-100000.0*Ogre::Vector3::UNIT_SCALE, 100000.0*Ogre::Vector3::UNIT_SCALE));
	node->attachObject(mLinesOverlay);

	mShadowOverlay = new Ogre::Rectangle2D(true);
	mShadowOverlay->setCorners(-1.0f, 1.0f, 1.0f, -1.0f);
	mShadowOverlay->setMaterial("OverlayShadow");
	mShadowOverlay->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY - 1);
	mShadowOverlay->setBoundingBox(Ogre::AxisAlignedBox(-100000.0*Ogre::Vector3::UNIT_SCALE, 100000.0*Ogre::Vector3::UNIT_SCALE));
	node->attachObject(mShadowOverlay);
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
		completeMissionAtStation(mCurrentNodeIdx,MissionTypeDeliver);
	}
}

void Game::land()
{
	if(mGameState != GameStateCity) {
		setGameState(GameStateCity);
		completeMissionAtCity(mCurrentNodeIdx,MissionTypeDeliver);
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

	mPassengers.clear();
	mMissions.clear();

	mPlayerMoney =  Ogre::StringConverter::parseReal(
		mConfig->getSetting("startFunds","","250.0"));

	// randomly rotate the skybox
	Ogre::SceneNode *n = mSceneManager->getSkyBoxNode();
	n->yaw(Ogre::Radian(Ogre::Math::RangeRandom(0.0,Ogre::Math::TWO_PI)));
	n->pitch(Ogre::Radian(Ogre::Math::RangeRandom(0.0,Ogre::Math::TWO_PI)));
	n->roll(Ogre::Radian(Ogre::Math::RangeRandom(0.0,Ogre::Math::TWO_PI)));

	mPlayerShip->reset();

	int numSectors = Ogre::StringConverter::parseInt(
		mConfig->getSetting("numSectors","","3"));
	int numNodesPerSector = Ogre::StringConverter::parseInt(
		mConfig->getSetting("numNodesPerSector","","4"));

	createGameNodes(numSectors,numNodesPerSector);
	updateContacts();

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

	mInGameMenu->updateBattleStats();
	mInGameMenu->openHelp(NULL);
}

void Game::quit()
{
	setGameState(GameStateMainMenu);
}

void Game::setGameState(GameState state)
{
	mPrevGameState = mGameState;
	mGameState = state;

	mLensFlare->SetFlareVisible(false);
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

			mPlayerShip->mSceneNode->setVisible(true,false);
			mPlayerShip->mSceneNode->getParentSceneNode()->removeChild(
				mPlayerShip->mSceneNode);
			mSceneManager->getRootSceneNode()->addChild(
				mPlayerShip->mSceneNode);
			mPlayerShip->mSceneNode->setOrientation(Ogre::Quaternion::IDENTITY);
			mPlayerShip->mSceneNode->setPosition(mSunCamPosition +
				Ogre::Vector3(-20.0,0,0));
			mPlayerShip->mLaserLight->setVisible(false);
			mPlayerShip->mLaser->mSceneNode->setVisible(false);
			mPlayerShip->mSceneNode->setScale(0.7f,0.7f,0.7f);
			mPlayerShip->mSceneNode->yaw(Ogre::Degree(30.f));
			mPlayerShip->mSceneNode->pitch(Ogre::Degree(30.f));

			break;
		case GameStateEnd:
			{
				if(mPlayerShip->mHullStrength <= 0) {
					mInGameMenu->displayDialog("Game Over",
						"Your ship was destroyed",
						"game-over.png", 
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
						mInGameMenu->displayDialog("Game Over","You Survived!","victory.png",
							"",
							NULL,
							"CLOSE",
							MyGUI::newDelegate(this,&Game::closeEndGameDialogPressed));
					}
					else {
						mInGameMenu->displayDialog("Game Over","Your didn't make it","game-over.png", 
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

	mCamera->detachFromParent();

	if(visible) {
		mCamera->setFixedYawAxis(true,-Ogre::Vector3::UNIT_Z);
		mCamera->setPosition(mNavCamPosition);
		mCamera->lookAt(Ogre::Vector3(mNavCamPosition.x,0,mNavCamPosition.z));
		mPlayerShip->mSceneNode->setVisible(false);
	}
	else {
		if(mGameState > GameStateMainMenu) {
			Ogre::Vector3 nodePos = mGameNodes[mCurrentNodeIdx]->scenenode->getPosition();
			Ogre::Vector3 dir = mDangerZoneNode->getPosition() - nodePos;
			dir.normalise();

			mPlayerShip->mSceneNode->setVisible(true,false);
			if(mGameNodes[mCurrentNodeIdx]->planet) {
				mPlayerShip->mSceneNode->getParentSceneNode()->removeChild(mPlayerShip->mSceneNode);
				mGameNodes[mCurrentNodeIdx]->planet->mSceneNode->addChild(mPlayerShip->mSceneNode);
				mPlayerShip->mSceneNode->setOrientation(Ogre::Quaternion::IDENTITY);
				mPlayerShip->mSceneNode->setPosition(-5.0,0.5,2.75);
				mPlayerShip->mSceneNode->setScale(0.1f,0.1f,0.1f);
				mPlayerShip->mSceneNode->yaw(Ogre::Degree(90.0));

				mGameNodes[mCurrentNodeIdx]->planet->mSceneNode->attachObject(mCamera);

				mCamera->setPosition(-10.0,0.5,1.0);
				mCamera->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);
				mCamera->lookAt(nodePos);
			}
			else {
				mPlayerShip->mSceneNode->getParentSceneNode()->removeChild(mPlayerShip->mSceneNode);
				mGameNodes[mCurrentNodeIdx]->scenenode->addChild(mPlayerShip->mSceneNode);
				mPlayerShip->mSceneNode->setOrientation(Ogre::Quaternion::IDENTITY);
				mPlayerShip->mSceneNode->setPosition(0.0,0.5,-2.75);
				mPlayerShip->mSceneNode->setScale(0.1f,0.1f,0.1f);
				mPlayerShip->mSceneNode->yaw(Ogre::Degree(-90.0));

				float offset = 10.0f;
				Ogre::Vector3 camPos = nodePos - dir * offset;
				Ogre::Vector3 xDir = dir.crossProduct(Ogre::Vector3::UNIT_Y);
				camPos += xDir * offset * 0.5f;
				camPos += Ogre::Vector3::UNIT_Y * 2.0f;
				mCamera->setPosition(camPos);
				mCamera->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);
				mCamera->lookAt(nodePos);
			}
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

	if(mGameNodes[mCurrentNodeIdx]->hasHostileShip ||
		Ogre::Math::UnitRandom() > 0.4) {
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

		if(mPlayerShip) {
			delete mPlayerShip;
			mPlayerShip = 0;
		}

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
		if(mGameState != GameStateEnd) {
			setGameState(GameStateEnd);
		}
	}

	mInGameMenu->updateTimeLeft(mGameTimeRemaining);

	float timeLeft = mGameTimeRemaining / mMaxGameTime;
	float delta = (mDangerZoneEnd - mDangerZoneStart) * 2.0;
	float dangerZoneSize = mDangerZoneStart + (delta * (1.0 - timeLeft));

	mDangerZone->setDimensions(dangerZoneSize,dangerZoneSize * 2.0);
	mWarningZone->setDimensions(dangerZoneSize + mWarningZoneSize * 2.0,
			(dangerZoneSize * 2.0) + mWarningZoneSize * 2.0);

	if(mGameState > GameStateMainMenu) {
		// make the planet rotate
		GameNode *n = mGameNodes[mCurrentNodeIdx];
		if(n->planet) {
			n->planet->update(dt);
		}
	}

	if(mInBattle) {
		if(!mBattleDialogOpened) {
			mEnemyShip = new Ship(true);

			GameNode *n = mGameNodes[mCurrentNodeIdx];
			mEnemyShip->setSpecsForSector(n->sector);
			// give the ship random specs

			mInGameMenu->displayDialog(
					"COMBAT",
					"A hostile ship is approaching! Do we run or fight?\nEnemy Ship Stats: \nShield Level: " + 
					Ogre::StringConverter::toString(mEnemyShip->mShieldLevel) +
					"\nWeapon Level: " + Ogre::StringConverter::toString(mEnemyShip->mWeaponsLevel) +
					"\nHull Level: " + Ogre::StringConverter::toString(mEnemyShip->mHullLevel)
					,
					"warning.png",
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
					playEffect("explosion.ogg");
					setGameState(GameStateEnd);
				}
			}
			else {
				// enemy destroyed
				mBattleStarted = false;
				GameNode *n = mGameNodes[mCurrentNodeIdx];
				float reward = 500.0 * ((float)n->sector + 1.0);
				mPlayerMoney += reward;
				playEffect("explosion.ogg");
				// did this fulfill a mission?
				if(n->hasHostileShip) {
					// does the player have the mission?
					completeMissionAtNode(mCurrentNodeIdx,MissionTypeDestroyEnemy);
				}

				mInGameMenu->displayDialog(
						"COMBAT",
						"The hostile ship has been destroyed.\nYou have been rewarded " +
						Ogre::StringConverter::toString(reward),
						"victory.png",
						"",
						NULL,
						"CLOSE",
						MyGUI::newDelegate(this, &Game::closeBattleDialogPressed),
						true
					);
				delete mEnemyShip;
				mEnemyShip = 0;
			}
			mUpdateBattleStatsCooldown -= dt;
			if(mUpdateBattleStatsCooldown <= 0) {
				mUpdateBattleStatsCooldown = 0.5;
				mInGameMenu->updateBattleStats();
			}
		}
	}

	if(mPlayerShip) {
		mPlayerShip->update(dt);
	}
	if(mEnemyShip) {
		mEnemyShip->update(dt);
	}
}

int hiddenNodeInSector(std::vector<GameNode *> *nodes, int sector) {
	for(unsigned int i = 0; i < nodes->size(); i++) {
		if(nodes->at(i)->sector == sector &&
			nodes->at(i)->mapRequired) {
			return i;
		}
	}
	return -1;
}

bool missionTypeExistsInSector(std::vector<GameNode *> *nodes, int sector, MissionType type) {
	for(unsigned int i = 0; i < nodes->size(); i++) {
		GameNode *n = nodes->at(i);
		if(n->sector == sector) {
			if(n->hasCity) {
				for(unsigned int j = 0; j < n->cityContacts.size(); j++) {
					if(n->cityContacts[j].mission.type == type) {
						return true;
					}
				}
			}
			if(n->hasStation) {
				for(unsigned int j = 0; j < n->stationContacts.size(); j++) {
					if(n->stationContacts[j].mission.type == type) {
						return true;
					}
				}
			}
		}
	}
	return false;
}

int randomNodeInSector(std::vector<GameNode *> *nodes, int nodeIdx)
{
	int sector = nodes->at(nodeIdx)->sector;

	std::vector<int>possibleNodes;
	for(unsigned int i = 0; i < nodes->size(); i++) {
		if(i == nodeIdx) continue;

		GameNode *n = nodes->at(i);
		if(n->sector == sector) {
			possibleNodes.push_back(i);
		}
	}
	std::random_shuffle(possibleNodes.begin(), possibleNodes.end());

	if(possibleNodes.size()) {
		return possibleNodes[0];
	}
	else {
		return -1;
	}
}

void createMapMissionForContact(std::vector<GameNode *> *nodes, int nodeIdx, 
	Contact *c) 
{
	int sector = nodes->at(nodeIdx)->sector;
	c->mission.type = MissionTypeMap;
	c->mission.objectiveNodeIdx = hiddenNodeInSector(nodes,sector);
	c->mission.cost = floorf(Ogre::Math::RangeRandom(100,300));
	c->mission.reward = 0;
	c->mission.objective = "I'm the only one who knows the nav coords for " +
		nodes->at(c->mission.objectiveNodeIdx)->title + ".\nI'll sell them to you for " +
		Ogre::StringConverter::toString(c->mission.cost) + "!";
	c->mission.successMessage = "Congrats! You now know the secret nav coords for " +
		nodes->at(c->mission.objectiveNodeIdx)->title;
}

void createDeliverMissionForContact(std::vector<GameNode *> *nodes, int nodeIdx, 
	Contact *c) 
{
	int sector = nodes->at(nodeIdx)->sector;
	c->mission.type = MissionTypeDeliver;
	c->mission.objectiveNodeIdx = randomNodeInSector(nodes,nodeIdx);

	GameNode *n = nodes->at(c->mission.objectiveNodeIdx);
	Ogre::String location;
	
	c->mission.objectiveInStation = false;
	c->mission.objectiveInCity = false;

	if(n->hasCity && n->hasStation) {
		float r = Ogre::Math::UnitRandom();
		if (r < 0.5) {
			c->mission.objectiveInCity = true;
			location = n->cityName; 
		}
		else {
			c->mission.objectiveInStation = true;
			location = n->stationName;
		}
	}
	else if(n->hasCity) {
		c->mission.objectiveInCity = true;
		location = n->cityName;
	}
	else if(n->hasStation) {
		c->mission.objectiveInStation = true;
		location = n->stationName;
	}

	c->mission.cost = 0;
	c->mission.reward = floorf(Ogre::Math::RangeRandom(100,300));
	
	location += " in the " +  n->title + " system";
	c->mission.objective = "I'm looking for someone to deliver some cargo to " + 
		location + ".\nI'll gladly pay " +
		Ogre::StringConverter::toString(c->mission.reward) + ".";

	c->mission.waitingMessage = "Deliver my cargo to " +
		nodes->at(c->mission.objectiveNodeIdx)->title + " and I'll give you " +
		Ogre::StringConverter::toString(c->mission.reward) + ".";
	c->mission.successMessage = "Thanks for delivering my cargo!  \nI'll let you know if I have other shipping needs.";
}

void createDestroyMissionForContact(std::vector<GameNode *> *nodes, int nodeIdx, 
	Contact *c) 
{
	int sector = nodes->at(nodeIdx)->sector;
	c->mission.type = MissionTypeDestroyEnemy;
	c->mission.objectiveNodeIdx = randomNodeInSector(nodes,nodeIdx);
	c->mission.objectiveInCity = false;
	c->mission.objectiveInStation = false;
	c->mission.upgradeType = MissionUpgradeTypeNone;

	GameNode *n = nodes->at(c->mission.objectiveNodeIdx);
	n->hasHostileShip = true;
	c->mission.cost = 0;
	c->mission.reward = floorf(Ogre::Math::RangeRandom(100,300)) * (float)(sector + 1.0);
	c->mission.objective = nodes->at(c->mission.objectiveNodeIdx)->title + 
		" needs help eliminating a hostile ship that has been attacking all inbound vessels!\n\nThey'll gladly pay you " +
		Ogre::StringConverter::toString(c->mission.reward) + " if you succeed.";
	c->mission.waitingMessage = nodes->at(c->mission.objectiveNodeIdx)->title + 
		" awaits your help eliminating a hostile ship that has been attacking all inbound vessels!\n\nThey'll gladly pay you " +
		Ogre::StringConverter::toString(c->mission.reward) + " if you succeed.";
	c->mission.successMessage = "The council of " +
		nodes->at(c->mission.objectiveNodeIdx)->title + " thanks you for your service and protection.";
}

void createPassengerMissionForContact(std::vector<GameNode *> *nodes, int nodeIdx, 
	Contact *c) 
{
	int sector = nodes->at(nodeIdx)->sector;
	c->mission.type = MissionTypePassenger;
	c->mission.objectiveNodeIdx = nodeIdx;

	c->mission.cost = 0;
	c->mission.reward = 0;
	if(Ogre::Math::UnitRandom() > 0.5) {
		c->mission.reward = floorf(Ogre::Math::RangeRandom(0,300));
	}
	c->mission.upgradeType = (MissionUpgradeType)(int)floorf(
		Ogre::Math::RangeRandom(0,3.99f));

	c->mission.objective = "I heard you talking about the sun collapsing.\n I want to join you on your ship and I'll help out however I can!";
	c->mission.successMessage = "Thank you!\n";

	if(c->mission.upgradeType > MissionUpgradeTypeNone) {
		Ogre::String specialistType;
		c->mission.successMessage += "\nBy the way, did I mention I'm a " +
			gSpecialistTypes[c->mission.upgradeType] + 
			" specialist?\nI'll help out however I can.\n";
	}

	if(c->mission.reward > 0) {
		c->mission.successMessage += "\nHere's " +
			Ogre::StringConverter::toString(c->mission.reward) + " for your trouble.";
	}
}

void createMissionForContact(std::vector<GameNode *> *nodes, int nodeIdx, 
	Contact *c, bool firstNodeInSector)
{
	int sector = nodes->at(nodeIdx)->sector;

	// each node needs to have a hidden map quest
	if(firstNodeInSector && !missionTypeExistsInSector(nodes,sector, MissionTypeMap)) {
		createMapMissionForContact(nodes,nodeIdx,c);
		return;
	}

	if(!missionTypeExistsInSector(nodes,sector,MissionTypeDeliver)) {
		createDeliverMissionForContact(nodes,nodeIdx,c);
		return;
	}

	if(!missionTypeExistsInSector(nodes,sector,MissionTypeDestroyEnemy)) {
		createDestroyMissionForContact(nodes,nodeIdx,c);
		return;
	}

	if(!missionTypeExistsInSector(nodes,sector,MissionTypePassenger)) {
		createPassengerMissionForContact(nodes,nodeIdx,c);
		return;
	}
	
	// randomly create a mission
	float r = Ogre::Math::UnitRandom();
	if(r < 0.5) {
		createDeliverMissionForContact(nodes,nodeIdx,c);
	}
	else {
		createPassengerMissionForContact(nodes,nodeIdx,c);
	}
}

void Game::updateContacts()
{
	int sector = -1;
	for(unsigned int i = 0; i < mGameNodes.size(); i++) {
		GameNode *n = mGameNodes[i];

		bool firstNodeInSector = n->sector != sector;

		if(firstNodeInSector) {
			sector = n->sector;
		}

		if(n->hasCity) {
			int numContactsToCreate = 2;
			for(unsigned int j = 0; j < n->cityContacts.size(); j++) {
				Contact *c = &(n->cityContacts[j]);

				if(c->mission.status == MissionTypeNone) {
					numContactsToCreate--;
					// create the mission
					createMissionForContact(&mGameNodes, i, c, firstNodeInSector);
				}
				else if(c->mission.status != MissionStatusTurnedIn) {
					numContactsToCreate--;
				}
			}

			if(numContactsToCreate > 0) {
				// @TODO create new contacts
			}
		}

		if(n->hasStation) {
			int numContactsToCreate = 2;
			for(unsigned int j = 0; j < n->stationContacts.size(); j++) {
				Contact *c = &(n->stationContacts[j]);

				if(c->mission.status == MissionTypeNone) {
					numContactsToCreate--;
					// create the mission
					createMissionForContact(&mGameNodes, i, c, firstNodeInSector);
				}
				else if(c->mission.status != MissionStatusTurnedIn) {
					numContactsToCreate--;
				}
			}

			if(numContactsToCreate > 0) {
				// @TODO create new contacts
			}
		}
	}
}

void Game::updateVisibleNodes()
{
	GameNode *curNode = mGameNodes[mCurrentNodeIdx];
	float range =mPlayerShip->maxJumpRangeForEngineLevel(mPlayerShip->mEngineLevel + 1);
	float squaredRange = range * range;

	Ogre::Vector3 curPos = curNode->scenenode->getPosition();

	for(unsigned int i = 0; i < mGameNodes.size(); i++) {
		GameNode *n = mGameNodes[i];
		// some nodes require a map
		if(n->mapRequired && !n->playerHasMap) {
			n->visible = false;
			n->scenenode->setVisible(false);
		}
		else if(n->scenenode->getPosition().squaredDistance(curPos) <= squaredRange ||
			n->playerHasMap) {
			n->visible = true;
			n->scenenode->setVisible(true);
		}
	}
}