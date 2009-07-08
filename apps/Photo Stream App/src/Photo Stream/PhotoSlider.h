#pragma once
#include "ofMain.h"
#include "PhotoStream.h"
#include "ofxSlider.h"

class PhotoSlider {

private:
	
	ofxSlider *	slider;
	float		contentSliderPos;

public:
	
	//-------------------
	PhotoSlider();
	~PhotoSlider();
	
	//-------------------
	void setup();
	void draw();
	
	//-------------------
	void addListeners();
	void removeListeners();	
	
	void update();
	void mouseMoved(ofMouseEventArgs &e);
	void mousePressed(ofMouseEventArgs &e);	
	void mouseDragged(ofMouseEventArgs &e);	
	void mouseReleased(ofMouseEventArgs &e);
	void keyPressed(ofKeyEventArgs &e);
	void keyReleased(ofKeyEventArgs &e);
};