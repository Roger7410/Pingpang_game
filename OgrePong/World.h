#ifndef __World_h_
#define __World_h_

// Forward delcarations of Ogre classes.  Note the Ogre namespace!
namespace Ogre {
    class SceneNode;
    class SceneManager;
}

// Forward delcarations of our own classes
class PongCamera;
class InputHandler;


class World
{
public:
	
    World(Ogre::SceneManager *sceneManager, InputHandler *input);

    // You'll want various methods to access & change your world here
    
    void Think(float time);
	
	void addCamera(PongCamera *c) { mCamera = c; }

	Ogre::SceneManager *SceneManager() { return mSceneManager; }


protected:


	Ogre::SceneManager *mSceneManager;

	InputHandler *mInputHandler;
	PongCamera *mCamera;

	// Here is where you keep all your world data.
	//  You probably want to use containers (arrays / lists / classes / etc) to ogranize them, 
	//    instead of a whole list of variables.  

	Ogre::SceneNode *pongBall;
	Ogre::SceneNode *leftBaffler;
	Ogre::SceneNode *rightBaffler;
	//for level 4 - 6
	Ogre::SceneNode *obstacle1;
	Ogre::SceneNode *obstacle2;
	Ogre::SceneNode *obstacle_hide;
	float LEFT_SPEED;
	float BALL_SPEED;
	float X_SPEED;
	float Y_SPEED;
	int PLAYER_SCORE;
	int AI_SCORE;
	int AI_TMP;
	int LEVEL;
	int LIFE;
	int MAKE_IT_ONCE;

};

#endif