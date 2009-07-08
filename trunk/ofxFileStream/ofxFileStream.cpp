#include "ofxFileStream.h"

// ---------------------------------------
ofxFileStream::ofxFileStream() {
	bSearching  = false;
	bAllocated  = false;
	bVerbose    = false;
	
	fileIDCount = 0;
	numFiles    = 0;
}
ofxFileStream::~ofxFileStream() {
	files.clear();
	reset();
}

// ---------------------------------------
void ofxFileStream::setupFileStream(string path, float seconds) {
	
	folderPath  = path;
	timer.setup(seconds, true);
	
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
		
		int numFilesFound = listDir(folderPath);
		
		if(numFilesFound == -1) {
			printf("--- We lost the path connection ---\n");	
			printf("--- path:%s\n ---\n", folderPath.c_str());
			reset();
			return;
		}
		if(numFilesFound) {
			
			for(int i=0; i<numFilesFound; i++) {
				
				bool fileFound = false;
				for(int j=0; j<files.size(); j++) {
					if(getName(i) == files[j]) fileFound = true;
				}
				
				
				// if we found the file in the files
				// vector the do not add it else add it
				if(!fileFound) {
					files.push_back(getName(i));
					static ofxFileStreamEventArgs fileEventArgs;
					fileEventArgs.name = getName(i);
					fileEventArgs.path = getPath(i);
					fileEventArgs.id   = fileIDCount;
					fileEventArgs.totalPhotos = numFiles;
					ofNotifyEvent(FILE_FOUND, fileEventArgs, this);
					
					fileIDCount ++;
					numFiles ++;
				}
			}
			
			//debug
			if(bVerbose) {
				debugText = "";
				debugText += "\nFiles Array\n";
				debugText += "------------------------------\n";
				for(int j=0; j<files.size(); j++) {
					debugText += "File: "+files[j]+"\n";
				}
			}
			
		}
		else {
			if(bVerbose) debugText += "No Files Found\n";
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
