/***********************************************************************
 
 Copyright (c) 2009, Todd Vanderlin, www.vanderlin.cc
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 ***********************************************************************/

#pragma once

#include "ofMain.h"
#include "ThreadImage.h"
#include "ofxPBOTexture.h"

class PhotoObject : public ofxThread {
	
public:
	
	bool			bAlive;
	bool			bTextureReady;
	bool			bFileIsLoaded;
	bool			bPixelsLoaded;
	
	
	float			maxWidth;
	float			ratio;
	int				id;
	string			name;
	string			path;
	
	
	// thumbnail
	ofxPBOTexture		thumbnailTexture;
	ofImage			thumbnailPixels;
	
	// HD image
	ofxPBOTexture		hdTexture;
	ofImage			hdPixels;
	
	// ------------------------------------
	PhotoObject() {
		
		// info
		name = "";
		path = "";
		id   = -1;
		
		
		// sizing
		maxWidth	  = 0;
		ratio		  = 0;
		
		// flags
		bTextureReady = false;
		bFileIsLoaded = false;
		bPixelsLoaded = false;
		bAlive		  = false;
		
		// thumbnail texture		
		thumbnailPixels.setUseTexture(false);
		
		// hd image
		hdPixels.setUseTexture(false);
	}
	
	// ------------------------------------	
	~PhotoObject() {
				
		// hd
		hdPixels.clear();
		hdTexture.clear();
		
		// thumb
		thumbnailPixels.clear();
		thumbnailTexture.clear();
		
		printf("*** Image Destroyed ***\n");
	}

	// ------------------------------------ reset the photo obj to load a new one
	void reset() {
		
	
		
		// we are dead
		bAlive = false;
		
		// reset all the booleans
		bTextureReady = false;
		bFileIsLoaded = false;
		bPixelsLoaded = false;
		
		// clear the images
		hdPixels.clear();
		hdTexture.clear();
		thumbnailPixels.clear();
		thumbnailTexture.clear();
		
		
		// we do not want image textures
		thumbnailPixels.setUseTexture(false);
		hdPixels.setUseTexture(false);
		
		
		// stop the thread
		if(isThreadRunning()) {
			stopThread();			
		}
		
		
		
	}
	
	// ------------------------------------
	void loadPhoto(string n, string p, int i) {
		
		path = p;
		name = n;
		id   = i;
		
		startThread(true, false);
		
		bAlive = true;
	}
	
	// ------------------------------------
	void threadedFunction() {
		
		// we are so fast we need to slow down
		ofSleepMillis(100); 
	
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

	// ------------------------------------ thumbnail texture
	float getThumbWidth() {
		return thumbnailTexture.getWidth();
	}
	float getThumbHeight() {
		return thumbnailTexture.getHeight();
	}
	ofTexture& getThumbnail() {
		if(!thumbnailTexture.bAllocated() ){		
			return thumbnailTexture;
		}
	}
	
	// ------------------------------------ HD texture
	float getHDWidth() {
		return thumbnailTexture.getWidth();
	}
	float getHDHeight() {
		return thumbnailTexture.getHeight();
	}
	ofTexture& getHDTexture() {
		if(!hdTexture.bAllocated() ){		
			return hdTexture;
		}
	}
	
	// ------------------------------------ load the pixels
	void loadThePixels() {
		
		if(!bAlive) return; // nope we are dead
		
		
		
		
		bool loaded = hdPixels.loadImage(path);
		
		
		
		// load the pixels
		if(loaded) {
			
			// we need the ratio
			ratio = (float)hdPixels.getHeight() / (float)hdPixels.getWidth();
			
			// now make a small thumbnail
			thumbnailPixels.setFromPixels(hdPixels.getPixels(), hdPixels.getWidth(), hdPixels.getHeight(), hdPixels.type);
			thumbnailPixels.resize(150, 150*ratio);
			
			// lets resize the larger one
			hdPixels.resize(maxWidth, maxWidth*ratio);
			
			
			//printf("--- Ratio: %f ---\n", ratio);
			printf("--- Pixels Loaded [%i/%i] ---\n", (int)hdPixels.getWidth(), (int)hdPixels.getHeight());
			
			
			// ok we are done kick it off
			bAlive = true;
			if(isThreadRunning()) {
				stopThread();			
			}
			
		}
		else {
			printf("--- error loading photo ---\n");	
		}
	}
	
	// ------------------------------------ upload the texture
	void uploadTheTexture() {
		
		if(!bAlive) return;
		
		if(pixelsReady()) {
			
			//printf("--- ready to upload the texture ---\n");
			//printf("-- Texture t:%i r:%f w:%f h:%f --\n", type, ratio, getWidth(), getHeight());
			
			// lets make a thumbnail texture
			thumbnailTexture.allocate((int)thumbnailPixels.getWidth(), (int)thumbnailPixels.getHeight(), GL_RGB);
			thumbnailTexture.loadData(thumbnailPixels.getPixels(), (int)thumbnailPixels.getWidth(), (int)thumbnailPixels.getHeight(), GL_RGB);
			
			// lets make the HD texture
			hdTexture.allocate((int)hdPixels.getWidth(), (int)hdPixels.getHeight(), GL_RGB);
			hdTexture.loadData(hdPixels.getPixels(), (int)hdPixels.getWidth(), (int)hdPixels.getHeight(), GL_RGB);
			
			
			bTextureReady = true;
			
			printf("--- the texture is loaded ---\n");
		}
	}
	
	
	// ------------------------------------ draw a thumbnail
	void drawThumb(float x, float y) {
		
		if(!bAlive) return;
		
		if(textureReady()) {
			glPushMatrix();
			glTranslatef(x, y, 0);
			thumbnailTexture.draw(0, 0);
			glPopMatrix();
		}
	}

	// ------------------------------------ draw the HD image
	void drawHD(float x, float y) {
		
		if(!bAlive) return;
		
		if(textureReady()) {
			hdTexture.draw(x, y);
		}
	}
};












