#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// using pch
#include "stdafx.h"
#else

#include "Planet.h"
#endif



Planet::Planet(Ogre::SceneNode *parent)
{
	mSceneNode = parent->createChildSceneNode();
	mSurface = Game::getSingleton().mSceneManager->createEntity(Ogre::SceneManager::PT_SPHERE);
	mSceneNode->setScale(0.4f,0.4f,0.4f);
	mSceneNode->attachObject(mSurface);
}


Planet::~Planet(void)
{
}
