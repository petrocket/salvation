/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#include "stdafx.h"

//-------------------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void) : 
	mGame(0)
{
}
//-------------------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
	if(mGame) {
		delete mGame;
		mGame = 0;
	}
}

//-------------------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
    // create your scene here :)
	if(!mGame) {
		mGame = new Game(mSceneMgr,mWindow, mCamera);
	}

	mGame->createScene();
}

//-------------------------------------------------------------------------------------
bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	bool result = BaseApplication::frameRenderingQueued(evt);

	if(mGame) {
		mGame->update(evt.timeSinceLastFrame);
	}

	return result;
}

//-------------------------------------------------------------------------------------
bool TutorialApplication::keyPressed( const OIS::KeyEvent &arg )
{
	//BaseApplication::keyPressed(arg);
	return mGame->keyPressed(arg);
}

bool TutorialApplication::keyReleased( const OIS::KeyEvent &arg )
{
	//BaseApplication::keyReleased(arg);
	return mGame->keyReleased(arg);
}

bool TutorialApplication::mouseMoved( const OIS::MouseEvent &arg )
{
	//BaseApplication::mouseMoved(arg);
	return mGame->mouseMoved(arg);
}

bool TutorialApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	//BaseApplication::mousePressed(arg,id);
	return mGame->mousePressed(arg,id);
}

bool TutorialApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	//BaseApplication::mouseReleased(arg,id);
	return mGame->mouseReleased(arg,id);
}



#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
