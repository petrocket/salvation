#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// using pch
#include "stdafx.h"
#else

#include "Ship.h"
#endif


Ship::Ship(void)
{
	reset();
}


Ship::~Ship(void)
{
}


void Ship::reset()
{
	mHealth = 100;

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
		return 300.0;
	}
	else if(level <= 2) {
		return 600.0;
	}
	else {
		return 1000.0;
	}
}