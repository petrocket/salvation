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
	Ship(bool enemy = false);
	virtual ~Ship(void);

	virtual void reset(); // reset to default

	void setSpecsForSector(int sector);
	
	void updateSpecs();

	float maxJumpRangeForEngineLevel(int level);
	float fireRateForLevel(int level);
	float shieldStrengthForLevel(int level);
	float shieldRechargeRateForLevel(int level);
	float missChanceForLevel(int level);
	float weaponDamageForLevel(int level);

	float fire(double dt);
	void prepForBattle();
	void takeDamage(double amt);
	void rechargeShields(double dt);

	void update(double dt);

	float mFuel; // fuel

	// SYSTEMS
	int mEngineLevel;	// upgrade engine to boost range
	float mEngineDamage; // engine damage 0 .. 100
	float mMaxJumpRange; // jump range

	int mShieldLevel; // upgrade shield strength and recharge rate
	float mShieldStrength; // shield strength
	float mShieldRechargeRate; // shield recharge rate
	float mShieldDamage; // damage to shield generator
	float mShields; // shield level

	int mHullLevel; // hull upgrade
	float mHullStrength; // current hull strength

	int mWeaponsLevel; // weapons level (strength & recharge)
	float mWeaponsDamage; // damage to weapons system
	float mFireRate; // how fast the weapons can fire
	double mWeaponVisibleRemaining;
	double mWeaponCooldownRemaining;

	bool mStatusUpdated;

	Laser *mLaser;

	Ogre::BillboardSet *mRangeBillboardSet;
	Ogre::Billboard *mRangeBillboard;

	Ogre::SceneNode *mSceneNode;
	Ogre::Entity *mEntity;
	Ogre::Light *mLaserLight;

};

#endif
