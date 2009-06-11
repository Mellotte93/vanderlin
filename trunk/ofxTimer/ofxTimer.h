#pragma once
#include "ofMain.h"

class ofxTimer {
	
private:
	
	//timer
	bool		bLoop;
	bool		bPauseTimer;
	bool		bStartTimer;
	float		delay;
	float		timer;
	float		timerStart;

	void update(ofEventArgs &e);
	
public:
	
	ofEvent <ofEventArgs> TIMER_REACHED;
	
	ofxTimer();
	~ofxTimer();
	
	// ---------------------------------------
	void setup(float seconds, bool loopTimer);
	//void draw();
	
	// ---------------------------------------
	void setTimer(float seconds);
	void startTimer();
	void stopTimer();
	void loop(bool b);
};