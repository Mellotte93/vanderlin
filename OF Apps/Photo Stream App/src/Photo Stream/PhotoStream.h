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