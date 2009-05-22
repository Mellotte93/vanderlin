#pragma once
#include "ofMain.h"

class ofxFileStreamEventArgs : public ofEventArgs {
public:
	int		id;
	string	name;
	string	path;
};