#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxMSASpline.h"
#include "BaseCreature.h"
#include "MotionNode.h"

class Grass : public ofxBox2dBaseShape, public BaseCreature {
	
	
public:
	
	// ------------------------------- Debug mode
	bool    bEditMode;
	bool	bDrawVelocity;
	bool	bGrassSelected;
	bool	bDrawRestPoints;
	bool	bDrawGrabPoint;
	
	// ------------------------------- grass params
	int		length;
	float	dis;
	float	sz;
	float	maxWidth;
	float	minWidth;
	
	vector      <ofxBox2dJoint>		joints;
	vector		<ofxBox2dCircle>	nodes;
	
	ofxBox2dJoint					anchorJoint;
	ofxBox2dCircle					anchorNode;
	ofPoint							anchorPos;
	
	ofxMSASpline2D					spline2D;
	
	float		damping;
	float		radius;
	ofPoint		des;
	ofPoint		pos;
	ofPoint		vel;
	
	ofPoint		restPoint;
	float		theta;
	float		midTheta;
	
	bool			bEating;		 //are we eating
	bool			bGrabbing;		 //are we grabbing someting
	bool			bPauseGrabbing;  //is it time to pause the grabbing
	int				grabCount;		 //how long can we grab
	int				offSetX;		 //does the grass bend right or left
	ofPoint			grabPoint;		 //point of grabbing force
	int				eatDelayTime;	 //time between each feast
	int				eatTime;		 //how long do we eat for (force time)
	
	b2Vec2 Impulse;
	b2Vec2 pt;
	
	//motion
	MotionNode		headPulse;
	MotionPulse		headSway;
	
	
	// -------------------------------------------	
	Grass() {
		
		bEditMode		= false;
		bGrassSelected  = false;
		bDrawVelocity	= false;
		bDrawRestPoints = false;
		bDrawGrabPoint  = false;
		
		
		maxWidth		= 4.0;
		minWidth		= 1.0;
		grabCount		= 0;
		bGrabbing		= false;
		bPauseGrabbing  = false;
		bEating			= false;
		eatDelayTime	= (int)ofRandom(20, 50);
		eatTime			= 20;
		
		(int)ofRandom(0, 2) == 0 ? offSetX = ofRandom(10, 40) : offSetX = -ofRandom(10, 40);
		
		theta	 = ofRandom(-1.0, 1.0);
		midTheta = ofRandom(-1.0, 1.0);
		
		//motion (may be temp)
		damping = 0.05;
		radius  = 0;
		pos		= 0;
		des		= 0;
		vel		= 0;
		
		// head pulse
		headPulse.spring  = 0.9;
		headPulse.damping = 0.2;
		
		// head sway
		headSway.speed = ofRandom(0.01, 0.02);
		headSway.theta = ofRandomf();
		headSway.amp   = ofRandom(2, 4);
		
	}
	
	// ------------------------------------------- destroy
	void destroy() {
		
		for(int i=0; i<joints.size(); i++) {
			joints[i].destroyJoint();
		}
		joints.clear();
		
		for(int i=0; i<nodes.size(); i++) {
			nodes[i].destroyShape();
		}	
		nodes.clear();
		
		anchorJoint.destroyJoint();
		anchorNode.destroyShape();
		spline2D.clear();
		
	}
	
	// ------------------------------------------- edit the grass
	void editMode(bool b) {
		bEditMode = b;
	}
	
	// ------------------------------------------- get the top head node
	ofxBox2dCircle& getTopNode() {
		return nodes[0];
	}
	
	// -------------------------------------------
	void setAncorNode(float x, float y, bool updateAnchorDone=false) {
		
		anchorPos.set(x, y);
		
		if(updateAnchorDone) {
			anchorNode.setPosition(x, y);
			anchorJoint.destroyJoint();
			anchorJoint.setWorld(world);
			anchorJoint.addJoint(nodes[0].body, anchorNode.body, 0.9f, 13.0f);
		}
	}
	
