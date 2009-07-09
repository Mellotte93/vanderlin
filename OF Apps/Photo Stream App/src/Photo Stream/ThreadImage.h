#pragma once

#include "ofMain.h"
#include "ofxThread.h"
#include "ofxPBOTexture.h"

class ThreadImage : public ofxThread, public ofImage {
	
public:
	
	ThreadImage() {
		isReady         = false;
		isReadyToUse    = false;
		isUpdated       = false;
		
		ofxThread::verbose = false;
	}
	
	//------------------------------------
	void allocate(int w, int h, int type){
		
		int newBpp = 0;
		
		switch (type){
			case OF_IMAGE_GRAYSCALE:
				newBpp = 8;
				break;
			case OF_IMAGE_COLOR:
				newBpp = 24;
				break;
			case OF_IMAGE_COLOR_ALPHA:
				newBpp = 32;
				break;
			default:
				ofLog(OF_LOG_ERROR,"error = bad imageType in ofImage::allocate");
				return;
		}
		
		allocatePixels(myPixels, w, h, newBpp);
		
		// take care of texture allocation --
		if ( myPixels.bAllocated == true && bUseTexture == true && !pboTex.bAllocated() ){
			pboTex.allocate(myPixels.width, myPixels.height, myPixels.glDataType);
		}
		
		update();
	}
	
	//-------------------------
	void loadImageThreaded(string filename){
		fileToLoad      = filename;
		isReady         = false;
		isReadyToUse    = false;
		isUpdated       = false;
		
		startThread(true, false);
	}
	
	//-------------------------
	bool updatePixelsAndTexture(){
		if( isImageReady() ) {
			
			if(isUpdated == false){
				
				if (myPixels.bAllocated == true && bUseTexture == true && !pboTex.bAllocated() ){
					
					pboTex.allocate(myPixels.width, myPixels.height, myPixels.glDataType);
				}
				update();
				
				isUpdated = true;
			}
		}
		
		return isUpdated;
	}
	
	//------------------------------------
	void update(){
		
		if (myPixels.bAllocated == true && bUseTexture == true){
			pboTex.loadData(myPixels.pixels, myPixels.width, myPixels.height, myPixels.glDataType);
		}
		
		width	= myPixels.width;
		height	= myPixels.height;
		bpp		= myPixels.bitsPerPixel;
		type	= myPixels.ofImageType;
	}
	
	
	//------------------------------------
	void draw(float _x, float _y, float _w, float _h){
		if (bUseTexture){
			pboTex.draw(_x, _y, _w, _h);
		}
	}
	
	//------------------------------------
	void draw(float x, float y){
		draw(x,y,myPixels.width,myPixels.height);
	}
	
	//--------------------------
	bool isImageUpdated(){
		return isUpdated;
	}
	
	//--------------------------
	bool isImageReady(){
		if( isReady == false){
			if( lock() ){
				isReady = isReadyToUse;
				unlock();
			}
		}
		return isReady;
	}
	
	//--------------------------
	void threadedFunction() {
		
		if(lock()) {
			isReadyToUse = loadImageIntoPixels(fileToLoad, myPixels);
			if(isReadyToUse) {
				printf("--- F:%s Done ---\n", fileToLoad.c_str());	
				//resize(90, 90);
				updatePixelsAndTexture();
			}
 			unlock();
		}
	}
	
	string fileToLoad;
	bool isReady;
	bool isUpdated;
	
	ofxPBOTexture pboTex;
	
private:
	bool isReadyToUse;
};

