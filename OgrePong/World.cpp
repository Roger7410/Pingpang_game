// My header file.  This should go first!
#include "World.h"
#include <ctime>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <string>
#include <sstream>

// Ogre header files
#include "Ogre.h"
#include "OgreMath.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "OgreOverlayManager.h"
#include "OgreOverlay.h"
#include "OgreFontManager.h"
#include "OgreTextAreaOverlayElement.h"
#include "OgrePong.h"

// IOS (Input system) header files

#include <ois/ois.h>

// Other input files for my project
#include "Camera.h"
#include "InputHandler.h"


World::World(Ogre::SceneManager *sceneManager, InputHandler *input)   : mSceneManager(sceneManager), mInputHandler(input)
{
	// Global illumination for now.  Adding individual light sources will make you scene look more realistic
	mSceneManager->setAmbientLight(Ogre::ColourValue(1,1,1));

	// First, we create a scene node, and attach a model to it.  coin.mesh is defined in the Content directory,
	//  under models.  Note that you should probably place all of this creation code into a separate method (or methods)
	//  for your final game
	Ogre::Entity *ent1 =SceneManager()->createEntity("coin.mesh");
	pongBall = SceneManager()->getRootSceneNode()->createChildSceneNode("pongBall");
	pongBall->attachObject(ent1);
	

	// Now that we have a scene node, we can move, scale and rotate it as we please.  We'll scale this up a smidge
	// just as an example.  
	//   (take a look at the header file for Ogre::SceneNode -- a good IDE is your friend, here)
	// yaw
	pongBall->scale(2,2,2);
	pongBall->pitch(Ogre::Degree(-90));

	Ogre::Entity *ent2 =SceneManager()->createEntity("pingpp.mesh");
	leftBaffler = SceneManager()->getRootSceneNode()->createChildSceneNode("leftBaffler",Ogre::Vector3(-50,0,0));
	leftBaffler->attachObject(ent2);
	leftBaffler->scale(0.5,5,0);

	Ogre::Entity *ent3 =SceneManager()->createEntity("pingpp.mesh");
	rightBaffler = SceneManager()->getRootSceneNode()->createChildSceneNode("rightBaffler",Ogre::Vector3(50,0,0));
	rightBaffler->attachObject(ent3);
	rightBaffler->scale(0.5,5,0);

	Ogre::Entity *e_obstacle1 =SceneManager()->createEntity("pingpp.mesh");
	obstacle1 = SceneManager()->getRootSceneNode()->createChildSceneNode("obstacle1",Ogre::Vector3(20,25,0));
	obstacle1->attachObject(e_obstacle1);
	obstacle1->scale(0.5,10,0);

	Ogre::Entity *e_obstacle2 =SceneManager()->createEntity("pingpp.mesh");
	obstacle2 = SceneManager()->getRootSceneNode()->createChildSceneNode("obstacle2",Ogre::Vector3(30,-30,0));
	obstacle2->attachObject(e_obstacle2);
	obstacle2->scale(0.5,10,0);

	Ogre::Entity *e_obstacle_hide =SceneManager()->createEntity("pingpp.mesh");
	obstacle_hide = SceneManager()->getRootSceneNode()->createChildSceneNode("obstacle_hide",Ogre::Vector3(0,0,0));
	obstacle_hide->attachObject(e_obstacle_hide);
	obstacle_hide->scale(0.5,20,0);
	obstacle_hide->setVisible(0);

	// Yeah, this should be done automatically for all fonts referenced in an overlay file.
	//  But there is a bug in the OGRE code so we need to do it manually.
	Ogre::ResourceManager::ResourceMapIterator iter = Ogre::FontManager::getSingleton().getResourceIterator();
	while (iter.hasMoreElements()) 
	{ 
		iter.getNext()->load(); 
	}

	// Now we will show the sample overlay.  Look in the file Content/Overlays/Example to
	// see how this overlay is defined
	Ogre::OverlayManager& om = Ogre::OverlayManager::getSingleton();
	Ogre::Overlay *overly = om.getByName("Sample");
	overly->show();

	LEFT_SPEED = 60;
	BALL_SPEED = 0.1;
	//X_SPEED = BALL_SPEED*2;
	//Y_SPEED = BALL_SPEED;
	PLAYER_SCORE = 0;
	AI_SCORE = 0;
	AI_TMP = 0;
	LEVEL = 1;
	LIFE = 5;
	MAKE_IT_ONCE = 1;
}


