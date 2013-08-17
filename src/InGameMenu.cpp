#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// using pch
#include "stdafx.h"

#else

#include "InGameMenu.h"
#include "GameAudioInterface.h"
#include "Game.h"

#endif

namespace Salvation
{

	InGameMenu::InGameMenu(MyGUI::Widget* _parent)
	{
		initialiseByAttributes(this, _parent);

		mQuitButton->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::quit);
		mQuitButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);
		mQuitButton->eventMouseButtonClick += MyGUI::newDelegate(playButtonClick);


		mSettingsButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);
		mSettingsButton->eventMouseButtonClick += MyGUI::newDelegate(playButtonClick);
		mSettingsButton->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::openSettings);
		mSettingsButton->setVisible(false);

		setVisible(false);

		// settings
		mSettingsWindow->setVisible(false);
		mSettingsWindow->eventWindowButtonPressed += MyGUI::newDelegate(this, &Salvation::InGameMenu::closeSettings);

		mShowNavGrid->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::toggleShowNavGrid);
		mShowNavGrid->setStateSelected(Game::getSingleton().mShowGrid);

		mMaxGameTime->eventEditTextChange += MyGUI::newDelegate(this, &Salvation::InGameMenu::updateMaxGameTime);
		mMaxGameTime->setCaption(Ogre::StringConverter::toString((float)Game::getSingleton().mMaxGameTime));

		mReloadConfigButton->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::reloadConfig);
		mReloadConfigButton->eventMouseButtonClick += MyGUI::newDelegate(playButtonClick);

		// nav
		mCloseNavButton->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::closeNav);
		mCloseNavButton->eventMouseButtonClick += MyGUI::newDelegate(playButtonClick);
		mCloseNavButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);
		mDepartButton->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::depart);
		mDepartButton->eventMouseButtonClick += MyGUI::newDelegate(playButtonClick);
		mDepartButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);

		mLandedNavMapButton->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::openNav);
		mLandedNavMapButton->eventMouseButtonClick += MyGUI::newDelegate(playButtonClick);
		mLandedNavMapButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);
		mNavButton->eventMouseButtonClick  += MyGUI::newDelegate(this, &Salvation::InGameMenu::openNav);
		mNavButton->eventMouseButtonClick += MyGUI::newDelegate(playButtonClick);
		mNavButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);

		mLandButton->eventMouseButtonClick  += MyGUI::newDelegate(this, &Salvation::InGameMenu::land);
		mLandButton->eventMouseButtonClick += MyGUI::newDelegate(playButtonClick);
		mLandButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);
		mDockButton->eventMouseButtonClick  += MyGUI::newDelegate(this, &Salvation::InGameMenu::dock);
		mDockButton->eventMouseButtonClick += MyGUI::newDelegate(playButtonClick);
		mDockButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);

		// STORE
		mStoreButton->eventMouseButtonClick  += MyGUI::newDelegate(this, &Salvation::InGameMenu::openStore);
		mStoreButton->eventMouseButtonClick  += MyGUI::newDelegate(playButtonClick);
		mStoreButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);

		mBuyCheckBox1Button->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::toggleBuyCheckbox);
		mBuyCheckBox2Button->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::toggleBuyCheckbox);
		mBuyCheckBox3Button->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::toggleBuyCheckbox);
		mBuyCheckBox4Button->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::toggleBuyCheckbox);

		mStoreWindowWindow->eventWindowButtonPressed += MyGUI::newDelegate(this, &Salvation::InGameMenu::closeStore);
		mFuelUpButton->eventMouseButtonClick  += MyGUI::newDelegate(this, &Salvation::InGameMenu::fuelUp);
		mFuelDownButton->eventMouseButtonClick  += MyGUI::newDelegate(this, &Salvation::InGameMenu::fuelDown);
		mBuyButton->eventMouseButtonClick  += MyGUI::newDelegate(this, &Salvation::InGameMenu::buy);
		mBuyButton->eventMouseButtonClick  += MyGUI::newDelegate(playButtonClick);
		mBuyButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);

		mRepairCheckBox1Button->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::toggleRepairCheckbox);
		mRepairCheckBox2Button->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::toggleRepairCheckbox);
		mRepairCheckBox3Button->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::toggleRepairCheckbox);
		mRepairCheckBox4Button->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::toggleRepairCheckbox);

		mRepairButton->eventMouseButtonClick  += MyGUI::newDelegate(this, &Salvation::InGameMenu::repair);
		mRepairButton->eventMouseButtonClick  += MyGUI::newDelegate(playButtonClick);
		mRepairButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);

		// battle
		mRunButton->eventMouseButtonClick  += MyGUI::newDelegate(this, &Salvation::InGameMenu::run);
		mRunButton->eventMouseButtonClick  += MyGUI::newDelegate(playButtonClick);
		mRunButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);

		// contacts
		mContactsButton1Button->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::contactButtonPressed);
		mContactsButton1Button->eventMouseButtonClick  += MyGUI::newDelegate(playButtonClick);
		mContactsButton1Button->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);

		mContactsButton2Button->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::contactButtonPressed);
		mContactsButton2Button->eventMouseButtonClick  += MyGUI::newDelegate(playButtonClick);
		mContactsButton2Button->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);
		/*
		const MyGUI::IntSize size(200, 200);
		mJumpRange = MyGUI::Gui::getInstance().createWidget<MyGUI::ImageBox>("ImageBox",MyGUI::IntCoord(0,0, size.width, size.height), MyGUI::Align::Default, "Back");
//		mJumpRange = mMainWidget->createWidget<MyGUI::ImageBox>("ImageBox", MyGUI::IntCoord(0,0, size.width, size.height), MyGUI::Align::Default);
		mJumpRange->setImageTexture("white-circle.png");
		*/

		mHelpWindow->eventWindowButtonPressed += MyGUI::newDelegate(this, &Salvation::InGameMenu::closeHelp);

		mHelpButton->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::openHelp);
		mHelpButton->eventMouseButtonClick  += MyGUI::newDelegate(playButtonClick);
		mHelpButton->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);
	}

	InGameMenu::~InGameMenu()
	{
	}

	void InGameMenu::closeHelp(MyGUI::Window* _sender, const std::string& _name)
	{
		mHelpWindow->setVisibleSmooth(false);
		playButtonClick(_sender);
	}

	void InGameMenu::openHelp(MyGUI::WidgetPtr _sender)
	{
		mHelpWindow->setVisibleSmooth(true);
	}

	void InGameMenu::acceptMission(MyGUI::WidgetPtr _sender)
	{
		Contact *c = *(_sender->getUserData<Contact *>());

		if(c->mission.type == MissionTypePassenger) {
			c->mission.status = MissionStatusTurnedIn;
			c->isPassenger = true;

			if(c->mission.reward > 0) {
				Game::getSingleton().mPlayerMoney += c->mission.reward;
			}

			Game::getSingleton().mPassengers.push_back(c);
		}
		else if(c->mission.type == MissionTypeMap) {
			if(Game::getSingleton().mPlayerMoney < c->mission.cost) {
				playEffect("error.ogg");

			}
			else {
				c->mission.status = MissionStatusTurnedIn;
				Game::getSingleton().mPlayerMoney -= c->mission.cost;
				GameNode *n = Game::getSingleton().mGameNodes[c->mission.objectiveNodeIdx];
				n->playerHasMap = true;
				n->visible = true;
				n->scenenode->setVisible(true);
				update();
			}
		}
		else {
			c->mission.status = MissionStatusAccepted;
		}

		Mission *m = &c->mission;
		Game::getSingleton().mMissions.push_back(m);

		closeDialog(NULL);
	}

	void InGameMenu::buy(MyGUI::WidgetPtr _sender)
	{
		updateBuyCost();

		// do we have enough money for this?
		float cost = *(mBuyCostTextBox->getUserData<float>());
		if(cost < 0) cost = 0;

		if(cost <= 0 || cost > Game::getSingleton().mPlayerMoney) {
			playEffect("error.ogg");
			return;
		}

		playButtonClick(_sender);

		int fuel = Ogre::StringConverter::parseInt(mFuelQty->getCaption());
		if(fuel < 0) fuel = 0;

		Ship *playerShip = Game::getSingleton().mPlayerShip;

		playerShip->mFuel += fuel;

		if(mBuyCheckBox1Button->getStateSelected()) {
			playerShip->mEngineLevel++;
		}
		if(mBuyCheckBox2Button->getStateSelected()) {
			playerShip->mShieldLevel++;
		}
		if(mBuyCheckBox3Button->getStateSelected()) {
			playerShip->mHullLevel++;
		}
		if(mBuyCheckBox4Button->getStateSelected()) {
			playerShip->mWeaponsLevel++;
		}

		playerShip->updateSpecs();
		Game::getSingleton().mPlayerMoney -= cost;
		
		// sanity check for rounding issues
		if(Game::getSingleton().mPlayerMoney < 0) {
			Game::getSingleton().mPlayerMoney = 0;
		}

		resetBuyMenu();

		// credits
		mMoneyTextBoxTextBox->setCaption("Credits: " + 
			Ogre::StringConverter::toString(Game::getSingleton().mPlayerMoney));

		// fuel
		mFuelTextBox->setCaption("Fuel: " +
			Ogre::StringConverter::toString(Game::getSingleton().mPlayerShip->mFuel));

		// update stats
		update();
		updateBattleStats();
	}

	void InGameMenu::closeDialog(MyGUI::WidgetPtr _sender)
	{
		mDialogWindowWindow->setVisibleSmooth(false);
		mPopupPanel->setVisible(false);
		playButtonClick(_sender);
	}

	void InGameMenu::closeNav(MyGUI::WidgetPtr _sender)
	{
		Game::getSingleton().setNavVisible(false);
		update();
	}

	void InGameMenu::displayDialog(Ogre::String title, 
			Ogre::String message, 
			Ogre::String imageName,
			Ogre::String action1Name,
			MyGUI::delegates::CDelegate1<MyGUI::WidgetPtr>::IDelegate *action1Delegate,
			Ogre::String action2Name,
			MyGUI::delegates::CDelegate1<MyGUI::WidgetPtr>::IDelegate *action2Delegate,
			bool modal
			)
	{
		mDialogTextBox->setCaption(title + "\n" + message);
		if(action1Delegate) {
			mAction1Button->setCaption(action1Name);
			mAction1Button->eventMouseButtonClick.clear();
			mAction1Button->eventMouseButtonClick += action1Delegate;
			mAction1Button->setVisible(true);
		}
		else {
			mAction1Button->setVisible(false);
		}

		if(action2Delegate) {
			mOkButton->setCaption(action2Name);
			mOkButton->eventMouseButtonClick.clear();
			mOkButton->eventMouseButtonClick += action2Delegate;
			mOkButton->setVisible(true);
		}
		else {
			mOkButton->setVisible(false);
		}

		if(imageName.size()) {
			mDialogImageImageBox->setImageTexture(imageName);
			mDialogImageImageBox->setVisible(true);
		}
		else {
			mDialogImageImageBox->setVisible(false);
		}

		if(modal) {
			mPopupPanel->setVisible(true); 
		}
		
		mDialogWindowWindow->setVisibleSmooth(true);
	}

	void InGameMenu::dock(MyGUI::WidgetPtr _sender)
	{
		Game::getSingleton().dock();
	}

	void InGameMenu::fuelUp(MyGUI::WidgetPtr _sender)
	{
		int fuel = Ogre::StringConverter::parseInt(
			mFuelQty->getCaption());
		if(fuel < 0) fuel = 0;
		fuel++;

		playButtonClick(_sender);

		mFuelQty->setCaption(Ogre::StringConverter::toString(fuel));
		float fuelCost = (float)fuel * 100.0;
		mFuelCost->setCaption("Fuel Cost: " + 
			Ogre::StringConverter::toString(fuelCost));
		updateBuyCost();
	}

	void InGameMenu::fuelDown(MyGUI::WidgetPtr _sender)
	{
		int fuel = Ogre::StringConverter::parseInt(
			mFuelQty->getCaption());
		fuel--;
		if(fuel < 0) {
			playEffect("error.ogg");
			fuel = 0;
		}
		else {
			playButtonClick(_sender);
		}

		mFuelQty->setCaption(Ogre::StringConverter::toString(fuel));
		float fuelCost = (float)fuel * 100.0;
		mFuelCost->setCaption("Fuel Cost: " + 
			Ogre::StringConverter::toString(fuelCost));
		updateBuyCost();
	}

	void InGameMenu::run(MyGUI::WidgetPtr _sender)
	{
		Game::getSingleton().run(NULL);
		update();
	}

	/**
	* This little snippet gets the screenspace coordinates for a MovableObject
	*
	* @param   object   The object to retrieve the coordidnates of.
	* @param   camera   The active camera
	* @param   result   The Vector2 to store the result in
	*
	* @return   Returns true if the object is visible and the coordinates were
	*         retrieved, false otherwise.
	*/
	bool InGameMenu::getScreenspaceCoords(Ogre::SceneNode* node, Ogre::Camera* camera, Ogre::Vector2& result)
	{
	   if(!node->isInSceneGraph())
		  return false;
 
	   Ogre::Vector3 point = node->getPosition();
	
	   // Is the camera facing that point? If not, return false
	   Ogre::Plane cameraPlane = Ogre::Plane(Ogre::Vector3(camera->getDerivedOrientation().zAxis()), camera->getDerivedPosition());
	   if(cameraPlane.getSide(point) != Ogre::Plane::NEGATIVE_SIDE)
		  return false;
 
	   // Transform the 3D point into screen space
	   point = camera->getProjectionMatrix() * (camera->getViewMatrix() * point);
 
	   // Transform from coordinate space [-1, 1] to [0, 1] and update in-value
	   result.x = (point.x / 2) + 0.5f;
	   result.y = 1 - ((point.y / 2) + 0.5f);
 
	   return true;
	}

	void InGameMenu::quit(MyGUI::WidgetPtr _sender)
	{
		playEffect("click4.ogg");
		Game::getSingleton().quit();
	}

	void InGameMenu::closeSettings(MyGUI::Window* _sender, const std::string& _name)
	{
		if(mSettingsWindow->getVisible()) {
			mSettingsWindow->setVisibleSmooth(false);
		}
	}

	void InGameMenu::openNav(MyGUI::WidgetPtr _sender)
	{
		Game::getSingleton().setNavVisible(true);
		update();
	}
 
	void InGameMenu::openStore(MyGUI::WidgetPtr _sender)
	{
		resetBuyMenu();
		resetRepairMenu();

		mStoreWindowWindow->setVisibleSmooth(true);
	}

	void InGameMenu::closeStore(MyGUI::Window* _sender, const std::string& _name)
	{
		playButtonClick(_sender);
		mStoreWindowWindow->setVisibleSmooth(false);
	}

	void InGameMenu::contactButtonPressed(MyGUI::WidgetPtr _sender)
	{
		Contact *c = *(_sender->getUserData<Contact *>());

		Ogre::String msg = "";
		MyGUI::delegates::CDelegate1<MyGUI::WidgetPtr>::IDelegate *action1 = NULL;
		Ogre::String action1msg = "";
		MyGUI::delegates::CDelegate1<MyGUI::WidgetPtr>::IDelegate *action2 = NULL;
		Ogre::String action2msg = "";

		switch(c->mission.status) {
		case MissionStatusDefault:
			action1msg = "NO THANKS";
			action1 = MyGUI::newDelegate(this,&InGameMenu::closeDialog);
			action2msg = "ACCEPT";
			action2 = MyGUI::newDelegate(this,&InGameMenu::acceptMission);
			msg = c->mission.objective;
			break;
		case MissionStatusAccepted:
			action2msg = "CLOSE";
			action2 = MyGUI::newDelegate(this,&InGameMenu::closeDialog);
			msg = c->mission.waitingMessage;
			break;
		case MissionStatusCompleted:
			action2msg = "CLOSE";
			action2 = MyGUI::newDelegate(this,&InGameMenu::closeDialog);
			msg = c->mission.successMessage;
			c->mission.status = MissionStatusTurnedIn;

			if(c->mission.reward > 0) {
				Game::getSingleton().mPlayerMoney += c->mission.reward;
				update();
			}
			break;
		case MissionStatusTurnedIn:
			action2msg = "CLOSE";
			action2 = MyGUI::newDelegate(this,&InGameMenu::closeDialog);
			msg = c->mission.successMessage;
			break;
		case MissionStatusFailed:
		default:
			// nada
			return;
		}

		displayDialog(
			c->name,
			msg,
			c->imageName,
			action1msg,
			action1,
			action2msg,
			action2,
			true
			);

		mOkButton->setUserData(c);
	}

	void InGameMenu::openSettings(MyGUI::WidgetPtr _sender)
	{
		if(!mSettingsWindow->getVisible()) {
			mSettingsWindow->setVisibleSmooth(true);
		}
	}

	void InGameMenu::setVisible(bool visible)
	{
		mMainWidget->setVisible(visible);

		if(!visible) {
			// reset
			mDialogWindowWindow->setVisible(false);
			mContactsWidget->setVisible(false);
			mLandedWidget->setVisible(false);
			mStoreWindowWindow->setVisible(false);
			mShipWidget->setVisible(false);
			mNavLocationWidget->setVisible(false);
			mPopupPanel->setVisible(false);
			mEnemyWidget->setVisible(false);
			mRunButton->setVisible(false);
		}
	}

	void InGameMenu::toggleBuyCheckbox(MyGUI::WidgetPtr _sender)
	{
		MyGUI::ButtonPtr btn = (MyGUI::ButtonPtr)_sender;
		btn->setStateSelected(!btn->getStateSelected());
		
		playButtonClick(NULL);

		updateBuyCost();
	}

	void InGameMenu::toggleRepairCheckbox(MyGUI::WidgetPtr _sender)
	{
		MyGUI::ButtonPtr btn = (MyGUI::ButtonPtr)_sender;
		btn->setStateSelected(!btn->getStateSelected());
		
		playButtonClick(NULL);

		updateRepairCost();
	}

	void InGameMenu::toggleShowNavGrid(MyGUI::WidgetPtr _sender)
	{
		bool mNewState = !(Game::getSingleton().mShowGrid);

		Game::getSingleton().setShowNavGrid(mNewState);
		mShowNavGrid->setStateSelected(mNewState);
	}

	void InGameMenu::update()
	{
		setNavButtonsVisible(false);

		mNavLocationWidget->setVisible(false);
		mLandedWidget->setVisible(false);
		mShipWidget->setVisible(false);
		mEnemyWidget->setVisible(false);
		mRunButton->setVisible(false);

		BOOL navOpen = Game::getSingleton().mNavOpen;
		BOOL inBattle = Game::getSingleton().mBattleStarted;

		GameNode *currentNode = NULL;
		
		if(Game::getSingleton().getGameState() > GameStateMainMenu) {
			currentNode = Game::getSingleton().mGameNodes[
					Game::getSingleton().mCurrentNodeIdx];
		}

		switch(Game::getSingleton().getGameState()) {
			case GameStateMainMenu:
				if(mNavButtons.size()) {
					for(unsigned int i = 0; i < mNavButtons.size(); i++) {
						Game::getSingleton().mGUI->destroyWidget(mNavButtons[i]);
					}

					mNavButtons.clear();
				}
				if(mNavButtonLabels.size()) {
					for(unsigned int i = 0; i < mNavButtonLabels.size(); i++) {
						Game::getSingleton().mGUI->destroyWidget(mNavButtonLabels[i]);
					}

					mNavButtonLabels.clear();
				}
				mCloseNavButton->setVisible(false);
				break;
			case GameStateSpace:
				mContactsWidget->setVisible(false);
				mLocationDescriptionTextBox->setCaption(currentNode->title + 
					"\n\nI must-see destination for any visitor to the galaxy, this system offers much in the way of commerce and entertainment.");
				if(!navOpen && !inBattle) {
					mNavLocationWidget->setVisible(true);
					mLandButton->setVisible(currentNode->hasCity);
					mDockButton->setVisible(currentNode->hasStation);
				}
				mShipWidget->setVisible(!navOpen);
				break;
			case GameStateCity:
				mLandedDescriptionTextBox->setCaption(currentNode->cityName + 
					"\n\nThe capital city of " + currentNode->title + 
					" has always been the pride of it's residents.  Some say the ship yards here are the finest in the galaxy.");
				if(!navOpen && !inBattle) {
					mLandedWidget->setVisible(true);
				}
				// open contacts if the station has any
				if(currentNode->cityContacts.size()) {
					mContactsWidget->setVisible(true);
					mContactsButton1Button->setCaption(currentNode->cityContacts[0].name);
					mContactsButton1Button->setUserData(&currentNode->cityContacts[0]);
					if(currentNode->cityContacts.size() > 1) {
						mContactsButton2Button->setCaption(currentNode->cityContacts[1].name);
						mContactsButton2Button->setVisible(true);
						mContactsButton2Button->setUserData(&currentNode->cityContacts[1]);
					}
					else {
						mContactsButton2Button->setVisible(false);
					}
				}
				mShipWidget->setVisible(!navOpen);
				break;
			case GameStateStation:
				mLandedDescriptionTextBox->setCaption(currentNode->stationName + 
					"\n\nThe central space station of " + currentNode->title + 
					" is a hub for commerce in the area.  Many of the wealthiest residents in the galaxy have vacation cabins here.");

				if(!navOpen && !inBattle) {
					mLandedWidget->setVisible(true);
				}

				// open contacts if the station has any
				if(currentNode->stationContacts.size()) {
					mContactsWidget->setVisible(true);
					mContactsButton1Button->setCaption(currentNode->stationContacts[0].name);
					mContactsButton1Button->setUserData(&currentNode->stationContacts[0]);
					if(currentNode->stationContacts.size() > 1) {
						mContactsButton2Button->setCaption(currentNode->stationContacts[1].name);
						mContactsButton2Button->setVisible(true);
						mContactsButton2Button->setUserData(&currentNode->stationContacts[1]);
					}
					else {
						mContactsButton2Button->setVisible(false);
					}
				}
				mShipWidget->setVisible(!navOpen);
				break;
			default:
				break;
		}

		if(navOpen && !inBattle) {
			if(!mNavButtons.size()) {
				createNavButtons();
			}
			mContactsWidget->setVisible(false);
		}
		if(inBattle) {
			mShipWidget->setVisible(true);
			mEnemyWidget->setVisible(true);
			mRunButton->setVisible(true);
		}

		setNavButtonsVisible(navOpen && !inBattle);
		mCloseNavButton->setVisible(navOpen && !inBattle);

		if(Game::getSingleton().mPlayerShip &&
			Game::getSingleton().mPlayerShip->mRangeBillboardSet) {
			Game::getSingleton().mPlayerShip->mRangeBillboardSet->setVisible(
				navOpen && !inBattle);
		}
		if(Game::getSingleton().mDangerZoneNode) {
			Game::getSingleton().mDangerZoneNode->setVisible(
				navOpen && !inBattle);
		}

		// credits
		mMoneyTextBoxTextBox->setCaption("Credits: " + 
			Ogre::StringConverter::toString(Game::getSingleton().mPlayerMoney));

		// fuel
		if(Game::getSingleton().mPlayerShip) {
			mFuelTextBox->setCaption("Fuel: " +
				Ogre::StringConverter::toString(Game::getSingleton().mPlayerShip->mFuel));
		}
	}

	void InGameMenu::updateBattleStats()
	{
		Ship *player = Game::getSingleton().mPlayerShip;
		Ship *enemy =  Game::getSingleton().mEnemyShip;

		Ogre::String hull, hullAmt,
			wpn,wpnAmt,
			shld, shldAmt;
		hull = Ogre::StringConverter::toString(player->mHullLevel);
		hullAmt = Ogre::StringConverter::toString(player->mHullStrength);
		wpn = Ogre::StringConverter::toString(player->mWeaponsLevel);
		wpnAmt = Ogre::StringConverter::toString((float)(100.0 - player->mWeaponsDamage));
		shld = Ogre::StringConverter::toString(player->mShieldLevel);
		shldAmt = Ogre::StringConverter::toString((float)(player->mShieldStrength));

		mStatsTextBox->setCaption("HULL        |  LEVEL " + hull + 
			"  |  "+ hullAmt + 
			"%\nSHIELDS   |  LEVEL " + shld +"  |  " +shldAmt +
			"%\nWEAPONS |  LEVEL " + wpn + "  |  " + wpnAmt +"%");
		if(enemy) {
			hull = Ogre::StringConverter::toString(enemy->mHullLevel);
			hullAmt = Ogre::StringConverter::toString(enemy->mHullStrength);
			wpn = Ogre::StringConverter::toString(enemy->mWeaponsLevel);
			wpnAmt = Ogre::StringConverter::toString((float)(100.0 - enemy->mWeaponsDamage));
			shld = Ogre::StringConverter::toString(enemy->mShieldLevel);
			shldAmt = Ogre::StringConverter::toString((float)(enemy->mShieldStrength));

			mEnemyStatsTextBox->setCaption("HULL        |  LEVEL " + hull + 
				"  |  "+ hullAmt + 
				"%\nSHIELDS   |  LEVEL " + shld +"  |  " +shldAmt +
				"%\nWEAPONS |  LEVEL " + wpn + "  |  " + wpnAmt +"%");
		}
	}

	void InGameMenu::updateMaxGameTime(MyGUI::EditBox* _sender)
	{
		double time = Ogre::StringConverter::parseReal(_sender->getCaption());
		Game::getSingleton().setMaxGameTime(time);
	}

	void InGameMenu::updateTimeLeft(double secLeft)
	{
		double time = secLeft;
		int mins = floor(time / (60.0));
		time -= mins * 60.0;

		int sec = floor(time);
		time -= sec;

		int ms = time * 1000.0;

		Ogre::String sMin = Ogre::StringConverter::toString(mins);
		if(sMin.size() == 1) {
			sMin = "0" + sMin;
		}
		Ogre::String sSec = Ogre::StringConverter::toString(sec);
		if(sSec.size() == 1) {
			sSec = "0" + sSec;
		}
		Ogre::String sMs = Ogre::StringConverter::toString(ms);
		if(sMs.size() == 1) {
			sMs = "00" + sMs;
		}
		else if(sMs.size() == 2) {
			sMs = "0" + sMs;
		}

		mTimerTextBoxTextBox->setCaption("Time Left " + sMin + ":" + sSec + ":" + sMs);
	}

	void InGameMenu::createNavButtons()
	{
		float height = Game::getSingleton().mRenderWindow->getHeight();
		float width = Game::getSingleton().mRenderWindow->getWidth();
		
		for(unsigned int i =0; i < Game::getSingleton().mGameNodes.size(); i++) {
			GameNode *node = Game::getSingleton().mGameNodes[i];
			Ogre::Vector2 result;
			if(getScreenspaceCoords(node->scenenode,
				Game::getSingleton().mCamera,
				result)) {

				float x = result.x * width;
				float y = result.y * height;
				float buttonWidth = 46.0;
				float buttonHeight = 50.0;
				MyGUI::IntCoord coord(
					x - buttonWidth * 0.35f,
					y - buttonHeight * 0.3f,
					buttonWidth,
					buttonHeight);

				MyGUI::ButtonPtr button = mMainWidget->createWidget<MyGUI::Button>(
					"ButtonNavSkin", 
					coord, 
					MyGUI::Align::Default, 
					node->title + "Button");
				button->setCaption(node->title);
				button->setUserData(i);
				button->setVisible(node->visible);
				button->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);
				button->eventMouseButtonClick += MyGUI::newDelegate(this,&InGameMenu::navButtonPressed);

				mNavButtons.push_back(button);

				coord.left = coord.left + coord.width;
				coord.top = coord.top + coord.height - 10.0f;
				coord.height = 20.0f;
				coord.width = 100.0f;
				MyGUI::TextBox *textBox = mMainWidget->createWidget<MyGUI::TextBox>(
					"TextBox", 
					coord, 
					MyGUI::Align::Left, 
					node->title + "TextBox");
				textBox->setNeedMouseFocus(false);
				textBox->setVisible(node->visible);
				textBox->setCaption(node->title);
				mNavButtonLabels.push_back(textBox);
			}
		}
	}

	void InGameMenu::depart(MyGUI::WidgetPtr _sender)
	{
		Game::getSingleton().depart();
		// hide store
		closeStore(NULL,"");
	}

	void InGameMenu::land(MyGUI::WidgetPtr _sender)
	{
		Game::getSingleton().land();
	}

	void InGameMenu::navButtonPressed(MyGUI::WidgetPtr _sender)
	{
		unsigned int i = *(_sender->getUserData<unsigned int>());

		GameNode *node = Game::getSingleton().mGameNodes[i];
		playButtonClick(_sender);

		float fuelNeeded = Game::getSingleton().fuelCostToTravelTo(i);

		Ogre::String info = "";
		if(node->hasCity) {
			info += "\nSpaceport: " + node->cityName;
		}
		if(node->hasStation) {
			info += "\nStation: " + node->stationName;
		}

		if(Game::getSingleton().canTravelToNodeWithIndex(i)) {
			mDialogWindowWindow->setUserData(i);
			displayDialog(
				node->title,
				info + "\nFuel Needed: " + Ogre::StringConverter::toString(fuelNeeded)
				,"",
				"CLOSE",
				MyGUI::newDelegate(this,&InGameMenu::closeDialog),
				"TRAVEL",
				MyGUI::newDelegate(this,&InGameMenu::travelToNode)
				);
		}
		else {
			Ogre::String err = "Cannot travel: Out of range or not\n enough fuel.";
			if(Game::getSingleton().getGameState() == GameStateCity) {
				err = "Cannot travel: Unable to engage FTL system\n when landed.";
			}
			else if(Game::getSingleton().getGameState() == GameStateStation) {
				err = "Cannot travel: Unable to engage FTL system\n when docked.";
			}

			displayDialog(
				node->title,
				info + "\nFuel Needed: " + Ogre::StringConverter::toString(fuelNeeded) +
				"\n\n" + err,
				"",
				"",
				NULL,
				"CLOSE",
				MyGUI::newDelegate(this,&InGameMenu::closeDialog)
				);
		}


	}

	void InGameMenu::repair(MyGUI::WidgetPtr _sender)
	{
		updateRepairCost();

		// do we have enough money for this?
		float cost = *(mRepairCostTextBox->getUserData<float>());
		if(cost < 0) cost = 0;

		if(cost <= 0 || cost > Game::getSingleton().mPlayerMoney) {
			playEffect("error.ogg");
			return;
		}

		playButtonClick(_sender);

		Ship *playerShip = Game::getSingleton().mPlayerShip;

		if(mRepairCheckBox1Button->getStateSelected()) {
			playerShip->mEngineDamage = 0;
		}
		if(mRepairCheckBox2Button->getStateSelected()) {
			playerShip->mShieldDamage = 0;
		}
		if(mRepairCheckBox3Button->getStateSelected()) {
			playerShip->mHullStrength = 100;
		}
		if(mRepairCheckBox4Button->getStateSelected()) {
			playerShip->mWeaponsDamage = 0;
		}

		Game::getSingleton().mPlayerMoney -= cost;
		
		// sanity check for rounding issues
		if(Game::getSingleton().mPlayerMoney < 0) {
			Game::getSingleton().mPlayerMoney = 0;
		}

		resetRepairMenu();

		// credits
		mMoneyTextBoxTextBox->setCaption("Credits: " + 
			Ogre::StringConverter::toString(Game::getSingleton().mPlayerMoney));
	}

	void InGameMenu::reloadConfig(MyGUI::WidgetPtr _sender)
	{
		Game::getSingleton().mConfig->load("Game.cfg","=:\t", false);
	}

	void InGameMenu::resetBuyMenu()
	{
		Ship *playerShip = Game::getSingleton().mPlayerShip;

		mFuelQty->setCaption("0");
		mFuelCost->setCaption("Fuel Cost: 0");

		mBuyCheckBox1Button->setStateSelected(false);
		if(playerShip->mEngineLevel < 3) {
			mBuyCheckBox1Button->setEnabled(true);
			int lvl = playerShip->mEngineLevel + 1;
			int cost = lvl * 100;
			mBuyCheckBox1Button->setUserData(cost);
			mBuyCheckBox1Button->setCaption(" Engine Upgrade " +
				Ogre::StringConverter::toString(lvl) + ": " +
				Ogre::StringConverter::toString(cost) );
		}
		else {
			mBuyCheckBox1Button->setCaption(" No Engine Upgrades");
			mBuyCheckBox1Button->setEnabled(false);
		}

		mBuyCheckBox2Button->setStateSelected(false);
		if(playerShip->mShieldLevel < 3) {
			mBuyCheckBox2Button->setEnabled(true);
			int lvl = playerShip->mShieldLevel + 1;
			int cost = lvl * 100;

			mBuyCheckBox2Button->setUserData(cost);
			mBuyCheckBox2Button->setCaption(" Shields Upgrade " +
				Ogre::StringConverter::toString(lvl) + ": " +
				Ogre::StringConverter::toString(cost) );
		}
		else {
			mBuyCheckBox2Button->setCaption(" No Shield Upgrades");
			mBuyCheckBox2Button->setEnabled(false);
		}

		mBuyCheckBox3Button->setStateSelected(false);
		if(playerShip->mHullLevel < 3) {
			mBuyCheckBox3Button->setEnabled(true);
			int lvl = playerShip->mHullLevel + 1;
			int cost = lvl * 100;

			mBuyCheckBox3Button->setUserData(cost);
			mBuyCheckBox3Button->setCaption(" Hull Upgrade " +
				Ogre::StringConverter::toString(lvl) + ": " +
				Ogre::StringConverter::toString(cost) );
		}
		else {
			mBuyCheckBox3Button->setCaption(" No Hull Upgrades");
			mBuyCheckBox3Button->setEnabled(false);
		}

		mBuyCheckBox4Button->setStateSelected(false);
		if(playerShip->mWeaponsLevel < 3) {
			mBuyCheckBox4Button->setEnabled(true);
			int lvl = playerShip->mWeaponsLevel + 1;
			int cost = lvl * 100;

			mBuyCheckBox4Button->setUserData(cost);
			mBuyCheckBox4Button->setCaption(" Weapons Upgrade " +
				Ogre::StringConverter::toString(lvl) + ": " +
				Ogre::StringConverter::toString(cost) );
		}
		else {
			mBuyCheckBox4Button->setCaption(" No Weapons Upgrades");
			mBuyCheckBox4Button->setEnabled(false);
		}

		updateBuyCost();
	}

	void InGameMenu::resetRepairMenu()
	{
		Ship *playerShip = Game::getSingleton().mPlayerShip;

		float cost = 0;

		cost = playerShip->mEngineDamage;
		if(cost < 0) cost = 0;
		mRepairCheckBox1Button->setUserData(cost);
		mRepairCheckBox1Button->setStateSelected(false);
		if(cost <= 0) {
			mRepairCheckBox1Button->setEnabled(false);
			mRepairCheckBox1Button->setCaption(" No Engine Damage ");
		}
		else {
			mRepairCheckBox1Button->setEnabled(true);
			mRepairCheckBox1Button->setCaption(" Engine Damage: " +
				Ogre::StringConverter::toString(cost) );
		}

		cost = playerShip->mShieldDamage;
		if(cost < 0) cost = 0;
		mRepairCheckBox2Button->setUserData(cost);
		mRepairCheckBox2Button->setStateSelected(false);
		if(cost <= 0) {
			mRepairCheckBox2Button->setEnabled(false);
			mRepairCheckBox2Button->setCaption(" No Shield Damage ");
		}
		else {
			mRepairCheckBox2Button->setEnabled(true);
			mRepairCheckBox2Button->setCaption(" Shield Damage: " +
				Ogre::StringConverter::toString(cost) );
		}
		cost = 100.0 - playerShip->mHullStrength;
		if(cost < 0) cost = 0;
		mRepairCheckBox3Button->setUserData(cost);
		mRepairCheckBox3Button->setStateSelected(false);
		if(cost <= 0) {
			mRepairCheckBox3Button->setEnabled(false);
			mRepairCheckBox3Button->setCaption(" No Hull Damage ");
		}
		else {
			mRepairCheckBox3Button->setEnabled(true);
			mRepairCheckBox3Button->setCaption(" Hull Damage: " +
				Ogre::StringConverter::toString(cost) );
		}

		cost = playerShip->mWeaponsDamage;
		if(cost < 0) cost = 0;
		mRepairCheckBox4Button->setUserData(cost);
		mRepairCheckBox4Button->setStateSelected(false);
		if(cost <= 0) {
			mRepairCheckBox4Button->setEnabled(false);
			mRepairCheckBox4Button->setCaption(" No Weapons Damage ");
		}
		else {
			mRepairCheckBox4Button->setEnabled(true);
			mRepairCheckBox4Button->setCaption(" Weapons Damage: " +
				Ogre::StringConverter::toString(cost) );
		}

		updateRepairCost();
	}


	void InGameMenu::travelToNode(MyGUI::WidgetPtr _sender)
	{
		closeDialog(NULL);
		unsigned int i = *(mDialogWindowWindow->getUserData<unsigned int>());
		if(Game::getSingleton().travelToNodeWithIndex(i)) {
			playButtonClick(_sender);
		}
		else {
			playEffect("error.ogg");
		}
	}

	void InGameMenu::updateBuyCost()
	{
		float cost = 0;

		float fuel = Ogre::StringConverter::parseInt(mFuelQty->getCaption());
		if(fuel < 0) fuel = 0;
		float fuelCost = (float)fuel * 100.0;

		int engine = *(mBuyCheckBox1Button->getUserData<int>());
		if(engine < 0) engine = 0;
		if(!mBuyCheckBox1Button->getStateSelected()) engine = 0;

		float shields = *(mBuyCheckBox2Button->getUserData<int>());
		if(shields < 0) shields = 0;
		if(!mBuyCheckBox2Button->getStateSelected()) shields = 0;

		float hull = *(mBuyCheckBox3Button->getUserData<int>());
		if(hull < 0) hull = 0;
		if(!mBuyCheckBox3Button->getStateSelected()) hull = 0;

		float weapons =*(mBuyCheckBox4Button->getUserData<int>());
		if(weapons < 0) weapons = 0;
		if(!mBuyCheckBox4Button->getStateSelected()) weapons = 0;

		cost = fuelCost + engine + shields + hull + weapons;

		mBuyCostTextBox->setUserData(cost);
		mBuyCostTextBox->setCaption("TOTAL: " +
			Ogre::StringConverter::toString(cost));
	}

	void InGameMenu::updateRepairCost()
	{
		float cost = 0;

		float engines = *(mRepairCheckBox1Button->getUserData<float>());
		if(engines < 0) engines = 0;
		if(!mRepairCheckBox1Button->getStateSelected()) engines = 0;

		float shields = *(mRepairCheckBox2Button->getUserData<float>());
		if(shields < 0) shields = 0;
		if(!mRepairCheckBox2Button->getStateSelected()) shields = 0;

		float hull = *(mRepairCheckBox3Button->getUserData<float>());
		if(hull < 0) hull = 0;
		if(!mRepairCheckBox3Button->getStateSelected()) hull = 0;

		float weapons = *(mRepairCheckBox4Button->getUserData<float>());
		if(weapons < 0) weapons = 0;
		if(!mRepairCheckBox4Button->getStateSelected()) weapons = 0;

		cost = engines + shields + hull + weapons;

		mRepairCostTextBox->setUserData(cost);
		mRepairCostTextBox->setCaption("TOTAL: " +
			Ogre::StringConverter::toString(cost));
	}
	void InGameMenu::setNavButtonsVisible(bool visible)
	{
		for(unsigned int i = 0; i < mNavButtons.size(); i++) {
			MyGUI::ButtonPtr btn = mNavButtons[i];
			MyGUI::TextBox *label = mNavButtonLabels[i];

			if(i == Game::getSingleton().mCurrentNodeIdx) {
				btn->setStateSelected(true);
				label->setColour(MyGUI::Colour(1.0f,1.0f,1.0f,0.8f));
				//label->setTextShadow(true);
				//label->setTextShadowColour(MyGUI::Colour(1.0f,1.0f,1.0f,0.5f));

				label->setTextColour(MyGUI::Colour(1.0f,1.0f,1.0f,0.8f));
			}
			else {
				btn->setStateSelected(false);
				label->setColour(MyGUI::Colour(0.0f/255.0f,156.0f/255.0f,215.0f/255.0f,0.8f));
				//label->setTextShadow(true);
				//label->setTextShadowColour(MyGUI::Colour(0.0f/255.0f,156.0f/255.0f,215.0f/255.0f,0.5f));
				label->setTextColour(MyGUI::Colour(0.0f/255.0f,156.0f/255.0f,215.0f/255.0f,0.8f));
			}

			if(!visible) {
				mNavButtons[i]->setVisible(false);
				label->setVisible(false);
			}
			else {
				// only show nav buttons whose nodes are visible
				if(i < Game::getSingleton().mGameNodes.size()) {
					GameNode *node = Game::getSingleton().mGameNodes[i];
					mNavButtons[i]->setVisible(node->visible);
					label->setVisible(node->visible);
				}
				else {
					mNavButtons[i]->setVisible(true);
					label->setVisible(true);
				}
			}
			//mNavButtons[i]->setVisible(true);
		}
	}

} // Salvation
