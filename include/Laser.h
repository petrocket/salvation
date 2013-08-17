#ifndef __LASER_H_
#define __LASER_H_

class Laser
{
public:
	Laser(Ogre::Real radius, Ogre::uint numPlanes, Ogre::String materialName);
	virtual ~Laser(void);

   Ogre::ManualObject* createManualObject();
   void fire(Ogre::Vector3 start, Ogre::Vector3 end);
   void destroy();

   Ogre::SceneNode* mSceneNode;

   Ogre::Real mRadius;
   Ogre::uint mNumberPlanes;
   Ogre::ManualObject* mQuadManualObject;
   Ogre::MaterialPtr mMaterial;
   Ogre::String mMaterialName;
   bool mCreated;

};

#endif