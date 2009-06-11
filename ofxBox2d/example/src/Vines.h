#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxMSASpline.h"

class Vines {
	
	
public:
	
	int		length;
	float	dis;
	float	sz;
	float	maxWidth;
	float	minWidth;
	
	vector      <ofxBox2dJoint>		joints;
	vector		<ofxBox2dCircle>	nodes;
	
	ofxBox2d *						b2d;
	ofxMSASpline2D					spline2D;
	ofPoint							topSpringPos;
	
	// -------------------------------------------	
	Vines() {
		maxWidth = 20.0;
		minWidth = 3.0;
		topSpringPos = 0;
	}
	
	
	// -------------------------------------------
	void setTopNode(float x, float y) {
		nodes[0].addAttractionPoint(x, y, 50.0, 40.0);
	}
	
	// ------------------------------------------- 
	void setMaxWidth(float f) {
		maxWidth = f;
	}
	void setMinWidth(float f) {
		maxWidth = f;
	}
	
	// -------------------------------------------
	void setup(ofxBox2d * b2d, float x, float y, int len) {
		
		if(len < 2) {
			ofLog(OF_LOG_NOTICE, "- length to small in Vines min[3] -");
			length = 3;
		}
		
		length  = len;
		dis		= 10;
		sz		= 1;
		float frc = 4.90;
		
		topSpringPos.x = x;
		topSpringPos.y = 30;
		
		for(int i=0; i<length; i++) {	
			
			float top = y-(length*(maxWidth));
			
			bool isfix = false;
			if(i == length-1) isfix = true;
			
			//size of node
			sz = ofMap(i, 0, length, minWidth, maxWidth);
			
			//make the node for the stem
			ofxBox2dCircle bub;
			//bub.setFixedRotation(true);
			bub.setRotationFriction(100);
			bub.enableGravity(false);
			bub.setPhysics(4.0, 0.0, frc);
			bub.setup(b2d->getWorld(), x, top+(i*sz), sz, isfix);
			nodes.push_back(bub);
			
			
			if(i > 0) {
				ofxBox2dJoint joint;
				joint.setWorld(b2d->world);
				joint.addJoint(nodes[i].body, nodes[i-1].body, 100.0, 100.0);
				joints.push_back(joint);
			}
			else {
				
				b2Body * body, *body2;
				b2BodyDef bodyDef;
				
				bodyDef.position.Set(((ofGetWidth()/2)-200)/OFX_BOX2D_SCALE, (ofGetHeight())/OFX_BOX2D_SCALE);	
				body = b2d->world->CreateBody(&bodyDef);
				
				bodyDef.position.Set(topSpringPos.x/OFX_BOX2D_SCALE, topSpringPos.y/OFX_BOX2D_SCALE);	
				body2 = b2d->world->CreateBody(&bodyDef);
				
				/*ofxBox2dJoint joint;
				 joint.setWorld(b2d->world);
				 joint.addJoint(nodes[i].body, body, .3, 13.0);
				 joints.push_back(joint);
				 */
				
				ofxBox2dJoint joint;
				joint.setWorld(b2d->world);
				joint.addJoint(nodes[i].body, body2, 0.9f, 13.0f);
				joints.push_back(joint);
				
				
			}
		}
		
	}	
	
	// -------------------------------------------
	void update() {
		nodes[0].addAttractionPoint(topSpringPos, 10.0);
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
			
			float r = ofMap(f, 0.0, 1-splineStep, minWidth, maxWidth);
			
			float xOff        = perp.x * r;
			float yOff        = perp.y * r;
			
			glVertex3f( pos1.x - xOff, pos1.y - yOff, 0);
			glVertex3f( pos1.x + xOff, pos1.y + yOff, 0);				
		}
		glEnd();
		ofDisableAlphaBlending();
		
		/*
		for(int i=0; i<nodes.size(); i++) {
			ofEnableAlphaBlending();
			ofPoint np = nodes[i].getPosition();
			ofSetColor(255, 255, 0, 90);
			ofFill();
			ofCircle(np.x, np.y, nodes[i].getRadius());
			ofDisableAlphaBlending();
			nodes[i].draw();
			
		}
		
		for(int i=0; i<joints.size(); i++) {
			joints[i].draw();
		}
		 */
		
	}
	
	
};









