/* ------------------------------------------------------------------------- */
 // File       : LensFlare.cpp
 // Project    : Long Forgotten Earth
 // Author     : David de Lorenzo
 /* ------------------------------------------------------------------------- */
 
#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	// using pch
	#include "stdafx.h"
#else

	#include "LensFlare.h"
#endif

LensFlare::LensFlare() {
  Reset();
}

LensFlare::~LensFlare() { } 

void LensFlare::Init(Ogre::SceneManager* sceneManager,
                     Ogre::Camera* camera, 
                     Ogre::Real width, Ogre::Real height,
                     Ogre::Real lensFlareSourceSize,
                     Ogre::Real lensFlareBaseSize,
                     Ogre::String strHaloMaterialName,
                     Ogre::String strCircleMaterialName,
                     Ogre::String strSourceMaterialName) {
  
  mSceneManager = sceneManager;
  mCamera = camera;
  mWidth = width;
  mHeight = height;
  mLensFlareBaseSize = lensFlareBaseSize;

  mHaloSet = mSceneManager->createBillboardSet();
  mHaloSet->setMaterialName(strHaloMaterialName);
  mHaloSet->setCullIndividually(true);

  mCircleSet = mSceneManager->createBillboardSet();
  mCircleSet->setMaterialName(strCircleMaterialName);
  mCircleSet->setCullIndividually(true);

  mBurstSet= mSceneManager->createBillboardSet();
  mBurstSet->setMaterialName(strSourceMaterialName);
  mBurstSet->setCullIndividually(true);

  mNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
  mNode->attachObject(mBurstSet);
  mNode->attachObject(mCircleSet);
  mNode->attachObject(mHaloSet);

  mNumHalos = 3;
  for(int i = 0; i < mNumHalos; i++)
    mHaloSet->createBillboard(0, 0, 0);

  mNumCircles = 12;
  for(int i = 0; i < mNumCircles; i++)
    mCircleSet->createBillboard(0, 0, 0);

  //This will represent the source of the light (i.e. the sun)
  mBurstSet->createBillboard(0, 0, 0);
  mBurstSet->getBillboard(0)->setDimensions(lensFlareSourceSize, lensFlareSourceSize);
  mBurstSet->getBillboard(0)->setColour(Ogre::ColourValue(1, 1, 1, 1));

  mHaloSet->setRenderQueueGroup(Ogre::RENDER_QUEUE_SKIES_LATE);
  mCircleSet->setRenderQueueGroup(Ogre::RENDER_QUEUE_SKIES_LATE);
  mBurstSet->setRenderQueueGroup(Ogre::RENDER_QUEUE_SKIES_LATE);

  mScale = 1;
}

void LensFlare::End() {
	/*
  mNode->detachObject(mHaloSet);
  mNode->detachObject(mCircleSet);
  mNode->detachObject(mBurstSet);
  mSceneManager->destroyBillboardSet(mHaloSet);
  mSceneManager->destroyBillboardSet(mCircleSet);
  mSceneManager->destroyBillboardSet(mBurstSet);
  mSceneManager->destroySceneNode(mNode);
  */
  Reset();
}

void LensFlare::Reset() {
  mSceneManager = NULL;
  mCamera = NULL;
  mNode = NULL;
  mHaloSet = NULL;
  mCircleSet = NULL;
  mBurstSet = NULL;
  mLightPosition = Ogre::Vector3::ZERO;
  mScale = 0;
  mWidth = 0;
  mHeight = 0;
  mNumHalos = 0;
  mNumCircles = 0;
  m2DLightLocationX = 0;
  m2DLightLocationY = 0;
  mLensFlareBaseSize = 0;
  mFlareVisible = true;
  mBaseHaloColor = Ogre::ColourValue(1, 1, 1, 1);
  mBaseCircleColor = Ogre::ColourValue(1, 1, 1, 1);
  mVisible = false;
}

void LensFlare::Update() {
  
	if(!mFlareVisible) return;

  //If the Light is out of the Camera field Of View, the lensflare is hidden.
  if (!mCamera->isVisible(mLightPosition)) {
    SetVisible(false);
    mScale = 0;
    return;
  }

  //TODO: If there is an entity between the camera and the lensflare source the
  //Lensflare is hidden
  /*if(ObjectBlockingFlare()) {
    SetVisible(false);
    mScale = 0;
    return;
  }*/

  SetVisible(true);

  CalculateLensFlareScreenLocation();
  SetLensFlareLocations(GetLensFlareDirectionVector());
  CalculateLensFlareScale(); //mScale will be 0 - 1
  SetLensFlareAlpha();
  SetLensFlareSize();
  RotateLensFlareSource();
}

Ogre::Vector3 LensFlare::GetLensFlareDirectionVector() {
  Ogre::Vector3 camPos = mCamera->getDerivedPosition();
  Ogre::Real lightDistance = (camPos - mLightPosition).length();
  Ogre::Vector3 cameraVect = camPos + (lightDistance * mCamera->getDirection());
  return cameraVect - mLightPosition;
}

