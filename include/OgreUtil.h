#ifndef __OGRE_UTIL_H_
#define __OGRE_UTIL_H_

class OgreUtil
{
public:
	static void destroyAllAttachedMovableObjects( Ogre::SceneNode* node )
	{
	   if(!node) return;

	   // Destroy all the attached objects
	   Ogre::SceneNode::ObjectIterator itObject = node->getAttachedObjectIterator();

	   while ( itObject.hasMoreElements() )
		  node->getCreator()->destroyMovableObject(itObject.getNext());

	   // Recurse to child SceneNodes
	   Ogre::SceneNode::ChildNodeIterator itChild = node->getChildIterator();

	   while ( itChild.hasMoreElements() )
	   {
		  Ogre::SceneNode* pChildNode = static_cast<Ogre::SceneNode*>(itChild.getNext());
		  OgreUtil::destroyAllAttachedMovableObjects( pChildNode );
	   }
	}

	static void destroySceneNode( Ogre::SceneNode* node )
	{
	   if(!node) return;
	   OgreUtil::destroyAllAttachedMovableObjects(node);
	   node->removeAndDestroyAllChildren();
	   node->getCreator()->destroySceneNode(node);
	}
};
#endif