	// -------------------------------------------
	void setTopNode(float x, float y) {
		
		ofPoint p = nodes[0].getPosition();
		//float dis = ofDist(p.x, p.y, x, y);
		
		//		des.set(x/OFX_BOX2D_SCALE, y/OFX_BOX2D_SCALE);
		//		vel = (vel*0.5) + (des-pos) * 0.02;
		//		pos += vel;//(des-pos)* 0.03;//vel;
		//		b2Vec2 P(pos.x, pos.y);//(x/OFX_BOX2D_SCALE, y/OFX_BOX2D_SCALE);
		//		b2Vec2 D = P - nodes[0].body->GetPosition(); 
		//		P.Normalize();
		//		b2Vec2 F = 12.0 * D;
		
		
		if(!bGrabbing && !bEating) {
			printf("--- Grabbing [%f,%f] ---\n", x, y);
			bEating   = true;
			bGrabbing = true;
			des.set(x, y);
			grabPoint.set(x, y);
			
			//pulse the head a bit
			headPulse.d = radius*ofRandom(1.04, 1.2);
		}
	}
	
	// ------------------------------------------- 
	void grab(float x, float y) {
		
	}
	
	// ------------------------------------------- 
	void setMaxWidth(float f) {
		maxWidth = f;
	}
	void setMinWidth(float f) {
		maxWidth = f;
	}
	
	// -------------------------------------------
	ofPoint getRootPosition() {
		return 	nodes.back().getPosition();
	}
	
	// -------------------------------------------
	void setup(b2World * w, float x, float y) {
		
		des.set(x, y);
		pos.set(x, y);
		
		world   = w;
		
		length  = 3;
		dis		= ofRandom(12, 80);
		sz		= 1;
		
		float frc			= 1.90;
		float stiff			= 10;
		bool  isCollision	= false;
		
		// the top anchor node. This is where the spring is
		// rooted at. You can move this anchor and it changes 
		// the rest point
		anchorPos.set(x, y+length);
		anchorNode.setup(world, anchorPos.x, anchorPos.y, 10, true);
		
		for(int i=0; i<length; i++) {	
			
			
			//top node (the head)
			if(i == 0) {
				radius		= ofRandom(3, 6);
				sz			= 7;
				isCollision = false;
			}
			else {
				sz = 3;
				isCollision = true;
			}
			
			
			//vary the stiffness at the top
			if(i == 1) stiff = 10;
			else stiff = 3.0;
			
			//lock the bottom node
			bool isfix = false;
			if(i == length-1) isfix = true;
			
			//make the node for the stem
			ofxBox2dCircle bub;
			if(isCollision) bub.disableCollistion();
			
			
			//based on the postion added we 
			//go in a upward or downward location
			float dirX = x;
			float dirY = y;
			if(y <ofGetWidth()/2) {
				dirY = (length-1)*dis + (y-(i*dis));
			}
			else dirY = -((length-1)*dis)+(y+(i*dis));
			
			
			
			
			//bub.disableCollistion();
			bub.setFixedRotation(true);
			bub.enableGravity(false);
			bub.setPhysics(4.0f, 0.0f, frc);
			bub.setup(world, dirX, dirY, sz, isfix);
			bub.setDamping(5.06);
			nodes.push_back(bub);
			
			//rest node
			if(i == 0) {
				restPoint = bub.getPosition();
			}
			
			//link the nodes together
			if(i > 0) {
				ofxBox2dJoint joint;
				joint.setWorld(world);
				joint.addJoint(nodes[i].body, nodes[i-1].body, stiff, 1.0);
				joints.push_back(joint);
			}
			
			//top spring for anchor
			if(i == 0) {
				anchorJoint.setWorld(world);
				anchorJoint.addJoint(nodes[i].body, anchorNode.body, 0.9f, 13.0f);				
			}
		}
		
	}	
	
