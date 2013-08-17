#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// using pch
#include "stdafx.h"
#else

#include "Ship.h"
#endif


Ship::Ship(bool enemy):
	mStatusUpdated(false),
	mRangeBillboardSet(0),
	mWeaponCooldownRemaining(0),
	mWeaponVisibleRemaining(0),
	mLaserLight(0)
{
	reset();

	Ogre::SceneManager *mgr = Game::getSingleton().mSceneManager;
	if(enemy) {
		mSceneNode = mgr->getRootSceneNode()->createChildSceneNode();
		mEntity = mgr->createEntity("EnemyShip.mesh");
		mSceneNode->attachObject(mEntity);

		GameNode *n = Game::getSingleton().mGameNodes[Game::getSingleton().mCurrentNodeIdx];
		if(n->planet) {
			mSceneNode->getParentSceneNode()->removeChild(mSceneNode);
			n->planet->mSceneNode->addChild(mSceneNode);
			mSceneNode->setPosition(-5.0f,0.4f,-2.2f);
			mSceneNode->setScale(0.1f,0.1f,0.1f);
			mSceneNode->yaw(Ogre::Degree(-90.0));
		}
		else {
			mSceneNode->getParentSceneNode()->removeChild(mSceneNode);
			n->scenenode->addChild(mSceneNode);
			mSceneNode->setPosition(0.0,0.4,2.5);
			mSceneNode->setScale(0.1f,0.1f,0.1f);
			mSceneNode->yaw(Ogre::Degree(90.0));
		}

		mLaser = new Laser(0.35,4,"EnemyLaserMaterial");
		mLaser->createManualObject();
		mLaser->mSceneNode = mSceneNode->createChildSceneNode();
		mLaser->fire(Ogre::Vector3(0,0.5,0),Ogre::Vector3(50.0,0.5,0));

		mLaserLight = mgr->createLight();
		mLaserLight->setType(Ogre::Light::LT_POINT);
		//mLaserLight->setAttenuation(7.0f, 1.0f, 0.7f, 1.8f);
		mLaserLight->setPosition(Ogre::Vector3(10.0f,0,0));
		mLaserLight->setDiffuseColour(Ogre::ColourValue(1.0,0,0));
		mSceneNode->attachObject(mLaserLight);
	}
	else {
		mRangeBillboardSet = mgr->createBillboardSet();
		mRangeBillboardSet->setAutoUpdate(true);
		mRangeBillboardSet->setDefaultDimensions(
			mMaxJumpRange * 2.0,
			mMaxJumpRange * 2.0);

		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName(
			"RangeMaterial");
		mRangeBillboardSet->setMaterial(mat);

		Ogre::SceneNode *n = mgr->getRootSceneNode()->createChildSceneNode();
		n->attachObject(mRangeBillboardSet);

		mRangeBillboard = mRangeBillboardSet->createBillboard(Ogre::Vector3::ZERO);

		mSceneNode = mgr->getRootSceneNode()->createChildSceneNode();
		mEntity = mgr->createEntity("PlayerShip.mesh");
		mSceneNode->attachObject(mEntity);

		mLaser = new Laser(0.35,4,"PlayerLaserMaterial");
		mLaser->createManualObject();
		mLaser->mSceneNode = mSceneNode->createChildSceneNode();
		mLaser->fire(Ogre::Vector3(0,0,0),Ogre::Vector3(50.0,0.0,0));

		mLaserLight = mgr->createLight();
		mLaserLight->setType(Ogre::Light::LT_POINT);

		//mLaserLight->setAttenuation(7.0f, 1.0f, 0.7f, 1.8f);
		//mLaserLight->setAttenuation(50, 1.0, 0.09, 0.032);
		
		mLaserLight->setPosition(Ogre::Vector3(10.0f,0,0));
		mLaserLight->setDiffuseColour(Ogre::ColourValue(0.0,0.5,1.0f));
		mSceneNode->attachObject(mLaserLight);

	}

	mLaserLight->setLightMask(1 << 1);
	mLaserLight->setVisible(false);
	mLaser->mSceneNode->setVisible(false);
}


Ship::~Ship(void)
{
	if(mLaserLight) {
		Game::getSingleton().mSceneManager->destroyLight(mLaserLight);
	}
}

float Ship::fireRateForLevel(int level)
{
	if(level <= 1) {
		return Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"fireRate1","weapons","1.0"));
	}
	else if(level == 2) {
		return Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"fireRate2","weapons","2.0"));
	}
	else {
		return Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"fireRate3","weapons","3.0"));
	}
}

float Ship::fire(double dt)
{
	// are weapons operational?
	if(mWeaponsDamage >= 100.0) {
		return 0.0;
	}

	// fire rate first
	mWeaponCooldownRemaining -= dt;
	if(mWeaponCooldownRemaining > 0) return 0.0;

	float missChance = missChanceForLevel(mWeaponsLevel);
	if(Ogre::Math::UnitRandom() < missChance) {
		// missed!
		return 0.0;
	}


	mWeaponCooldownRemaining = 1.0 / mFireRate;
	mLaserLight->setVisible(true);

	mWeaponVisibleRemaining =  mWeaponCooldownRemaining * 0.4;
	mLaser->mSceneNode->setVisible(true);

	// damage for level
	return weaponDamageForLevel(mWeaponsLevel);
}

