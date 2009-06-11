#pragma once
#include "ofMain.h"

class BaseParticle {
	
public:
	
	bool	 dead;
	ofPoint  pos, vel, acc;
	ofColor  color;
	
	// ------------------------------------------------
	BaseParticle() {
		dead	= false;
		color.r = 255;
		color.g = 255;
		color.b = 255;
		color.a = 200;
	}
	
	// ------------------------------------------------
	virtual void fadeDeath(float amt) {
		color.a -= amt;
		if(color.a <= 0) dead = true;
	}
	
	// ------------------------------------------------
	virtual void setColor(float r, float g, float b) {
		color.r = r;
		color.g = g;
		color.b = b;
	}
	virtual void setColor(ofColor c) {
		color.r = c.r;
		color.g = c.g;
		color.b = c.b;
	}	
	
	// ------------------------------------------------
	virtual void setPos(float x, float y) {
		pos.set(x, y);	
	}
	virtual void setPos(ofPoint p) {
		pos = p;
	}
	
	// ------------------------------------------------
	virtual void update()	{ }
	virtual void draw()		{ }
	
};