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

Planet::Planet(Ogre::SceneNode *parent)
{
	if(!gSphereMeshCreated) {
		gSphereMeshCreated = true;
		createSphere("planetSphere", 50, 64, 64);
	}

	mSceneNode = parent->createChildSceneNode();
	//mSurface = Game::getSingleton().mSceneManager->createEntity(Ogre::SceneManager::PT_SPHERE);
	mSurface = Game::getSingleton().mSceneManager->createEntity("planetSphere");
	if(Ogre::Math::UnitRandom() > 0.5) {
		mSurface->setMaterialName("PlanetMaterial1");
	}
	else {
		mSurface->setMaterialName("PlanetMaterial2");
	}
	mSceneNode->setScale(0.05f,0.05f,0.05f);
	mSceneNode->attachObject(mSurface);
	//mSceneNode->showBoundingBox(true);
}


Planet::~Planet(void)
{
}