	// -------------------------------------------
	void update() {
		
		vel = (vel*.7) + (des-pos) * damping;
		pos += vel;
		
		if(bGrabbing) {
			
			//start the grab counter
			grabCount ++;
			
			
			if(grabCount < eatTime) {
				
				//add a force to the grab point
				getTopNode().addAttractionPoint(grabPoint, 100.0);
				damping = 0.2;
								
			}
			else {
				damping     = 0.01;			//slow down when done bitting
				headPulse.d = radius;		//pulse head back down
				des			= restPoint;	//go back to rest point (home)
			}
			
			if(grabCount >= eatTime+eatDelayTime) {
				bPauseGrabbing = false;
				grabCount	   = 0;
				bGrabbing	   = false;
				bEating		   = false;
				eatDelayTime   = (int)ofRandom(20, 50);
			}
		}
		
		else {
			grabCount   = 0;
			des			= restPoint;
			headPulse.d = radius;
		}
		
		
		
		//if we are not eating or grabbing
		//lets just sway in the wind
		pos    = getTopNode().getPosition();
		theta += 0.02;
		pos.x += cos(theta) * ofRandomf();
		pos.y += sin(theta) * ofRandomf();
		if(!bGrabbing) {
			getTopNode().addAttractionPoint(pos, 200.0);			//sway force
			getTopNode().addAttractionPoint(anchorPos, 3.0);		//upward anchor force
		}
		
		
		
		//motion nodes (should use poco)
		headPulse.update();
		headSway.update();
		
		//force the head to new position
		//getTopNode().setPosition(pos);
		
		/*
		 midTheta += .1;
		 ofPoint mid = nodes[1].getPosition();
		 mid.x += -cos(midTheta)*10;
		 mid.y += sin(midTheta)*4;
		 nodes[1].addAttractionPoint(mid, 200.0);*/
	}
	
