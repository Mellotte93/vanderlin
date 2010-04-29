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

#pragma once
#include "ofMain.h"
#include "ofxDirList.h"
#include "ofxTimer.h"
#include "ofxFileStreamEvent.h"

struct FileStreamFile {
	string name;
	string path;
};

class ofxFileStream : public ofxDirList {

private:
	
	string					debugText;
	string					folderPath;
	vector <FileStreamFile>	files;
	int						fileIDCount;
	ofxTimer				timer;
	bool					bSearching;
	bool					bVerbose;
	
public:
	
	int									numFiles;
	bool								bAllocated;
	ofEvent <ofxFileStreamEventArgs>	FILE_FOUND;
	
	// ---------------------------------------
	ofxFileStream();
	~ofxFileStream();
	
	// ---------------------------------------
	void allowImageExtensions();

	// ---------------------------------------
	FileStreamFile&	getFile(int i)  { return files.at(i); }
	int				getNumFiles()   { files.size(); }
	string			getSearchPath() { return folderPath; }
	
	// ---------------------------------------
	void setupFileStream(string path, float milliseconds);
	void setTimer(float milliseconds);
	void drawDebugInfo(float x=0, float y=0);
	
	// ---------------------------------------
	void setPath(string p);
	void checkForNewFiles(ofEventArgs &e);
};