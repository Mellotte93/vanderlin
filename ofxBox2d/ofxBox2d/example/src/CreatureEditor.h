

#pragma once
#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxBox2d.h"
#include "ofxXmlSettings.h"

#include "CreatureSettings.h"
#include "DustEngine.h"
#include "Bubble.h"
#include "Grass.h"
#include "Globbers.h"
#include "Vines.h"
#include "Mounds.h"
#include "Vision.h"

class CreatureEditor {
	
	
public:
	
	ofxXmlSettings		XML;
	ofxSimpleGuiToo*	gui;
	
	ofxBox2d *			box2d;
	Vision *			vision;
	
	bool				bEditGrass;
	int					selectedGrass;
	int					draggingGrassID;
	
	bool				bSaveCreatures;
	bool				bClearCreatures;
	bool				bDrawGrassVelocity;
	bool				bDrawGrassRestPoints;
	bool				bDrawGrassGrabPoints;
	
	// ------------------------------------------------------	
	ofPoint				mouse;

	
	// ------------------------------------------------------ utils
	unsigned char * grabPixels;
	int				grabPixelSize;
	
	// ------------------------------------------------------	
	CreatureEditor();
	~CreatureEditor();
	
	void setup();
	void update();
	void draw();

	// ------------------------------------------------------	
	void removeListeners();
	void addListeners();
	void mouseMoved(ofMouseEventArgs &e);
	void mousePressed(ofMouseEventArgs &e);	
	void mouseDragged(ofMouseEventArgs &e);	
	void mouseReleased(ofMouseEventArgs &e);
	void keyPressed(ofKeyEventArgs &e);
	void keyReleased(ofKeyEventArgs &e);
	
	// ------------------------------------------------------	
	void loadCreatures();
	void saveCreatures();
	
	void clearCreatures();

	// ------------------------------------------------------	
	DustEngine *					dustEngine;
	vector		<Grass>				grass;
		
	// ------------------------------------------------------	
	void addGrass(float x, float y);		
	
	
	
};