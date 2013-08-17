#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	// using pch
	#include "stdafx.h"
#else

	#include "Laser.h"
#endif

static int gNumLasers = 0;

Laser::Laser(Ogre::Real radius, Ogre::uint numPlanes, Ogre::String materialName) :
	mRadius(radius),
	mCreated(false),
	mNumberPlanes(numPlanes),
	mMaterialName(materialName),
	mQuadManualObject(0),
	mSceneNode(0)
{
}


Laser::~Laser(void)
{
	destroy();
}


Ogre::ManualObject* Laser::createManualObject()
{      
   if (mQuadManualObject) return mQuadManualObject;   
   mMaterial = Ogre::MaterialManager::getSingleton().getByName(mMaterialName);
  mQuadManualObject = Game::getSingleton().mSceneManager->createManualObject("LaserObject" + 
	  Ogre::StringConverter::toString(gNumLasers));
  gNumLasers++;
   
   mQuadManualObject->setDynamic(true);

   if (mSceneNode) {
      mSceneNode->attachObject(mQuadManualObject);
   }
   
   return mQuadManualObject;   
}

void Laser::fire(Ogre::Vector3 start, Ogre::Vector3 end)
{
   Ogre::Vector3 dir = end - start;
   Ogre::Real dist = dir.normalise();


   Ogre::Vector3 vec_pattern(0, mRadius/2.0f, 0);

   Ogre::Quaternion rot;
   rot.FromAngleAxis(Ogre::Degree(180 / mNumberPlanes), Ogre::Vector3::UNIT_Z);
   
   if ((mQuadManualObject->getDynamic() == true) && (mCreated == true))
      mQuadManualObject->beginUpdate(0);
   else
      mQuadManualObject->begin(mMaterial->getName(),Ogre::RenderOperation::OT_TRIANGLE_LIST);

   for (Ogre::uint i = 0; i < mNumberPlanes; ++i)
   {
      mQuadManualObject->position(-vec_pattern.x, -vec_pattern.y, dist);
        mQuadManualObject->textureCoord(0, 0);

        mQuadManualObject->position(vec_pattern.x, vec_pattern.y, dist);  
        mQuadManualObject->textureCoord(1, 0);

        mQuadManualObject->position(-vec_pattern.x, -vec_pattern.y, 0);          
        mQuadManualObject->textureCoord(0, 1);

        mQuadManualObject->position(vec_pattern.x, vec_pattern.y, 0);         
        mQuadManualObject->textureCoord(1, 1);
            
        int offset = i * 4;
        mQuadManualObject->triangle(offset, offset+3, offset+1);
        mQuadManualObject->triangle(offset, offset+2, offset+3);
		
		mQuadManualObject->triangle(offset+1, offset+3, offset+2);
		mQuadManualObject->triangle(offset, offset+1, offset+2);

      vec_pattern = rot * vec_pattern;         
   }

   mQuadManualObject->end();          

   if (mSceneNode)
   {
      if (!mCreated)
         mSceneNode->attachObject(mQuadManualObject);

      Ogre::Vector3 src = mSceneNode->getOrientation() * Ogre::Vector3::UNIT_Z;

      Ogre::Quaternion quat = src.getRotationTo(dir);
      

      mSceneNode->rotate(quat);
      mSceneNode->translate(start);

      //mSceneNode->showBoundingBox(true);
   }
   mCreated = true;
}

void Laser::destroy()
{
   if (mQuadManualObject)
   {
      if (mQuadManualObject->getParentSceneNode())
         mQuadManualObject->getParentSceneNode()->detachObject(mQuadManualObject);

	  Game::getSingleton().mSceneManager->destroyManualObject(mQuadManualObject);
      mQuadManualObject = 0;
   }
   
   if (mSceneNode)
   {
      mSceneNode->removeAndDestroyAllChildren();
      mSceneNode->getParentSceneNode()->removeAndDestroyChild(mSceneNode->getName());
      mSceneNode = 0;
   }

   mMaterial.setNull();
   mCreated = false;
}
