#pragma once

#include "ofxBox2d.h"

class Bubble : public ofxBox2dCircle {
	
	
	
public:
	
	float alpha;
	float i;
	
	int age;
	int deadDate;
	bool doFade;
	int r;
	Bubble() {
		alpha = 250;
		i = 3;
		age = 0;
		deadDate = (int)ofRandom(250, 400);
		doFade = false;
		r = ofRandom(5, 10);
	}
	
	void update() {
		
		if(!doFade && i < r) {
			i += 1.4;
			setRadius(i);
		}
		
		if(doFade) {
			alpha -= 2.5;
			if(alpha <= 1 && !dead) {
				destroyShape();	
			}
		}
		
		if(age >= deadDate) {
			doFade = true;
		}
		age ++;
	}
	
	void draw() {
		
		if(dead) return;
		
		ofPoint pos = getPosition();
		float	r   = getRadius() / 1.3f;
		
		ofEnableAlphaBlending();
		glPushMatrix();
		glTranslatef(pos.x, pos.y, 0);
		ofSetColor(100, 100, 100, alpha);
		ofFill();
		ofCircle(0, 0, r);
		ofSetColor(200, 200, 200, alpha);
		ofNoFill();
		ofCircle(0, 0, r);
		glPopMatrix();
		ofDisableAlphaBlending();
	}
	
	
};