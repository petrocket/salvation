
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>
#include <OgreSingleton.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <SdkTrays.h>
#include <SdkCameraMan.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
#include <math.h>
#include <time.h>

#include <SFML/Audio.hpp>

#include "MyGUI.h"
#include "MyGUI_OgrePlatform.h"
#include "BaseLayout/BaseLayout.h"

#include "TutorialApplication.h"
#include "BaseApplication.h"
#include "Game.h"
#include "Laser.h"
#include "GameAudioInterface.h"
#include "GameButton.h"
#include "GameConfig.h"
#include "InGameMenu.h"
#include "LensFlare.h"
#include "MainMenu.h"
#include "Planet.h"
#include "Ship.h"
#include "OgreUtil.h"
