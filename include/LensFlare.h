/* ------------------------------------------------------------------------- */
 // File       : LensFlare.h
 // Project    : Long Forgotten Earth
 // Author     : David de Lorenzo
 /* ------------------------------------------------------------------------- */
 #ifndef _LENSFLARE_H_
 #define _LENSFLARE_H_
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	// using pch
	#include "stdafx.h"
#else
 
	 #include "ogre.h"

 #endif

/**
2011 - BHawk3D
Based on Lensflare code by David De Lorenzo. http://www.ogre3d.org/tikiwiki/LensFlare.cpp
Source code, material file, and art is public domain
**/
class LensFlare {
  private:
  Ogre::SceneManager* mSceneManager;
  Ogre::Camera*       mCamera;
  Ogre::SceneNode*    mNode;
  Ogre::BillboardSet* mHaloSet;
  Ogre::BillboardSet* mCircleSet;
  Ogre::BillboardSet* mBurstSet;
  Ogre::Vector3       mLightPosition;
  Ogre::Real          mScale;
  Ogre::Real          mWidth;
  Ogre::Real          mHeight;
  int                 mNumHalos;
  int                 mNumCircles;
  int                 m2DLightLocationX;
  int                 m2DLightLocationY;
  Ogre::Real          mLensFlareBaseSize;
  bool                mVisible;
  Ogre::ColourValue   mBaseHaloColor;
  Ogre::ColourValue   mBaseCircleColor;
  bool				  mFlareVisible;
  void Reset();
  Ogre::Vector3 GetLensFlareDirectionVector();
  void SetLensFlareLocations(Ogre::Vector3 dir);
  void CalculateLensFlareScreenLocation();
  void CalculateLensFlareScale();
  void SetLensFlareAlpha();
  void SetLensFlareSize();
  void RotateLensFlareSource();
  Ogre::Real GetScale();
  
public:
  LensFlare();
  ~LensFlare();
  void Init(Ogre::SceneManager* sceneManager,
            Ogre::Camera* camera,       
            Ogre::Real width, Ogre::Real height,
            Ogre::Real lensFlareSourceSize,
            Ogre::Real lensFlareBaseSize,
            Ogre::String strHaloMaterialName,
            Ogre::String strCircleMaterialName,
            Ogre::String strSourceMaterialName);
  void Update();
  void End();

  void SetPosition(Ogre::Vector3 pos);
  void SetVisible(bool visible);
  bool GetVisible();

  void SetFlareVisible(bool visible);

  void SetHaloColour(Ogre::ColourValue color);
  void SetCircleColour(Ogre::ColourValue color);
  void SetBurstColour(Ogre::ColourValue color);
};

#endif