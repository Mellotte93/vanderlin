#pragma once
#include "ofMain.h"
#include "BaseParticle.h"

class PixelDust : public BaseParticle {
	
public:
	
	ofImage	pixels;
	int			width, height;
	
	PixelDust() {
		width  = 50;
		height = 50;
		
		pixels.allocate(width, height, OF_IMAGE_COLOR);
	}
	
	void setPixels(unsigned char * px) {
		pixels.setFromPixels(px, width, height, OF_IMAGE_COLOR);
	}
	
	void update() {
		fadeDeath(3.4);
	}
	
	void draw() {
		ofEnableAlphaBlending();
		glPushMatrix();
		glTranslatef(pos.x, pos.y, 0);
		ofSetColor(color.r, color.g, color.b, color.a);
		pixels.draw(0, 0);
		glPopMatrix();
		ofDisableAlphaBlending();
	}
};