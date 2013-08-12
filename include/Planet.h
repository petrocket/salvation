#ifndef __PLANET_H_
#define __PLANET_H_

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// using pch
#include "stdafx.h"
#else

#endif
class Planet
{
public:
	Planet(Ogre::SceneNode *parent);
	virtual ~Planet(void);

	Ogre::SceneNode *mSceneNode;
	Ogre::Entity *mSurface;
};

#endif