void LensFlare::SetLensFlareLocations(Ogre::Vector3 dir) {
  //Flares behind the Sun
  mCircleSet->getBillboard(0)->setPosition(dir * -0.4f); 
  mHaloSet->getBillboard(0)->setPosition(dir * -0.3f);

  //Sun
  mBurstSet->getBillboard(0)->setPosition(Ogre::Vector3::ZERO);

  //Five Circles and a gap
  mCircleSet->getBillboard(1)->setPosition(dir * 0.1f);
  mCircleSet->getBillboard(2)->setPosition(dir * 0.2f);
  mCircleSet->getBillboard(3)->setPosition(dir * 0.4f);
  mCircleSet->getBillboard(4)->setPosition(dir * 0.45f);
  mCircleSet->getBillboard(5)->setPosition(dir * 0.5f);

  mCircleSet->getBillboard(6)->setPosition(dir * 0.8f);
  mCircleSet->getBillboard(7)->setPosition(dir * 0.9f);
  mCircleSet->getBillboard(8)->setPosition(dir * 1.0f);
  mCircleSet->getBillboard(9)->setPosition(dir * 1.1f);
  mCircleSet->getBillboard(10)->setPosition(dir * 1.2f);
  mCircleSet->getBillboard(11)->setPosition(dir * 1.3f);

  //Two Halos
  mHaloSet->getBillboard(1)->setPosition(dir * 1.8f);
  mHaloSet->getBillboard(2)->setPosition(dir * 2.1f);
}

void LensFlare::CalculateLensFlareScreenLocation() {
  Ogre::Vector2 r;
  Ogre::Vector3 point = mCamera->getProjectionMatrix() * 
                        (mCamera->getViewMatrix() * mLightPosition);
  r.x = (point.x / 2) + 0.5f;
  r.y = 1 - ((point.y / 2) + 0.5f);
  m2DLightLocationX = (int)(r.x * mWidth);
  m2DLightLocationY = (int)(r.y * mHeight);
}

void LensFlare::CalculateLensFlareScale() {
  //2D coordinates of the source of the lens flare
  Ogre::Vector2 currentLocation;
  //2D coordinates of the center of the screen
  Ogre::Vector2 centerLocation;

  //Claculete the distance between the center of the screen and a corner
  currentLocation.x = mWidth - 1;
  currentLocation.y = mHeight - 1;
  centerLocation.x = mWidth / 2;
  centerLocation.y = mHeight / 2;
  Ogre::Real maxDist = (currentLocation - centerLocation).length();

  //Calculate the distance from the center of the screen to the
  //Lens Flare source location
  currentLocation.x = m2DLightLocationX;
  currentLocation.y = m2DLightLocationY;
  Ogre::Real dist = (currentLocation - centerLocation).length();

  mScale = (1 - (dist / maxDist));
}

void LensFlare::SetLensFlareAlpha() {
  Ogre::Real total = mHaloSet->getNumBillboards();
  for(int i = 0; i < total; i++) {
    mHaloSet->getBillboard(i)->setColour(mBaseHaloColor * mScale);
  }

  total = mCircleSet->getNumBillboards();
  for(int i = 0; i < total; i++) {
    mCircleSet->getBillboard(i)->setColour(mBaseCircleColor * mScale);
  }
}

void LensFlare::SetLensFlareSize() {
  Ogre::Real size = mLensFlareBaseSize * mScale; 

  //Flares behind the Sun
  mCircleSet->getBillboard(0)->setDimensions(size * 2, size * 2); 
  mHaloSet->getBillboard(0)->setDimensions(size * 3, size * 3);

  //Five Circles and a gap
  mCircleSet->getBillboard(1)->setDimensions(size * 2, size * 2);
  mCircleSet->getBillboard(2)->setDimensions(size * 3, size * 3);
  mCircleSet->getBillboard(3)->setDimensions(size * 2.2, size * 2.2);
  mCircleSet->getBillboard(4)->setDimensions(size * 0.5, size * 0.5);
  mCircleSet->getBillboard(5)->setDimensions(size, size);

  mCircleSet->getBillboard(6)->setDimensions(size, size);
  mCircleSet->getBillboard(7)->setDimensions(size * 4, size * 4);
  mCircleSet->getBillboard(8)->setDimensions(size * 2, size * 2);
  mCircleSet->getBillboard(9)->setDimensions(size * 3, size * 3);
  mCircleSet->getBillboard(10)->setDimensions(size * 2, size * 2);
  mCircleSet->getBillboard(11)->setDimensions(size * 1.5, size * 1.5);

  //Two Halos
  mHaloSet->getBillboard(1)->setDimensions(size * 7, size * 7);
  mHaloSet->getBillboard(2)->setDimensions(size * 12, size * 12);
}

void LensFlare::RotateLensFlareSource() {
  Ogre::Vector2 source(m2DLightLocationX, m2DLightLocationY);
  Ogre::Vector2 dest(mWidth - 1, mHeight - 1);
  Ogre::Real rotMaxDist = dest.length();
  Ogre::Real rotDist = (dest - source).length();
  Ogre::Real rotScale = (1 - (rotDist / rotMaxDist));

  mBurstSet->getBillboard(0)->setRotation(Ogre::Radian(Ogre::Degree(90 * rotScale)));
}

void LensFlare::SetVisible(bool visible) {
  mHaloSet->setVisible(visible);
  mCircleSet->setVisible(visible);
  //mBurstSet->setVisible(visible);
  mVisible = visible;
}

bool LensFlare::GetVisible() {
  return mVisible;
}

void LensFlare::SetHaloColour(Ogre::ColourValue color) { 
  mBaseHaloColor = color;
}

void LensFlare::SetCircleColour(Ogre::ColourValue color) { 
  mBaseCircleColor = color; 
}

void LensFlare::SetBurstColour(Ogre::ColourValue color) {
  Ogre::Real total = mBurstSet->getNumBillboards();
  for(int i = 0; i < total; i++) {
    mBurstSet->getBillboard(i)->setColour(color);
  }
} 

void LensFlare::SetFlareVisible(bool visible) {
	mFlareVisible = visible;
	SetVisible(visible);
}

void LensFlare::SetPosition(Ogre::Vector3 pos) { 
  mLightPosition = pos;
  mNode->setPosition(mLightPosition); 
}

Ogre::Real LensFlare::GetScale() { 
  return mScale;
}