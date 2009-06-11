
#pragma once

#define MOTION_NODE_DES 0
#define MOTION_NODE_ACC 1
#include "ofMain.h"

//------------------------------------------------------

class MotionPulse {
public:
	
	float p, theta, amp, speed;
	
	MotionPulse() {
		p	  = 0;
		theta = 0.2;
		amp   = 10.0;
		speed = 0.2;
	}
	
	void update() {
		theta += speed;
		p = cos(theta) * amp;
		
		//if(theta >= 1.0) theta = 0.0;
	}
	
};

//------------------------------------------------------

class MotionNode {
	
public: 
	
	float p, v, a, d;
	float   spring, damping;
	int		mode;
	
	MotionNode() {
		p = 0; v = 0; a = 0; d = 0;
		spring  = 0.7f;
		damping = 0.03f;
		mode    = MOTION_NODE_DES;
	}
	
	virtual void update() {
		switch (mode) {
			case MOTION_NODE_DES:
				v = (v*spring) + (d-p) * damping;
				p += v;
				break;
			case MOTION_NODE_ACC:
				v += a;
				p += v;
				break;
		}
	}
	
	virtual void setMotionType(int moType) {
		mode = moType;
	}
	
	
};

//------------------------------------------------------

class MotionNode2d {
	
public: 
	
	ofPoint p, v, a, d;
	float   spring, damping;
	int		mode;
	
	MotionNode2d() {
		p = 0; v = 0; a = 0; d = 0;
		spring  = 0.7f;
		damping = 0.03f;
		mode    = MOTION_NODE_DES;
	}
	
	virtual void update() {
		switch (mode) {
			case MOTION_NODE_DES:
				v = (v*spring) + (d-p) * damping;
				p += v;
				break;
			case MOTION_NODE_ACC:
				v += a;
				p += v;
				break;
		}
	}
	
	virtual void setMotionType(int moType) {
		mode = moType;
	}
	
	
};