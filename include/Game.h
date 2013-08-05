#ifndef __Game_h_
#define __Game_h_

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	// using pch
	#include "stdafx.h"
#else

#include "Ogre.h"

#endif

class Game : public OIS::MouseListener , public OIS::KeyListener
{
public:
    Game(Ogre::SceneManager *mgr, Ogre::RenderWindow *win);
    virtual ~Game(void);

    void createScene(void);

    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	void exit(MyGUI::WidgetPtr _sender);

	MyGUI::Gui *mGUI;
	MyGUI::OgrePlatform *mPlatform;
	MyGUI::VectorWidgetPtr mMainMenuLayout;
	MyGUI::VectorWidgetPtr mInGameMenuLayout;

	Ogre::SceneManager *mSceneManager;
	Ogre::RenderWindow *mRenderWindow;
};

#endif