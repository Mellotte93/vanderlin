#pragma once
#include "ofMain.h"
#include "ofxDirList.h"
#include "ofxTimer.h"
#include "ofxFileStreamEvent.h"

class ofxFileStream : public ofxDirList {

private:
	
	string				debugText;
	string				folderPath;
	vector <string>		files;
	int					fileIDCount;
	ofxTimer			timer;
	bool				bSearching;
	
public:
	
	bool							bAllocated;
	ofEvent <ofxFileStreamEventArgs>	FILE_FOUND;
	
	// ---------------------------------------
	ofxFileStream();
	~ofxFileStream();
	
	// ---------------------------------------
	void setup(string path, float seconds);
	void setTimer(float seconds);
	void draw(float x, float y);

	// ---------------------------------------
	void setPath(string p);
	void checkForNewFiles(ofEventArgs &e);
};