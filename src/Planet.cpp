#include "OgrePlatform.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
// using pch
#include "stdafx.h"
#else

#include "Planet.h"
#endif

using namespace Ogre;

void createSphere(const std::string& strName, const float r, const int nRings = 16, const int nSegments = 16)
 {
	 ManualObject * manual = Game::getSingleton().mSceneManager->createManualObject(strName);
     manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
 
     float fDeltaRingAngle = (Math::PI / nRings);
     float fDeltaSegAngle = (2 * Math::PI / nSegments);
     unsigned short wVerticeIndex = 0 ;
 
     // Generate the group of rings for the sphere
     for( int ring = 0; ring <= nRings; ring++ ) {
         float r0 = r * sinf (ring * fDeltaRingAngle);
         float y0 = r * cosf (ring * fDeltaRingAngle);
 
         // Generate the group of segments for the current ring
         for(int seg = 0; seg <= nSegments; seg++) {
             float x0 = r0 * sinf(seg * fDeltaSegAngle);
             float z0 = r0 * cosf(seg * fDeltaSegAngle);
 
             // Add one vertex to the strip which makes up the sphere
             manual->position( x0, y0, z0);
             manual->normal(Vector3(x0, y0, z0).normalisedCopy());
             manual->textureCoord((float) seg / (float) nSegments, (float) ring / (float) nRings);
 
             if (ring != nRings) {
                 // each vertex (except the last) has six indicies pointing to it
                 manual->index(wVerticeIndex + nSegments + 1);
                 manual->index(wVerticeIndex);               
                 manual->index(wVerticeIndex + nSegments);
                 manual->index(wVerticeIndex + nSegments + 1);
                 manual->index(wVerticeIndex + 1);
                 manual->index(wVerticeIndex);
                 wVerticeIndex ++;
                 }
         }; // end for seg
     } // end for ring
     manual->end();
     MeshPtr mesh = manual->convertToMesh(strName);
     mesh->_setBounds( AxisAlignedBox( Vector3(-r, -r, -r), Vector3(r, r, r) ), false );
 
     mesh->_setBoundingSphereRadius(r);
        unsigned short src, dest;
        if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
        {
                  mesh->buildTangentVectors(VES_TANGENT, src, dest);
        }
 }

static bool gSphereMeshCreated = false;

Planet::Planet(Ogre::SceneNode *parent) :
	mRotateSpeed(0.025f)
{
	if(!gSphereMeshCreated) {
		gSphereMeshCreated = true;
		createSphere("planetSphere", 50, 64, 64);
	}

	mSceneNode = parent->createChildSceneNode();

	mCloudsNode = mSceneNode->createChildSceneNode();
	mSurfaceNode = mSceneNode->createChildSceneNode();

	//mSurface = Game::getSingleton().mSceneManager->createEntity(Ogre::SceneManager::PT_SPHERE);
	mSurface = Game::getSingleton().mSceneManager->createEntity("planetSphere");
	mSurface->setLightMask(1 << 0);
	int idx = floorf(Ogre::Math::RangeRandom(1.1f, 7.9f));
	mSurface->setMaterialName("PlanetTerrainMaterial" + 
		Ogre::StringConverter::toString(idx));
	mSurfaceNode->setScale(0.05f,0.05f,0.05f);
	mSurfaceNode->attachObject(mSurface);

	mClouds = Game::getSingleton().mSceneManager->createEntity("planetSphere");
	mClouds->setMaterialName("PlanetCloudsMaterial");
	mCloudsNode->setScale(0.0505f,0.0505f,0.0505f);
	mClouds->setLightMask(1 << 0);
	if(idx != 7) {
		mCloudsNode->attachObject(mClouds);
	}
}

Planet::~Planet(void)
{
}

void Planet::update(float dt)
{
	mSceneNode->yaw(Ogre::Radian(dt * mRotateSpeed));
}

