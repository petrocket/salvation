#include "InGameMenu.h"

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

		setVisible(false);
	}

	InGameMenu::~InGameMenu()
	{
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
		}
	}

	void InGameMenu::update()
	{
		switch(Game::getSingleton().getGameState()) {
			case GameStateNav:
				if(!mNavButtons.size()) {
					createNavButtons();
				}

				break;
		}
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
				MyGUI::IntCoord coord(x,y,100,100);

				MyGUI::ButtonPtr button = mMainWidget->createWidget<MyGUI::Button>(
					"Button", 
					coord, 
					MyGUI::Align::Default, 
					node->title + "Button");
				button->setCaption(node->title);
				button->eventMouseSetFocus += MyGUI::newDelegate(playButtonOver);
				button->eventMouseButtonClick += MyGUI::newDelegate(playButtonClick);

			}
		}
	}

	void InGameMenu::setNavButtonsVisible(bool visible)
	{

	}

} // Salvation
