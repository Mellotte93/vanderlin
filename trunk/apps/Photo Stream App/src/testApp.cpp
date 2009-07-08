#include "testApp.h"

//-------------------------------------------------------------- setup
void testApp::setup() {
	
	ofSetWindowTitle("Photo Stream");
    ofBackground(30, 30, 30);
	ofSetVerticalSync(true);
	ofSetWindowPosition(1500, 200);
	
	photoStream = new PhotoStream();
	photoStream->allowExt("jpg");
	photoStream->setup("/Users/todd/Desktop/CameraFeed", 0.3);
	
	photoSlider = new PhotoSlider();
	photoSlider->setup();
	
	
	
	pos.x = ofGetWidth()/2;
	pos.y = ofGetHeight()/2;
}

//-------------------------------------------------------------- exit
void testApp::exit() {
	
	delete photoStream;
	delete photoSlider;
	
	photoSlider = NULL;
	photoStream = NULL;
	
	printf("*** Destroyed App ***\n");
}


//-------------------------------------------------------------- update
void testApp::update() {
	photoSlider->update();
}

//-------------------------------------------------------------- draw
void testApp::draw() {
	
	photoSlider->draw();
	
	pos.x += cos(ofGetElapsedTimef()*8) * 4;
	pos.y += sin(ofGetElapsedTimef()*8) * 3;

	ofSetColor(90, 90, 90);
	ofFill();
	ofCircle(pos.x, pos.y, 10);
	
	// draw a grid of the photos
	glPushMatrix();
	glTranslatef(0, 200, 0);
	ofPoint p;
	for(int i=0; i<photoStream->size(); i++) {
		
		ofSetColor(0xffffff);
		PhotoObject * photo = photoStream->getPhotoAt(i);
		photo->drawThumb(p.x, p.y);
		
		p.x += photo->getThumbWidth();
		if(i % 10 == 9) {
			p.x = 0;
			p.y += photo->getThumbHeight();	
		}
		
	}
	glPopMatrix();
	
	photoStream->draw(0, 0);
	
	// what is our framerate
	ofDrawBitmapString(ofToString(ofGetFrameRate()), 30, ofGetHeight()-30);
	
	
}
//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
	
	if(key == 'f') ofToggleFullscreen();
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}


