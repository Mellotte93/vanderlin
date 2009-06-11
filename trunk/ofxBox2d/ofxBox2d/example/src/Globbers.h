#pragma once
#include "ofxBox2d.h"

class Globbers : public ofxBox2dBaseShape {
	
public:
	
	ofPoint						pos;
	ofxBox2dCircle				centerNode;
	vector <ofxBox2dCircle>		globs;
	vector <ofxBox2dJoint>		joints;
	
	Globbers() {
	}
	
	void setup() {
		
		pos.set(ofGetWidth()/2, ofGetHeight()/2);
		
		//the center node
		centerNode.setPhysics(10.0, 0.2, 0.4);
		centerNode.enableGravity(false);
		centerNode.setup(world, pos.x, pos.y, 10, false);
		
		//ring nodes
		ofPoint p;
		int   ringSize   = 5;
		float angle		 = 0.0f;
		float angleAdder = M_TWO_PI / (float)ringSize;
		for(int i=0; i<ringSize; i++) {
			
			float ringRadius = ofRandom(5, 20);
			p.x = 10+pos.x+cos(angle) * ringRadius;
			p.y = 10+pos.y+sin(angle) * ringRadius;
			angle += angleAdder;
			
			ofxBox2dCircle g;
			g.setPhysics(10.0, 0.2, 0.4);
			g.enableGravity(false);
			g.setup(world, p.x, p.y, ringRadius, false);
			globs.push_back(g);
			
		}
		
		for(int i=0; i<globs.size(); i++) {
			
				ofxBox2dJoint joint;
				joint.setWorld(world);
				joint.addJoint(centerNode.body, globs[i].body, 5.9f, 13.0f);
				joints.push_back(joint);
			
		}
		
	}
	
	void update() {
	}
	
	void draw() {
		
		centerNode.draw();
		
		for(int i=0; i<globs.size(); i++) {
			globs[i].draw();
		}
		for(int i=0; i<joints.size(); i++) {
			joints[i].draw();
		}
	}
	
	
	
};