void 
World::Think(float ftime)
{
	//const float RADIANS_PER_SECOND = 0.1;
	srand(unsigned(time(NULL)));
	float random = 0;
	//const float INIT_X_SPEED = rand()%10;
	//const float INIT_Y_SPEED = rand()%10;
	//pongBall->translate(ftime*INIT_X_SPEED, ftime*INIT_Y_SPEED, 0);

	// This is a pretty silly think method, but it gives you some ideas about how
	//  to proceed.  The single object will rotate

	 //----------pongBall->pitch(Ogre::Radian(time * RADIANS_PER_SECOND));

	//if LIFE = 0
	//end game
	if (LIFE == 0){
		pongBall->setPosition(0,0,0);
		leftBaffler->setPosition(-50,0,0);
		rightBaffler->setPosition(50,0,0);
		 Ogre::OverlayManager& om = Ogre::OverlayManager::getSingleton();
		 Ogre::TextAreaOverlayElement *te = (Ogre::TextAreaOverlayElement *) om.getOverlayElement("Sample/Panel/Text1");
		 //std::stringstream score;
		 //score<<"Score "<<PLAYER_SCORE<<(":")<<AI_SCORE;
		 std::stringstream endgame;
		 endgame<<"Game Over";
		 std::string gameover = endgame.str();
		 te->setCaption(gameover);
	}
	else if (LEVEL>10){
		pongBall->setPosition(0,0,0);
		leftBaffler->setPosition(-50,0,0);
		rightBaffler->setPosition(50,0,0);
		 Ogre::OverlayManager& om = Ogre::OverlayManager::getSingleton();
		 Ogre::TextAreaOverlayElement *te = (Ogre::TextAreaOverlayElement *) om.getOverlayElement("Sample/Panel/Text1");
		 //std::stringstream score;
		 //score<<"Score "<<PLAYER_SCORE<<(":")<<AI_SCORE;
		 std::stringstream wingame;
		 wingame<<"You Win";
		 std::string winwin = wingame.str();
		 te->setCaption(winwin);
	}
	else{
		//LEVEL 1 - 10
		//LEVEL 1 - 3 SPEED
		//LEVEL 4 - 6 OBSTACLE
		//LEVEL 7 RANDOM SPEED
		//LEVEL 8 HIDE OBSTACLE
		//LEVEL 9 SPEEP SPEED SPEED
		//LEVEL 10 UNLIMITED SPEED
		switch(LEVEL) {
		case 1: {
			obstacle1->setVisible(0);
			obstacle2->setVisible(0);
			if(MAKE_IT_ONCE == 1){
				X_SPEED = BALL_SPEED*2;
				Y_SPEED = BALL_SPEED;
				MAKE_IT_ONCE++;
			}
			break;
		}
		case 2: {  
			if(MAKE_IT_ONCE == 2){
				X_SPEED = X_SPEED * 1.3;
				Y_SPEED = Y_SPEED * 1.3;
				MAKE_IT_ONCE++;
			}
			break;
		}
		case 3: {
			if(MAKE_IT_ONCE == 3){
				X_SPEED = X_SPEED * 1.4;
				Y_SPEED = Y_SPEED * 1.4;
				MAKE_IT_ONCE++;
			}
			break;
		}
		case 4: {  
			obstacle1->setVisible(1);
			obstacle2->setVisible(1);
			if(MAKE_IT_ONCE == 4){
				X_SPEED = BALL_SPEED*2;
				Y_SPEED = BALL_SPEED;
				MAKE_IT_ONCE++;
			}
			break;
		}
		case 5: {  
			obstacle1->setVisible(1);
			obstacle2->setVisible(1);
			if(MAKE_IT_ONCE == 5){
				X_SPEED = X_SPEED * 1.2;
				Y_SPEED = Y_SPEED * 1.2;
				MAKE_IT_ONCE++;
			}
			break;
		}
		case 6: { 
			obstacle1->setVisible(1);
			obstacle2->setVisible(1);
			if(MAKE_IT_ONCE == 6){
				X_SPEED = X_SPEED * 1.3;
				Y_SPEED = Y_SPEED * 1.3;
				MAKE_IT_ONCE++;
			}
			break;
		}
		case 7: { 
			obstacle1->setVisible(0);
			obstacle2->setVisible(0);
			if(MAKE_IT_ONCE == 7){
				//initial speed
				X_SPEED = BALL_SPEED*2;
				Y_SPEED = BALL_SPEED;
				MAKE_IT_ONCE++;
			}
			random = rand()%10;
			random = random-5;
			random = random/1000;
			random = BALL_SPEED * random;
			X_SPEED = X_SPEED + random;
			break;
		}
		case 8: { 
			obstacle1->setVisible(0);
			obstacle2->setVisible(0);
			if(MAKE_IT_ONCE == 8){
				X_SPEED = BALL_SPEED*3;
				Y_SPEED = BALL_SPEED*1.5;
				MAKE_IT_ONCE++;
			}
			break;
		}
		case 9: { 
			obstacle1->setVisible(0);
			obstacle2->setVisible(0);
			if(MAKE_IT_ONCE == 9){
				X_SPEED = BALL_SPEED*4.4;
				Y_SPEED = BALL_SPEED*2.2;
				MAKE_IT_ONCE++;
			}
			break;
		}
		case 10: { 
			obstacle1->setVisible(0);
			obstacle2->setVisible(0);
			if(MAKE_IT_ONCE == 10){
				X_SPEED = BALL_SPEED*2;
				Y_SPEED = BALL_SPEED;
				MAKE_IT_ONCE++;
			}
			break;
		}
		default: break;
		}


		//STUPID AI NOW
		if(LEVEL == 10){
			if(AI_TMP >= 10){ //FOR LEVEL 10, WE NEED HIT 10 TIMES.
				rightBaffler->setPosition(rightBaffler->getPosition().x,-pongBall->getPosition().y,0);
			}
			else{
				rightBaffler->setPosition(rightBaffler->getPosition().x,pongBall->getPosition().y,0);
			}
		}
		else{
			if(AI_TMP >= 3){ //AI  IF AI KEEPING HIT 3 TIMES, MAKE A MISTICK AND PLAYER CAN WIN (SIMPLE AI) FOR NOW
				rightBaffler->setPosition(rightBaffler->getPosition().x,-pongBall->getPosition().y,0);
			}
			else{
				rightBaffler->setPosition(rightBaffler->getPosition().x,pongBall->getPosition().y,0);
			}
		}
		//START EMIT THE BALL
		pongBall->translate(X_SPEED, Y_SPEED, 0);
		//CONDITIONS
		if(pongBall->getPosition().x>=48)//right over
		{
			if(pongBall->getPosition().y<=rightBaffler->getPosition().y+5 && pongBall->getPosition().y>=rightBaffler->getPosition().y-5){ // hit
				if(X_SPEED > 0){
					AI_TMP++;
					X_SPEED = -X_SPEED;
					if(LEVEL == 10){
						X_SPEED = X_SPEED * (1.1);
						Y_SPEED = Y_SPEED * (1.1);
					}
				}
				pongBall->translate(X_SPEED, Y_SPEED, 0);
			}
			else{
				LEVEL++;	
				AI_TMP = 0;
				pongBall->setPosition(0,0,0);
				if(X_SPEED > 0){  // if level-up, emit ball toward player
					X_SPEED = -X_SPEED;
				}
				pongBall->translate(X_SPEED, Y_SPEED, 0);
			}
		}
		if(pongBall->getPosition().x<=-48)//LEFT over
		{
			if(pongBall->getPosition().y<=leftBaffler->getPosition().y+5 && pongBall->getPosition().y>=leftBaffler->getPosition().y-5){ // hit
				if(X_SPEED < 0){
					X_SPEED = -X_SPEED;
				}
				pongBall->translate(X_SPEED, Y_SPEED, 0);
			}
			else{ //miss
				//AI_SCORE++;
				//player have 5 lifes
				LIFE--;
				AI_TMP = 0;
				if(LEVEL == 10){
						X_SPEED = BALL_SPEED*2;
						Y_SPEED = BALL_SPEED;
				}
				pongBall->setPosition(0,0,0);
				// go opposite way
				if(X_SPEED < 0){  // if lose life, emit ball toward AI board
					X_SPEED = -X_SPEED;
				}
				pongBall->translate(X_SPEED, Y_SPEED, 0);
			}
		}
		if(pongBall->getPosition().y>=40)//TOP over
		{
			if(Y_SPEED > 0){
				Y_SPEED = -Y_SPEED;
			}
			pongBall->translate(X_SPEED, Y_SPEED, 0);
		}
		if(pongBall->getPosition().y<=-40)//BOTTOM over
		{
			if(Y_SPEED < 0){
				Y_SPEED = -Y_SPEED;
			}
			pongBall->translate(X_SPEED, Y_SPEED, 0);
		}
		//FOR LEVEL 4 - 6
		//SPEED > 0 obstacle active SPEED < 0 obstacle disappear
		if( LEVEL >= 4 && LEVEL <= 6){
			if(X_SPEED > 0){
				obstacle1->setVisible(1);
				obstacle2->setVisible(1);
			}
			else{
				obstacle1->setVisible(0);
				obstacle2->setVisible(0);
			}
			if(pongBall->getPosition().x >= 19 && pongBall->getPosition().x <= 21)//top obstacle1
			{
				if(pongBall->getPosition().y<=35 && pongBall->getPosition().y>=15){ // hit
					if(X_SPEED > 0){
						X_SPEED = -X_SPEED;
					}
					pongBall->translate(X_SPEED, Y_SPEED, 0);
				}
			}
			if(pongBall->getPosition().x >= 29 && pongBall->getPosition().x <= 31)//BOT obstacle2
			{
				if(pongBall->getPosition().y<=-20 && pongBall->getPosition().y>=-40){ // hit
					if(X_SPEED > 0){
						X_SPEED = -X_SPEED;
					}
					pongBall->translate(X_SPEED, Y_SPEED, 0);
				}
			}
		}
		//FOR LEVEL 8
		if( LEVEL == 8){
			if(pongBall->getPosition().x >= -1 && pongBall->getPosition().x <= 1)//hide obstacle
			{
				if(pongBall->getPosition().y<=20 && pongBall->getPosition().y>=-20){ // hit
					if(X_SPEED > 0){
						X_SPEED = -X_SPEED;
					}
					pongBall->translate(X_SPEED, Y_SPEED, 0);
				}
			}
		}
		//LEFT BAFFLER CONTROL
		if (mInputHandler->IsKeyDown(OIS::KC_UP))
		{
			 if(leftBaffler->getPosition().y>=40)
			 {
				 leftBaffler->setPosition(-50,40,0);
			 }
			 else{
				leftBaffler->translate(0, ftime * LEFT_SPEED, 0);
			 }
		 }
		 else if (mInputHandler->IsKeyDown(OIS::KC_DOWN))
		 {
			 if(leftBaffler->getPosition().y<=-40)
			 {
				 leftBaffler->setPosition(-50,-40,0);
			 }
			 else{
				leftBaffler->translate(0, -ftime * LEFT_SPEED, 0);
			 }
		 }
		 //leftBaffler->roll(Ogre::Radian(3.14));

		 Ogre::OverlayManager& om = Ogre::OverlayManager::getSingleton();
		 Ogre::TextAreaOverlayElement *te = (Ogre::TextAreaOverlayElement *) om.getOverlayElement("Sample/Panel/Text1");
		 //std::stringstream score;
		 //score<<"Score "<<PLAYER_SCORE<<(":")<<AI_SCORE;
		 std::stringstream level;
		 level<<"Level "<<LEVEL<<"  LIFE->"<<LIFE;
		 std::string scoreStr = level.str();
		 te->setCaption(scoreStr);
	 }
}