	// -------------------------------------------
	void draw() {
		
		
		/*
		 //a curve vertex
		 ofNoFill();
		 if(nodes.size() > 0) {
		 ofBeginShape();
		 ofCurveVertex(nodes[0].getPosition().x, nodes[0].getPosition().y);
		 for(int i=0; i<nodes.size(); i+=2) {
		 ofPoint p = nodes[i].getPosition();
		 ofCurveVertex(p.x, p.y);
		 }	
		 ofCurveVertex(nodes.back().getPosition().x, nodes.back().getPosition().y);
		 ofEndShape();
		 }*/
		
		ofPoint headPt     = nodes[0].getPosition();
		ofPoint rootPt     = nodes.back().getPosition();
		ofPoint middle	   = nodes[1].getPosition();// + (headPt - rootPt);
		
		float x0 = rootPt.x;
		float y0 = rootPt.y;
		
		float x1 = middle.x+offSetX;
		float y1 = middle.y;
		
		float x2 = headPt.x;
		float y2 = headPt.y;
		
		float x3 = headPt.x;
		float y3 = headPt.y;
		
		ofEnableAlphaBlending();
		ofEnableSmoothing();
		ofSetColor(255, 255, 255, 190);
		ofNoFill();
		ofBezier(x0, y0, x1, y1, x2, y2, x3, y3);
		ofDisableSmoothing();
		ofDisableAlphaBlending();
		
		
		//the creature head
		ofEnableAlphaBlending();
		ofSetColor(140, 140, 140);
		ofFill();
		ofCircle(headPt.x, headPt.y, headPulse.p/1.3);
		
		ofSetColor(255, 255, 255, 190);
		ofFill();
		ofCircle(headPt.x, headPt.y, radius/3.3);
		
		ofSetColor(255, 255, 255, 190);
		ofNoFill();
		ofCircle(headPt.x, headPt.y, headPulse.p);
		ofDisableAlphaBlending();
		
		//Ther Node makeing up curve
		/*
		 ofNoFill();
		 ofSetColor(155, 20, 55);
		 ofCircle(x0, y0, 2);
		 ofCircle(x1, y1, 2);
		 ofCircle(x2, y2, 2);
		 ofCircle(x3, y3, 2); 
		 */
		
		
		/*
		 //add the nodes into the spline
		 spline2D.clear();
		 //spline2D.setInterpolation(OFX_MSA_SPLINE_LINEAR);
		 
		 for(int i=0; i<nodes.size(); i++) {
		 spline2D.push_back(nodes[i].getPosition());	
		 }
		 
		 ofEnableAlphaBlending();
		 ofSetColor(230, 230, 230, 100);
		 ofNoFill();
		 
		 ofxVec3f pos1, pos2;
		 ofxVec3f perp;
		 ofxVec3f scr;
		 scr.set(0, 0, 1);
		 glBegin(GL_QUAD_STRIP);
		 float splineStep	= 1.0f/35;
		 
		 for(float f = 0; f <= 1 - splineStep; f+= splineStep) {
		 pos1 = spline2D.sampleAt(f);
		 pos2 = spline2D.sampleAt(f + splineStep - 0.0001f);
		 
		 perp.set(pos1 - pos2);
		 perp.perpendicular(scr);
		 
		 float r = ofMap(f, 0.0, 1-splineStep, maxWidth, minWidth);
		 
		 float xOff        = perp.x * r;
		 float yOff        = perp.y * r;
		 
		 glVertex3f( pos1.x - xOff, pos1.y - yOff, 0);
		 glVertex3f( pos1.x + xOff, pos1.y + yOff, 0);				
		 }
		 glEnd();
		 ofDisableAlphaBlending();
		 */
		/*
		 
		 //pulsing head
		 glPushMatrix();
		 glTranslatef(pos.x, pos.y, 0);
		 ofEnableAlphaBlending();
		 ofSetColor(255, 255, 255, 190);
		 ofNoFill();
		 ofCircle(0, 0, 5);
		 ofDisableAlphaBlending();
		 glPopMatrix();
		 */
		
		//are we eating
		if(bEating) {
			ofFill();
			ofSetColor(255, 0, 0);
			ofCircle(0, 0, radius);
		}
		
		//grabbing force
		if(bGrabbing && bDrawGrabPoint) {
			ofSetColor(255, 255, 0);
			ofFill();
			ofCircle(grabPoint.x, grabPoint.y, 2);
			ofSetColor(255, 255, 0);
			ofNoFill();
			ofCircle(grabPoint.x, grabPoint.y, 5);
		}
		
		//draw the velocity
		if(bDrawVelocity) {
			ofEnableAlphaBlending();
			ofxVec2f v	= getTopNode().getVelocity();
			float mag	= v.length()*10.0;
			float a		= (float)(-1*(atan2(-v.y, v.x)));
			
			ofSetColor(255, 255, 0, 150);
			glPushMatrix();
			glTranslatef(headPt.x, headPt.y, 0);
			glRotatef(ofRadToDeg(a), 0, 0, 1);
			ofNoFill();
			ofLine(0, 0, mag, 0);
			ofFill();
			ofBeginShape();
			ofVertex(mag, 0);
			ofVertex(mag-10, +10/2);
			ofVertex(mag-10, -10/2);
			ofVertex(mag, 0);
			ofEndShape(true);
			glPopMatrix();
			ofDisableAlphaBlending();
		}
		
		//the resting point
		if(bDrawRestPoints) {
			ofSetColor(0, 255, 255, 250);
			ofFill();
			ofCircle(restPoint.x, restPoint.y, 3);
			ofNoFill();
			ofCircle(restPoint.x, restPoint.y, 5);
		}
		
		
		//the anchor node
		if(bEditMode) {
		
			//the box2d nodes
			for(int i=0; i<nodes.size(); i++) {
				ofEnableAlphaBlending();
				ofPoint np = nodes[i].getPosition();
				ofSetColor(255, 34, 255, 90);
				ofFill();
				ofCircle(np.x, np.y, nodes[i].getRadius()/1.3);
				
				ofSetColor(255, 34, 255, 190);
				ofFill();
				ofCircle(np.x, np.y, nodes[i].getRadius()/3.3);
				ofDisableAlphaBlending();
			}
			
			//the joints
			for(int i=0; i<joints.size(); i++) {
				joints[i].draw(80);
			}
			
			ofEnableAlphaBlending();
			
			//anchorNode.draw();
			//anchorJoint.draw();
			
			//highlight the head
			if(bGrassSelected) {
				ofSetColor(0, 255, 255, 255);
				ofFill();
				ofCircle(headPt.x, headPt.y, 10);
				ofNoFill();
				ofCircle(headPt.x, headPt.y, 13);
			}
			
			//highlight the 
			bGrassSelected ? ofSetColor(255, 0, 255, 250) : ofSetColor(255, 255, 255, 150);
			
			ofFill();
			ofCircle(anchorPos.x, anchorPos.y, 3);
			
			ofNoFill();
			ofCircle(anchorPos.x, anchorPos.y, 5);
			
			ofLine(anchorPos.x, anchorPos.y, getTopNode().getPosition().x, getTopNode().getPosition().y);
			ofDisableAlphaBlending();
			
			
		}
	}	
};













