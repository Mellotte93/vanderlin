#include "ofxTimer.h"

ofxTimer::ofxTimer() {
	
}

ofxTimer::~ofxTimer() {
	printf("*** Timer Destroyed ***\n");
}

// ---------------------------------------

void ofxTimer::setup(float seconds) {
	
	bLoop		= true;
	bPauseTimer = false;
	
	//timer
	bStartTimer = true;
	delay		= seconds;	//seconds
	timer		= 0;
	timerStart	= 0;
	
	//events
	ofAddListener(ofEvents.update, this, &ofxTimer::update);
	
}

// ---------------------------------------
void ofxTimer::loop(bool b) {
	bLoop = b;
}

// ---------------------------------------

void ofxTimer::update(ofEventArgs &e) {
	if(!bPauseTimer) {
		if(bStartTimer) {
			bStartTimer = false;
			timerStart  = ofGetElapsedTimef();
		}
		
		float time = ofGetElapsedTimef() - timerStart;
		if(time >= delay) {
			if(!bLoop) bPauseTimer = true;
			bStartTimer = true;
			ofNotifyEvent(TIMER_REACHED, e, this);
		}
	}
}

// ---------------------------------------

void ofxTimer::setTimer(float seconds) {
	delay = seconds;
}

void ofxTimer::startTimer() {
	bPauseTimer = false;
}

void ofxTimer::stopTimer() {
	bPauseTimer = true;
}