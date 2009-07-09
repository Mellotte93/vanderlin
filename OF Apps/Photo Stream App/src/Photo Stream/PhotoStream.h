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
 
 *******asdasdasd****************************************************************/

#pragma once
#include "ofMain.h"
#include "ofxFileStream.h"
#include "PhotoObject.h"


#define MAX_PHOTOS 5

class PhotoLoader {

public:
	int			id;
	ofImage		img;
	string		name;
	bool		bLoaded;
	PhotoLoader() {
		bLoaded = false;
		name  = "";
		id = 0;
	}
	~PhotoLoader() {
		printf("*** Photo Loader Destroyed ***\n");	
	}
};




class PhotoStream :public ofxFileStream {
	
private:
	
	//vector  <PhotoLoader*>	imagesToLoad;
	//vector  <PhotoObject*>	images;
	int						photoWidth;
	int						photoLoadCount;
	int						maxPhotos;
	PhotoObject *			images[MAX_PHOTOS];
	
	
public:
	
	
	PhotoStream();
	~PhotoStream();
	
	
	// ------------------------------------
	PhotoObject * getPhotoAt(int i);
	
	// ------------------------------------
	void removePhotoAt(int i);
	int	 size() { return (int)MAX_PHOTOS; }
	void setup(string pathToFolder, float seconds);
	void photoFound(ofxFileStreamEventArgs &e);
	void setPhotoMaxWidth(int theMaxWidth);
	void update(ofEventArgs &e);
	void draw(float x, float y);
		
};

extern PhotoStream * photoStream;