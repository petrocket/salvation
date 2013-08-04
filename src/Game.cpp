#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// using pch
#include "stdafx.h"

#else

#include "Game.h"

#endif

//-------------------------------------------------------------------------------------
Game::Game(Ogre::SceneManager *mgr) :
	mSceneManager(mgr)
{
}

//-------------------------------------------------------------------------------------
Game::~Game(void)
{
}


//-------------------------------------------------------------------------------------
void Game::createScene(void)
{
	mSceneManager->setSkyBox(true, "Spacescape1024");
}