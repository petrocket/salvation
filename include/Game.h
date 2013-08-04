#ifndef __Game_h_
#define __Game_h_

#include "stdafx.h"

class Game
{
public:
    Game(Ogre::SceneManager *mgr);
    virtual ~Game(void);

    void createScene(void);

	Ogre::SceneManager *mSceneManager;
};

#endif