void Ship::prepForBattle()
{
	mWeaponCooldownRemaining = 0;

	float maxStrength =  shieldStrengthForLevel(mShieldLevel) -
		mShieldDamage * Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"shieldStrengthDamageModifier","shields","0.5"));
	mShieldStrength = maxStrength;
}

void Ship::update(double dt)
{
	mWeaponVisibleRemaining -= dt;
	if(mWeaponVisibleRemaining <= 0) {
		mLaserLight->setVisible(false);
		mLaser->mSceneNode->setVisible(false);

	}
}
void Ship::rechargeShields(double dt)
{
	// are shields operational at all?
	if(mShieldDamage >= 100.0) {
		return;
	}

	// shields recharge at a rate
	float rechargeAmount = dt * mShieldRechargeRate;
	mShieldStrength += rechargeAmount;

	float maxStrength =  shieldStrengthForLevel(mShieldLevel) -
		mShieldDamage * Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"shieldStrengthDamageModifier","shields","0.5"));
	if(mShieldStrength > maxStrength) {
		mShieldStrength = maxStrength;
	}
}

void Ship::takeDamage(double amt)
{
	// shields first
	mShieldStrength -= amt;
	if(mShieldStrength > 0) {
		return;
	}

	float dmgAmount = -mShieldStrength;
	mShieldStrength = 0.0;

	// randomly affect systems
	float r = Ogre::Math::UnitRandom();
	if(r < 0.25) {
		mHullStrength -= dmgAmount;
		if(mHullStrength < 0.0) mHullStrength = 0.0;
	}
	else if(r < 0.5) {
		mShieldDamage += dmgAmount;
		if(mShieldDamage > 100.0) mShieldDamage = 100;
	}
	else if(r < 0.75) {
		mWeaponsDamage += dmgAmount;
		if(mWeaponsDamage > 100.0) mWeaponsDamage = 100.0;
	}
	else {
		mEngineDamage += dmgAmount;
		if(mEngineDamage > 100.0) mEngineDamage = 100.0;
	}
}

void Ship::reset()
{
	mFuel = 0.5;

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

	updateSpecs();
}

float Ship::maxJumpRangeForEngineLevel(int level)
{
	if(level <= 1) {
		return Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"range1","engines","300"));
	}
	else if(level <= 2) {
		return Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"range2","engines","600"));
	}
	else {
		return Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"range3","engines","1000"));
	}
}

float Ship::missChanceForLevel(int level)
{
	if(level <= 1) {
		return Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"missChance1","weapons","0.5"));
	}
	else if(level <= 2) {
		return Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"missChance2","weapons","0.3"));
	}
	else {
		return Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"missChance3","weapons","0.1"));
	}
}

void Ship::setSpecsForSector(int sector)
{
	mHullStrength = 100;
	if(sector == 0) {
		mShieldLevel = 1;
		mWeaponsLevel = 1;
	}
	else if(sector == 1) {
		mShieldLevel = 1 + (Ogre::Math::UnitRandom() > 0.5 ? 1 : 0);
		mWeaponsLevel = 1 + (Ogre::Math::UnitRandom() > 0.5 ? 1 : 0);
	}
	else {
		// max level
		mShieldLevel = 2 + (Ogre::Math::UnitRandom() > 0.5 ? 1 : 0);
		mWeaponsLevel = 2 + (Ogre::Math::UnitRandom() > 0.5 ? 1 : 0);
	}

	updateSpecs();
}

float Ship::shieldStrengthForLevel(int level)
{
	if(level <= 1) {
		return Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"shieldStrength1","shields","100"));
	}
	else if(level == 2) {
		return Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"shieldStrength2","shields","200"));
	}
	else {
		return Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"shieldStrength3","shields","300"));
	}
}

float Ship::shieldRechargeRateForLevel(int level)
{
	if(level <= 1) {
		return Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"shieldRechargeRate1","shields","0.5"));
	}
	else if(level == 2) {
		return Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"shieldRechargeRate2","shields","1.0"));
	}
	else {
		return Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"shieldRechargeRate3","shields","1.5"));
	}
}

void Ship::updateSpecs()
{
	mMaxJumpRange = maxJumpRangeForEngineLevel(mEngineLevel) - 
		mEngineDamage * Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"engineDamageModifier","engines","1.0"));

	mFireRate = fireRateForLevel(mWeaponsLevel) -
		mWeaponsDamage * Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"weaponsDamageModifier","weapons","0.01"));

	mShieldStrength = shieldStrengthForLevel(mShieldLevel) -
		mShieldDamage * Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"shieldStrengthDamageModifier","shields","0.5"));

	mShieldRechargeRate = shieldRechargeRateForLevel(mShieldLevel) -
		mShieldDamage * Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"shieldRechargeDamageModifier","shields","0.01"));

	if(mRangeBillboardSet) {
		mRangeBillboardSet->setDefaultDimensions(
			mMaxJumpRange * 2.0,
			mMaxJumpRange * 2.0);
	}
}

float Ship::weaponDamageForLevel(int level)
{
	if(level <= 1) {
		return Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"weaponsDamage1","weapons","10.0"));
	}
	else if(level == 2) {
		return Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"weaponsDamage2","weapons","20.0"));
	}
	else {
		return Ogre::StringConverter::parseReal(
			Game::getSingleton().mConfig->getSetting(
			"weaponsDamage3","weapons","30.0"));
	}
}