#include "PhotoStream.h"

PhotoStream * photoStream = NULL;

// ------------------------------------
PhotoStream::PhotoStream() {
}
PhotoStream::~PhotoStream() {
	for(int i=0; i<MAX_PHOTOS; i++) {
		delete images[i];
	}
}

// ------------------------------------ setup
void PhotoStream::setup(string pathToFolder, float seconds) {
	
	
	// allocate for all the photo object
	for(int i=0; i<MAX_PHOTOS; i++) {
		images[i] = new PhotoObject();
	}
	
	
	
	photoLoadCount = 0;
	maxPhotos = 3;
	
	
	photoWidth = 100;
	
	setupFileStream(pathToFolder, seconds);
	
	// events
	ofAddListener(FILE_FOUND, this, &PhotoStream::photoFound);
	ofAddListener(ofEvents.update, this, &PhotoStream::update);
}

// ------------------------------------ remove a photo
void PhotoStream::removePhotoAt(int i) {
	printf("--- workig on this ---\n");
}

// ------------------------------------ get a photo
PhotoObject * PhotoStream::getPhotoAt(int i) {
	return images[i];;
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
	
	
	images[photoLoadCount]->reset();
	images[photoLoadCount]->maxWidth = photoWidth;
	images[photoLoadCount]->loadPhoto(e.name, e.path, e.id);

	
	photoLoadCount ++;
	photoLoadCount %= MAX_PHOTOS;
	
}

// ------------------------------------ threaded function
void PhotoStream::setPhotoMaxWidth(int theMaxWidth) {
	photoWidth = theMaxWidth;
}

// ------------------------------------ update
void PhotoStream::update(ofEventArgs &e) {
	
	for(int i=0; i<MAX_PHOTOS; i++) {
		if(getPhotoAt(i)->pixelsReady() && !getPhotoAt(i)->textureReady()) {
			getPhotoAt(i)->uploadTheTexture();
		}
	}
	
}

// ------------------------------------ draw
void PhotoStream::draw(float x, float y) {
	
	
	for(int i=0; i<MAX_PHOTOS; i++) {	
		
		PhotoObject * obj = getPhotoAt(i);
		
		glPushMatrix();
		glTranslatef(i*obj->getThumbWidth(), 0, 0);
		
		
		ofSetColor(0xffffff);
		obj->thumbnailTexture.draw(0, 0);
		
		
		// draw some info about the photo
		glPushMatrix();
		glTranslatef(0, obj->getThumbHeight(), 0);
		ofSetColor(10, 200, 10);
		ofRect(0, 0, obj->getThumbWidth(), 40);
		string info = ofToString(obj->id);
		ofSetColor(0xffffff);
		ofDrawBitmapString(info, 10, 10);
		glPopMatrix();
		
		
		
		glPopMatrix();
		
	}
	 
}


