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
	mSceneNode->setScale(0.05f,0.05f,0.05f);
	mSceneNode->attachObject(mSurface);
	//mSceneNode->showBoundingBox(true);
}


Planet::~Planet(void)
{
}
