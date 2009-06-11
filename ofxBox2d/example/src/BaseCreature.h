#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "DustEngine.h"

class BaseCreature {
	
public:
	
	DustEngine * dustEngine;
	
	BaseCreature() {
		dustEngine = NULL;
	}
	
	void setDustEngine(DustEngine* theDust) {
		if(theDust != NULL) {
			dustEngine = theDust;
			printf("--- DustEngine Added:%p\n---", dustEngine);
		}
		else {
			printf("--- Error in DustEngine [NULL] ---");	
		}
	}
	
};