#pragma once

#include "ofMain.h"
#include "ThreadImage.h"
#include "ofxPBOTexture.h"

class PhotoObject : public ofImage, public ofxThread {
	
public:
	
	bool			bTextureReady;
	bool			bFileIsLoaded;
	bool			bPixelsLoaded;
	
	float			maxWidth;
	float			ratio;
	int				id;
	string			name;
	string			path;
	
	ofTexture		PBOtex;
	
	// thumbnail
	ofTexture		thumbnailTexture;
	ofImage			thumbnailPixels;
	
	// HD image
	
	// ------------------------------------
	PhotoObject() {
		maxWidth	  = 0;
		ratio		  = 0;
		bTextureReady = false;
		bFileIsLoaded = false;
		bPixelsLoaded = false;
		
		setUseTexture(false);
		thumbnailPixels.setUseTexture(false);
	}
	
	// ------------------------------------	
	~PhotoObject() {
		clear();
		printf("*** Image Destroyed ***\n");
	}
	
	// ------------------------------------
	void loadPhoto(string n, string p, int i) {
		path = p;
		name = n;
		id   = i;
		startThread(true, false);
	}
	
	// ------------------------------------
	void threadedFunction() {
		if(lock()) {
			loadThePixels();
			bPixelsLoaded = true;
			unlock();
		}
	}

	// ------------------------------------
	bool ready() {
		bool b = false;
		if(pixelsReady() && textureReady()) b = true;
		return b;
	}
	
	// ------------------------------------
	bool pixelsReady() {
		return bPixelsLoaded;
	}
	
	// ------------------------------------
	bool textureReady() {
		return bTextureReady;
	}

	// ------------------------------------
	float getThumbWidth() {
		return thumbnailTexture.getWidth();
	}
	float getThumbHeight() {
		return thumbnailTexture.getHeight();
	}
	// ------------------------------------
	void loadThePixels() {
		
		bool loaded = loadImage(path);
		
		//lets resize the pixels
		if(loaded) {
			
			//we need our ratio
			ratio = (float)getHeight() / (float)getWidth();
			
			//now make a small thumbnail
			thumbnailPixels.setFromPixels(myPixels.pixels, getWidth(), getHeight(), type);
			thumbnailPixels.resize(150, 150*ratio);
			
			//lets resize the larger one
			resize(maxWidth, maxWidth*ratio);
			printf("--- Pixels Loaded [%i/%i] ---\n", (int)getWidth(), (int)getHeight());
			
		}
	}
	
	// ------------------------------------
	void uploadTheTexture() {
		if(pixelsReady() && myPixels.bAllocated) {
			
			//printf("-- Texture t:%i r:%f w:%f h:%f --\n", type, ratio, getWidth(), getHeight());
			
			PBOtex.allocate((int)getWidth(), (int)getHeight(), GL_RGB);	
			PBOtex.loadData(myPixels.pixels, getWidth(), getHeight(), GL_RGB);
			
			thumbnailTexture.allocate(thumbnailPixels.getWidth(), thumbnailPixels.getHeight(), GL_RGB);
			thumbnailTexture.loadData(thumbnailPixels.getPixels(), thumbnailPixels.getWidth(), thumbnailPixels.getHeight(), GL_RGB);
			
			bTextureReady = true;
			
		}
	}
	
	
	// ------------------------------------
	void drawThumb(float x, float y) {
		if(textureReady()) {
			glPushMatrix();
			glTranslatef(x, y, 0);
			thumbnailTexture.draw(0, 0);
			glPopMatrix();
		}
	}

	// ------------------------------------
	void draw(float x, float y) {
		if(textureReady()) {
			PBOtex.draw(x, y);
		}
	}
};












