
#pragma once
#include "ofMain.h"
#include "Box2D.h"

class ofxBox2dContactListener : public b2ContactListener {
	
public:
	
	ofxBox2dContactListener() {
	}

	void Add(const b2ContactPoint* point);		
	void Remove(const b2ContactPoint* point);
	

};



