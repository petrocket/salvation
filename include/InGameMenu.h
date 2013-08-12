#include "OgrePlatform.h"

#ifndef _IN_GAME_MENU_H_
#define _IN_GAME_MENU_H_

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	// using pch
	#include "stdafx.h"
#else

#include "Ogre.h"
#include "BaseLayout/BaseLayout.h"

#endif


namespace Salvation
{

	ATTRIBUTE_CLASS_LAYOUT(InGameMenu, "ingame.layout");
	class InGameMenu :
		public wraps::BaseLayout
	{
	public:
		InGameMenu(MyGUI::Widget* _parent = nullptr);
		virtual ~InGameMenu();

		void displayDialog(
			Ogre::String title, 
			Ogre::String message, 
			Ogre::String imageName,
			Ogre::String action1Name,
			MyGUI::delegates::CDelegate1<MyGUI::WidgetPtr>::IDelegate *action1Delegate,
			Ogre::String action2Name,
			MyGUI::delegates::CDelegate1<MyGUI::WidgetPtr>::IDelegate *action2Delegate,
			bool modal = false
			);
		void closeDialog(MyGUI::WidgetPtr _sender);

		void setVisible(bool visible);	

		void depart(MyGUI::WidgetPtr _sender);
		void dock(MyGUI::WidgetPtr _sender);
		void fight(MyGUI::WidgetPtr _sender);
		void land(MyGUI::WidgetPtr _sender);
		void run(MyGUI::WidgetPtr _sender);

		void closeNav(MyGUI::WidgetPtr _sender);
		void openNav(MyGUI::WidgetPtr _sender);

		void openSettings(MyGUI::WidgetPtr _sender);
		void closeSettings(MyGUI::Window* _sender, const std::string& _name);

		void openStore(MyGUI::WidgetPtr _sender);
		void closeStore(MyGUI::Window* _sender, const std::string& _name);

		void update();
		void updateTimeLeft(double secLeft);

		void quit(MyGUI::WidgetPtr _sender);
		void navButtonPressed(MyGUI::WidgetPtr _sender);
		void toggleShowNavGrid(MyGUI::WidgetPtr _sender);
		void travelToNode(MyGUI::WidgetPtr _sender);
		void updateMaxGameTime(MyGUI::EditBox* _sender);

	private:
		std::vector<MyGUI::ButtonPtr> mNavButtons;

		bool getScreenspaceCoords(Ogre::SceneNode* node, Ogre::Camera* camera, Ogre::Vector2& result);
		void createNavButtons();
		void setNavButtonsVisible(bool visible);

	//%LE Widget_Declaration list start
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mDialogWindowWindow, "DialogWindow");
		MyGUI::Window* mDialogWindowWindow;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mDialogTextBox, "Dialog");
		MyGUI::TextBox* mDialogTextBox;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mOkButton, "Ok");
		MyGUI::Button* mOkButton;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mAction1Button, "Action1");
		MyGUI::Button* mAction1Button;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mDialogImageImageBox, "DialogImage");
		MyGUI::ImageBox* mDialogImageImageBox;


		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mNavLocationWidget, "NavLocation");
		MyGUI::Widget* mNavLocationWidget;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mNavButton, "Nav");
		MyGUI::Button* mNavButton;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mLocationDescriptionTextBox, "LocationDescription");
		MyGUI::TextBox* mLocationDescriptionTextBox;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mLandButton, "Land");
		MyGUI::Button* mLandButton;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mDockButton, "Dock");
		MyGUI::Button* mDockButton;

		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mShipWidget, "Ship");
		MyGUI::Widget* mShipWidget;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mStatsTextBox, "Stats");
		MyGUI::TextBox* mStatsTextBox;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mInGameMenuWidget, "InGameMenu");
		MyGUI::Widget* mInGameMenuWidget;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mQuitButton, "Quit");
		MyGUI::Button* mQuitButton;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mSettingsButton, "Settings");
		MyGUI::Button* mSettingsButton;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mMoneyTextBoxTextBox, "MoneyTextBox");
		MyGUI::TextBox* mMoneyTextBoxTextBox;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mTimerTextBoxTextBox, "TimerTextBox");
		MyGUI::TextBox* mTimerTextBoxTextBox;

		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mLandedWidget, "Landed");
		MyGUI::Widget* mLandedWidget;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mLandedDescriptionTextBox, "LandedDescription");
		MyGUI::TextBox* mLandedDescriptionTextBox;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mDepartButton, "Depart");
		MyGUI::Button* mDepartButton;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mStoreButton, "Store");
		MyGUI::Button* mStoreButton;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mLandedNavMapButton, "landedNavMap");
		MyGUI::Button* mLandedNavMapButton;

		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mStoreWindowWindow, "StoreWindow");
		MyGUI::Window* mStoreWindowWindow;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mRepairButton, "Repair");
		MyGUI::Button* mRepairButton;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mRepairCostTextBox, "RepairCost");
		MyGUI::TextBox* mRepairCostTextBox;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mBuyButton, "Buy");
		MyGUI::Button* mBuyButton;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mBuyCostTextBox, "BuyCost");
		MyGUI::TextBox* mBuyCostTextBox;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mRepairCheckBox1Button, "RepairCheckBox1");
		MyGUI::Button* mRepairCheckBox1Button;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mRepairCheckBox2Button, "RepairCheckBox2");
		MyGUI::Button* mRepairCheckBox2Button;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mRepairCheckBox3Button, "RepairCheckBox3");
		MyGUI::Button* mRepairCheckBox3Button;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mRepairCheckBox4Button, "RepairCheckBox4");
		MyGUI::Button* mRepairCheckBox4Button;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mRepairCheckBox5Button, "RepairCheckBox5");
		MyGUI::Button* mRepairCheckBox5Button;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mBuyCheckBox1Button, "BuyCheckBox1");
		MyGUI::Button* mBuyCheckBox1Button;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mBuyCheckBox2Button, "BuyCheckBox2");
		MyGUI::Button* mBuyCheckBox2Button;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mBuyCheckBox3Button, "BuyCheckBox3");
		MyGUI::Button* mBuyCheckBox3Button;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mRepairHintTextBox, "RepairHint");
		MyGUI::TextBox* mRepairHintTextBox;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mContactsWidget, "Contacts");
		MyGUI::Widget* mContactsWidget;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mContactsTitleTextBox, "ContactsTitle");
		MyGUI::TextBox* mContactsTitleTextBox;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mContactsButton1Button, "ContactsButton1");
		MyGUI::Button* mContactsButton1Button;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mContactsButton2Button, "ContactsButton2");
		MyGUI::Button* mContactsButton2Button;


		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mPopupPanel, "PopupPanel");
		MyGUI::Widget* mPopupPanel;


		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mSettingsWindow, "SettingsDialog");
		MyGUI::Window* mSettingsWindow;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mShowNavNodes, "showNavNodes");
		MyGUI::Button* mShowNavNodes;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mShowNavGrid, "showNavGrid");
		MyGUI::Button* mShowNavGrid;
		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mMaxGameTime, "maxGameTime");
		MyGUI::EditBox* mMaxGameTime;

		ATTRIBUTE_FIELD_WIDGET_NAME(InGameMenu, mCloseNavButton, "closeNav");
		MyGUI::Button* mCloseNavButton;


	//%LE Widget_Declaration list end
	};

} // namespace Salvation

#endif // _IN_GAME_MENU_H_
