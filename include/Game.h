#ifndef __Game_h_
#define __Game_h_

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// using pch
#include "stdafx.h"

#else

#include "Ogre.h"

#endif

class Game
{
public:
    Game(Ogre::SceneManager *mgr);
    virtual ~Game(void);

    void createScene(void);

	Ogre::SceneManager *mSceneManager;
};

#endif