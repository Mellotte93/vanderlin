#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"

#include "BaseParticle.h"
#include "Dust.h"
#include "PixelDust.h"

class DustEngine {
	
public:
	
	vector <Dust>			dust;
	vector <PixelDust>		pixelDust;
	
	// ------------------------------------------------
	DustEngine() {
		printf("--- Particle Engine Created ---");
	}
	
	// ------------------------------------------------
	void addDust(float x, float y) {
		dust.push_back(Dust());
		dust.back().setPos(x, y);
	}
	void addDust(Dust aPartile) {
		dust.push_back(aPartile);
	}
	
	void addPixelDust(PixelDust pd) {
		pixelDust.push_back(pd);
	}
	
	// ------------------------------------------------
	void update() {
		
		//dust
		for(int i=0; i<dust.size(); i++) {
			dust[i].update();
			if(dust[i].dead) dust.erase(dust.begin()+i);
		}
	
		//pixels dust
		for(int i=0; i<pixelDust.size(); i++) {
			pixelDust[i].update();
			if(pixelDust[i].dead) pixelDust.erase(pixelDust.begin()+i);
		}
	}
	
	// ------------------------------------------------
	void draw() {
		
		for(int i=0; i<dust.size(); i++)		dust[i].draw();
		for(int i=0; i<pixelDust.size(); i++)	pixelDust[i].draw();
	}
	
};