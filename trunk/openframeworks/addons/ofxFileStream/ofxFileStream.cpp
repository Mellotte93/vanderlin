/***********************************************************************
 
 Copyright (c) 2009, Todd Vanderlin, www.vanderlin.cc
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 ***********************************************************************/

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
