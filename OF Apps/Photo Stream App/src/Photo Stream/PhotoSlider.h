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