#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "BaseParticle.h"

class Dust : public BaseParticle {
	
public:
	
	float    radius;
	
	float	 age;
	float	 deathDate;
	
	// ------------------------------------------------
	Dust() {
		pos = 0;
		vel = 0;
		acc.set(ofRandom(-.5, .5), ofRandom(-.5, .5));
		
		radius  = ofRandom(0.4, 2.0);
		
		age		  = 0.0;
		deathDate = ofRandom(10.0, 30.0); 
	}

	// ------------------------------------------------	
	void update() {
		
		color.a -= 4.0;
		if(color.a <= 0) dead = true;
		
		vel += acc;
		pos += vel;
		
		acc *= 0.3;
	}
	
	// ------------------------------------------------
	void draw() {
		ofEnableAlphaBlending();
		ofFill();
		ofSetColor(color.r, color.g, color.b, color.a);
		ofCircle(pos.x, pos.y, radius);
		ofDisableAlphaBlending();
	}
};