#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// using pch
#include "stdafx.h"
#else

#include "Ship.h"
#endif


Ship::Ship(bool enemy):
	mStatusUpdated(false),
	mRangeBillboardSet(0)
{
	reset();

	if(!enemy) {
		mRangeBillboardSet = Game::getSingleton().mSceneManager->createBillboardSet();
		mRangeBillboardSet->setAutoUpdate(true);
		mRangeBillboardSet->setDefaultDimensions(mMaxJumpRange * 2.0,mMaxJumpRange * 2.0);

		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName("RangeMaterial");
		mRangeBillboardSet->setMaterial(mat);
		Ogre::SceneNode *n = Game::getSingleton().mSceneManager->getRootSceneNode()->createChildSceneNode();
		n->attachObject(mRangeBillboardSet);

		mRangeBillboard = mRangeBillboardSet->createBillboard(Ogre::Vector3::ZERO);
	}
}


Ship::~Ship(void)
{
}


void Ship::reset()
{
	mHealth = 100;
	
	mFuel = 0;

	// upgrade level
	mEngineLevel = 1;
	mShieldLevel = 1;
	mHullLevel = 1;
	mWeaponsLevel = 1;

	// health of each system
	mEngineDamage = 0;
	mShieldDamage = 0;
	mWeaponsDamage = 0;
	mHullStrength = 100;

	mMaxJumpRange = maxJumpRangeForEngineLevel(mEngineLevel);
}

float Ship::maxJumpRangeForEngineLevel(int level)
{
	if(level <= 1) {
		return 600.0;
	}
	else if(level <= 2) {
		return 600.0;
	}
	else {
		return 1000.0;
	}
}

void Ship::setSpecsForSector(int sector)
{
	if(sector == 0) {
		mHealth = 100;
		mShieldLevel = 1;
		mWeaponsLevel = 1;
		mHullStrength = 100;
	}
	else if(sector == 1) {
		mHealth = 100;
		mShieldLevel = 1 + (Ogre::Math::UnitRandom() > 0.5 ? 1 : 0);
		mWeaponsLevel = 1 + (Ogre::Math::UnitRandom() > 0.5 ? 1 : 0);
		mHullStrength = 200;
	}
	else {
		// max level
		mHealth = 100;
		mShieldLevel = 2 + (Ogre::Math::UnitRandom() > 0.5 ? 1 : 0);
		mWeaponsLevel = 2 + (Ogre::Math::UnitRandom() > 0.5 ? 1 : 0);
		mHullStrength = 300;
	}
}