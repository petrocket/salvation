#ifndef __SHIP_H_
#define __SHIP_H_

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// using pch
#include "stdafx.h"
#else

#endif

class Ship
{
public:
	Ship(void);
	virtual ~Ship(void);

	virtual void reset(); // reset to default

	float maxJumpRangeForEngineLevel(int level);

	float mHealth; // health 0 .. 100

	// SYSTEMS
	int mEngineLevel;	// upgrade engine to boost range
	float mEngineDamage; // engine damage 0 .. 100
	float mMaxJumpRange; // jump range

	int mShieldLevel; // upgrade shield strength and recharge rate
	float mShieldDamage; // damage to shield generator
	float mShields; // shield level

	int mHullLevel; // hull upgrade
	float mHullStrength; // current hull strength

	int mWeaponsLevel; // weapons level (strength & recharge)
	float mWeaponsDamage; // damage to weapons system
};

#endif
