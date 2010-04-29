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
	if(bVerbose) printf("*** File Stream Destroyed **\n");

}

// ---------------------------------------
void ofxFileStream::setupFileStream(string path, float milliseconds) {
	
	folderPath  = path;
	timer.setup(milliseconds, true);
	
	// events
	ofAddListener(timer.TIMER_REACHED, this, &ofxFileStream::checkForNewFiles);	
	bAllocated = true;
	
	if(bVerbose) printf("--- File Stream Created ---\n");
}

// ---------------------------------------
void ofxFileStream::allowImageExtensions() {
	allowExt("jpg");
	allowExt("jpeg");
	allowExt("gif");
	allowExt("png");
}

// ---------------------------------------
void ofxFileStream::setTimer(float milliseconds) {
	timer.setTimer(milliseconds);	
}

// ---------------------------------------
void ofxFileStream::checkForNewFiles(ofEventArgs &e) {
	
	if(bVerbose) printf("--- looking for new files ---\n");
	
	
	if(!bSearching) {
		
		bSearching			= true;
		int numFilesFound	= listDir(folderPath);
		
		if(numFilesFound == -1) {
			printf("--- We lost the path connection ---\n");	
			printf("--- path:%s\n ---\n", folderPath.c_str());
			reset();
			return;
		}

		if(numFilesFound) {
						
			for(int i=0; i<numFilesFound; i++) {
				
				// look to see if the file is already 
				// added to the vector of file
				bool fileFound = false;
				for(int j=0; j<files.size(); j++) {
					if(getName(i) == files[j].name) fileFound = true;
				}
								
				// if we found the file in the files
				// vector the do not add it else add it
				if(!fileFound) {
					FileStreamFile f;
					f.name = getName(i);
					f.path = getPath(i);
					files.push_back(f);
					
					static ofxFileStreamEventArgs fileEventArgs;
					fileEventArgs.name = getName(i);
					fileEventArgs.path = getPath(i);
					fileEventArgs.id   = fileIDCount;
					fileEventArgs.totalPhotos = numFiles;
					ofNotifyEvent(FILE_FOUND, fileEventArgs, this);
					
					fileIDCount ++;
					numFiles	++;
				}
			}
			
			//debug
			if(bVerbose) {
				debugText = "";
				debugText += "\nFiles Array: "+getSearchPath()+"\n";
				debugText += "------------------------------\n";
				for(int j=0; j<files.size(); j++) {
					debugText += "File: "+files[j].name+"\n";
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
void ofxFileStream::drawDebugInfo(float x, float y) {
	
	ofSetColor(0xffffff);
	ofDrawBitmapString(debugText, 20, 20);
}
