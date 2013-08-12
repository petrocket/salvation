#include "InGameMenu.h"
#include "GameAudioInterface.h"
#include "Game.h"

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

		setVisible(false);

		mSettingsWindow->setVisible(false);
		mSettingsWindow->eventWindowButtonPressed += MyGUI::newDelegate(this, &Salvation::InGameMenu::closeSettings);

		mShowNavGrid->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::toggleShowNavGrid);
		mShowNavGrid->setStateSelected(Game::getSingleton().mShowGrid);

		mMaxGameTime->eventEditTextChange += MyGUI::newDelegate(this, &Salvation::InGameMenu::updateMaxGameTime);
		mMaxGameTime->setCaption(Ogre::StringConverter::toString((float)Game::getSingleton().mMaxGameTime));

		mCloseNavButton->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::closeNav);
		mDepartButton->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::depart);

		mLandedNavMapButton->eventMouseButtonClick += MyGUI::newDelegate(this, &Salvation::InGameMenu::openNav);
		mNavButton->eventMouseButtonClick  += MyGUI::newDelegate(this, &Salvation::InGameMenu::openNav);

		mLandButton->eventMouseButtonClick  += MyGUI::newDelegate(this, &Salvation::InGameMenu::land);
		mDockButton->eventMouseButtonClick  += MyGUI::newDelegate(this, &Salvation::InGameMenu::dock);

		mStoreButton->eventMouseButtonClick  += MyGUI::newDelegate(this, &Salvation::InGameMenu::openStore);
		mStoreWindowWindow->eventWindowButtonPressed += MyGUI::newDelegate(this, &Salvation::InGameMenu::closeStore);


		/*
		const MyGUI::IntSize size(200, 200);
		mJumpRange = MyGUI::Gui::getInstance().createWidget<MyGUI::ImageBox>("ImageBox",MyGUI::IntCoord(0,0, size.width, size.height), MyGUI::Align::Default, "Back");
//		mJumpRange = mMainWidget->createWidget<MyGUI::ImageBox>("ImageBox", MyGUI::IntCoord(0,0, size.width, size.height), MyGUI::Align::Default);
		mJumpRange->setImageTexture("white-circle.png");
		*/
	}

	InGameMenu::~InGameMenu()
	{
	}

	void InGameMenu::closeDialog(MyGUI::WidgetPtr _sender)
	{
		mDialogWindowWindow->setVisibleSmooth(false);
		mPopupPanel->setVisible(false);
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

	void InGameMenu::fight(MyGUI::WidgetPtr _sender)
	{
		mDialogWindowWindow->setVisibleSmooth(false);
		mPopupPanel->setVisible(false);
		//Game::getSingleton().fight();
	}

	void InGameMenu::run(MyGUI::WidgetPtr _sender)
	{
		mDialogWindowWindow->setVisibleSmooth(false);
		mPopupPanel->setVisible(false);
		//Game::getSingleton().run();
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
		mStoreWindowWindow->setVisibleSmooth(true);
	}

	void InGameMenu::closeStore(MyGUI::Window* _sender, const std::string& _name)
	{
		mStoreWindowWindow->setVisibleSmooth(false);
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
		}
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
				mCloseNavButton->setVisible(false);
				break;
			case GameStateSpace:
				mContactsWidget->setVisible(false);
				mLocationDescriptionTextBox->setCaption(currentNode->title);
				if(!Game::getSingleton().mNavOpen) {
					mNavLocationWidget->setVisible(true);
					mLandButton->setVisible(currentNode->hasCity);
					mDockButton->setVisible(currentNode->hasStation);
				}
				break;
			case GameStateCity:
				mLandedDescriptionTextBox->setCaption(currentNode->cityName);
				if(!Game::getSingleton().mNavOpen) {
					mLandedWidget->setVisible(true);
				}
				// open contacts if the station has any
				if(currentNode->cityContacts.size()) {
					mContactsWidget->setVisible(true);
					mContactsButton1Button->setCaption(currentNode->cityContacts[0].name);
					if(currentNode->cityContacts.size() > 1) {
						mContactsButton2Button->setCaption(currentNode->cityContacts[1].name);
						mContactsButton2Button->setVisible(true);
					}
					else {
						mContactsButton2Button->setVisible(false);
					}
				}
				break;
			case GameStateStation:
				mLandedDescriptionTextBox->setCaption(currentNode->stationName);
				if(!Game::getSingleton().mNavOpen) {
					mLandedWidget->setVisible(true);
				}

				// open contacts if the station has any
				if(currentNode->stationContacts.size()) {
					mContactsWidget->setVisible(true);
					mContactsButton1Button->setCaption(currentNode->stationContacts[0].name);
					if(currentNode->stationContacts.size() > 1) {
						mContactsButton2Button->setCaption(currentNode->stationContacts[1].name);
						mContactsButton2Button->setVisible(true);
					}
					else {
						mContactsButton2Button->setVisible(false);
					}
				}

				break;
			default:
				break;
		}

		if(Game::getSingleton().mNavOpen) {
			if(!mNavButtons.size()) {
				createNavButtons();
			}
			mContactsWidget->setVisible(false);
		}

		setNavButtonsVisible(Game::getSingleton().mNavOpen);
		mCloseNavButton->setVisible(Game::getSingleton().mNavOpen);

		if(Game::getSingleton().mPlayerShip &&
			Game::getSingleton().mPlayerShip->mRangeBillboardSet) {
			Game::getSingleton().mPlayerShip->mRangeBillboardSet->setVisible(
				Game::getSingleton().mNavOpen);
		}
		if(Game::getSingleton().mDangerZoneNode) {
			Game::getSingleton().mDangerZoneNode->setVisible(
				Game::getSingleton().mNavOpen);
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
				float buttonSize = 26.0;
				float halfButtonSize = buttonSize * 0.5f;
				MyGUI::IntCoord coord(x - halfButtonSize,y - halfButtonSize,buttonSize,buttonSize);

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
			}
		}
	}

	void InGameMenu::depart(MyGUI::WidgetPtr _sender)
	{
		Game::getSingleton().depart();
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

		if(Game::getSingleton().canTravelToNodeWithIndex(i)) {
			mDialogWindowWindow->setUserData(i);
			displayDialog(
				node->title,
				"Fuel Needed: 3\n\nDistance: 1","",
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
				"Fuel Needed: 3\n\nDistance: 1\n\n" + err,
				"",
				"",
				NULL,
				"CLOSE",
				MyGUI::newDelegate(this,&InGameMenu::closeDialog)
				);
		}


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

	void InGameMenu::setNavButtonsVisible(bool visible)
	{
		for(unsigned int i = 0; i < mNavButtons.size(); i++) {
			MyGUI::ButtonPtr btn = mNavButtons[i];
			if(i == Game::getSingleton().mCurrentNodeIdx) {
				btn->setStateSelected(true);
			}
			else {
				btn->setStateSelected(false);
			}
			if(!visible) {
				mNavButtons[i]->setVisible(false);
			}
			else {
				// only show nav buttons whose nodes are visible
				if(i < Game::getSingleton().mGameNodes.size()) {
					GameNode *node = Game::getSingleton().mGameNodes[i];
					mNavButtons[i]->setVisible(node->visible);
				}
				else {
					mNavButtons[i]->setVisible(true);
				}
			}
			//mNavButtons[i]->setVisible(true);
		}
	}

} // Salvation
