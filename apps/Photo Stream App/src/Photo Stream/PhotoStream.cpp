#include "PhotoStream.h"

PhotoStream * photoStream = NULL;

// ------------------------------------
PhotoStream::PhotoStream() {
}
PhotoStream::~PhotoStream() {
	for(int i=0; i<images.size(); i++) {
		delete images[i];
	}
}

// ------------------------------------ setup
void PhotoStream::setup(string pathToFolder, float seconds) {
	
	photoLoadCount = 0;
	maxPhotos = 3;
	
	
	photoWidth = 100;
	
	setupFileStream(pathToFolder, seconds);
	
	// events
	ofAddListener(FILE_FOUND, this, &PhotoStream::photoFound);
	ofAddListener(ofEvents.update, this, &PhotoStream::update);
}

// ------------------------------------ photo found
void PhotoStream::removePhotoAt(int i) {
	printf("--- workig on this ---\n");
}

// ------------------------------------ photo found
void PhotoStream::photoFound(ofxFileStreamEventArgs &e) {
	
	printf("Found Photo [%i] name:%s count:%i\n", e.id, e.name.c_str(), photoLoadCount);
	
//	images.push_back(new PhotoObject());
//	images.back()->maxWidth = photoWidth;
//	images.back()->loadPhoto(e.name, e.path, e.id);
	/*
	if(imagesToLoad.size() >= 3) {
		int removeID = 0;
		printf("--- remove the first image [%i] ---\n", removeID);
		
		
		
	}
	
	imagesToLoad.push_back(new PhotoLoader());
	imagesToLoad.back()->name = e.name;
	imagesToLoad.back()->id = e.id;
	
	*/
	photoLoadCount ++;
	photoLoadCount %= maxPhotos;
	
}

// ------------------------------------ threaded function
void PhotoStream::setPhotoMaxWidth(int theMaxWidth) {
	photoWidth = theMaxWidth;
}

// ------------------------------------ update
void PhotoStream::update(ofEventArgs &e) {
	
	for(int i=0; i<images.size(); i++) {
		if(images[i]->pixelsReady() && !images[i]->textureReady()) {
			images[i]->uploadTheTexture();
		}
	}
	
}

// ------------------------------------ draw
void PhotoStream::draw(float x, float y) {
	/*
	for(int i=0; i<imagesToLoad.size(); i++) {
		
		string info = imagesToLoad[i]->name + "id: "+ofToString(imagesToLoad[i]->id) +" "+ofToString(imagesToLoad[i]->bLoaded);
		
		ofSetColor(255, 255, 255);
		ofDrawBitmapString(info, 30, 200 + (i*15));
	}
	*/
	
	
	
	for(int i=0; i<images.size(); i++) {
		
		glPushMatrix();
		glTranslatef(i*images[i]->getWidth(), 0, 0);
		ofSetColor(0xffffff);
		images[i]->draw(0, 0);
		
		ofNoFill();
		ofSetColor(10, 200, 10);
		ofRect(0, 0, images[i]->getWidth(), images[i]->getHeight());
		glPopMatrix();
		
	}
}


