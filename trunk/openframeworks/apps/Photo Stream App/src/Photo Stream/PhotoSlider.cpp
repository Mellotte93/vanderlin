#include "PhotoSlider.h"

// --------------------------------------------------

PhotoSlider::PhotoSlider() {
	contentSliderPos = 0;
}
PhotoSlider::~PhotoSlider() {
	delete slider;
}

// --------------------------------------------------

void PhotoSlider::setup() {
	
	ofAddListener(ofEvents.mouseMoved, this, &PhotoSlider::mouseMoved);
	
	slider = new ofxSlider();
	slider->setup();
	slider->enableFullWidth();
	slider->setPos(0, 149);
	
	
	
}

// --------------------------------------------------
void PhotoSlider::draw() {
	
	//the slider bar
	slider->draw();
	
	//draw the photo stream
	glPushMatrix();
	glTranslatef(-contentSliderPos, 0, 0);
	for(int i=0; i<photoStream->size(); i++) {
		
		PhotoObject * obj = photoStream->getPhotoAt((photoStream->size()-1)-i);
		
		float w = 150;//obj->smallTexture.getWidth();
		float h = obj->thumbnailTexture.getHeight();
		
		glPushMatrix();
		glTranslatef((i*w), 0, 0);
		if(obj->ready()) {
			ofSetColor(0xffffff);
			obj->drawThumb(0, 0);
		}
		
		ofFill();
		i % 2 == 0 ? ofSetColor(50, 50, 50) : ofSetColor(30, 30, 30);
		ofRect(0, h, w, 50);
		ofSetColor(255, 255, 255);
		
		//draw my id
		ofSetColor(255, 255, 255);
		ofDrawBitmapString(ofToString(obj->id)+"\n"+obj->name, 20, h+20);
		glPopMatrix();
		
	}
	glPopMatrix();
	
	
}

// --------------------------------------------------
void PhotoSlider::update() {
	
	int maxa = floor(ofGetWidth()/150);
	float contw = ((photoStream->size()*150) - (maxa*150));
	contentSliderPos = slider->getSliderValue()*contw; 
}

// --------------------------------------------------
void PhotoSlider::addListeners() {
	//ofAddListener(ofEvents.update, this, &PhotoSlider::update);
	ofAddListener(ofEvents.mousePressed, this, &PhotoSlider::mousePressed);
	ofAddListener(ofEvents.mouseDragged, this, &PhotoSlider::mouseDragged);
	ofAddListener(ofEvents.mouseReleased, this, &PhotoSlider::mouseReleased);
	ofAddListener(ofEvents.keyPressed, this, &PhotoSlider::keyPressed);
	ofAddListener(ofEvents.keyReleased, this, &PhotoSlider::keyReleased);
}
void PhotoSlider::removeListeners() {
	//ofRemoveListener(ofEvents.update, this, &PhotoSlider::update);
	ofRemoveListener(ofEvents.mousePressed, this, &PhotoSlider::mousePressed);
	ofRemoveListener(ofEvents.mouseMoved, this, &PhotoSlider::mouseMoved);
	ofRemoveListener(ofEvents.mouseDragged, this, &PhotoSlider::mouseDragged);
	ofRemoveListener(ofEvents.mouseReleased, this, &PhotoSlider::mouseReleased);
	ofRemoveListener(ofEvents.keyPressed, this, &PhotoSlider::keyPressed);
	ofRemoveListener(ofEvents.keyReleased, this, &PhotoSlider::keyReleased);
}

// --------------------------------------------------
void PhotoSlider::mouseMoved(ofMouseEventArgs &e) {
	
	for(int i=0; i<photoStream->size(); i++) {
		
		PhotoObject * obj = photoStream->getPhotoAt((photoStream->size()-1)-i);
		if(obj->ready()) {
			float x = -contentSliderPos + (i*150);
			float y = 0;
			float w = obj->thumbnailTexture.getWidth();
			float h = obj->thumbnailTexture.getHeight();
			
			if(e.x > x && e.x < x+w && e.y > y && e.y < y+h) {
				printf("--- name:%i ---\n", (photoStream->size()-1) - i);
			}
		}
	}
	
}

// --------------------------------------------------
void PhotoSlider::mousePressed(ofMouseEventArgs &e) {
	
}

// --------------------------------------------------
void PhotoSlider::mouseDragged(ofMouseEventArgs &e) {
	
}

// --------------------------------------------------
void PhotoSlider::mouseReleased(ofMouseEventArgs &e) {
}

// --------------------------------------------------
void PhotoSlider::keyPressed(ofKeyEventArgs &e) {
	
}
void PhotoSlider::keyReleased(ofKeyEventArgs &e) {
	
}
