#include "ofxFileStream.h"

// ---------------------------------------
ofxFileStream::ofxFileStream() {
	bSearching  = false;
	bAllocated  = false;
	fileIDCount = 0;
}
ofxFileStream::~ofxFileStream() {
	files.clear();
	reset();
}

// ---------------------------------------
void ofxFileStream::setup(string path, float seconds) {
	
	folderPath  = path;
	timer.setup(seconds);
	
	//events
	ofAddListener(timer.TIMER_REACHED, this, &ofxFileStream::checkForNewFiles);	
	bAllocated = true;
	
}

// ---------------------------------------
void ofxFileStream::setTimer(float seconds) {
	timer.setTimer(seconds);	
}

// ---------------------------------------
void ofxFileStream::checkForNewFiles(ofEventArgs &e) {

	if(!bSearching) {
		bSearching = true;
		int numFiles = listDir(folderPath);
		
		if(numFiles) {
			for(int i=0; i<numFiles; i++) {
				
				bool fileFound = false;
				for(int j=0; j<files.size(); j++) {
					if(getName(i) == files[j]) fileFound = true;
				}
				
				
				//if we found the file in the files
				//vector the do not add it else add it
				if(!fileFound) {
					files.push_back(getName(i));
					static ofxFileStreamEventArgs fileEventArgs;
					fileEventArgs.name = getName(i);
					fileEventArgs.path = getPath(i);
					fileEventArgs.id   = fileIDCount;
					ofNotifyEvent(FILE_FOUND, fileEventArgs, this);
					
					fileIDCount ++;
				}
				
			}
			
			//debug
			debugText = "";
			debugText += "\nFiles Array\n";
			debugText += "------------------------------\n";
			for(int j=0; j<files.size(); j++) {
				debugText += "File: "+files[j]+"\n";
			}
			
		}
		else {
			debugText += "No Files Found\n";
		}
		bSearching = false;
	}
	
}

// ---------------------------------------
void ofxFileStream::setPath(string p) {
	folderPath = p;
	bAllocated = true;
}

// ---------------------------------------
void ofxFileStream::draw(float x, float y) {
	
	ofSetColor(0xffffff);
	ofDrawBitmapString(debugText, 20, 20);
}
