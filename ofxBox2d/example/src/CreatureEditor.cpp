#include "CreatureEditor.h"


CreatureEditor::CreatureEditor() {
}

CreatureEditor::~CreatureEditor() {
	delete dustEngine;
	delete gui;
	delete grabPixels;	
}

// ------------------------------------------------------
void CreatureEditor::setup() {
	
	
	bSaveCreatures    = false;
	bClearCreatures   = false;
	bDrawGrassVelocity = false;
	bDrawGrassRestPoints = false;
	bDrawGrassGrabPoints = false;
	
	gui = new ofxSimpleGuiToo();
	gui->addToggle("Save Creatures", &bSaveCreatures);
	gui->addToggle("Clear Creatures", &bClearCreatures);
	gui->addToggle("Edit The Grass", &bEditGrass);
	gui->addToggle("Draw Grass Vectors", &bDrawGrassVelocity);
	gui->addToggle("Draw Grass RestPoints", &bDrawGrassRestPoints);
	gui->addToggle("Draw Grass GrabPoints", &bDrawGrassGrabPoints);
	
	gui->loadFromXML("creatureSettings.xml");
	
	
	selectedGrass   = 0;	// the count for the grass that you are editing
	draggingGrassID = -1;
	
	
	removeListeners();
	addListeners();
	
	dustEngine = new DustEngine();
	
	// grabbing effects 10 x 10 (RGB)
	grabPixelSize = 50;
	grabPixels = new unsigned char[grabPixelSize*grabPixelSize*3];
}



// ------------------------------------------------------ Save The Creatures
void CreatureEditor::saveCreatures() {
	
	XML.clear();
	XML.addTag("grass");
	XML.pushTag("grass");
	
	for(int i=0; i<grass.size(); i++) {
		XML.addTag("item");
		XML.pushTag("item", i);
		
		XML.addValue("pos_x", grass[i].getRootPosition().x);
		XML.addValue("pos_y", grass[i].getRootPosition().y);
		
		XML.addValue("anchor_x", grass[i].anchorNode.getPosition().x);
		XML.addValue("anchor_y", grass[i].anchorNode.getPosition().y);
		
		
		XML.popTag();
	}
	XML.popTag();
	XML.saveFile("creatures.xml");
	
	printf("--- Creatures Saved ---");
}

// ------------------------------------------------------ Load The Creatures
void CreatureEditor::loadCreatures() {
	if(XML.loadFile("creatures.xml")) {
		
		XML.pushTag("grass");
		int numGrassTags = XML.getNumTags("item");
		
		for(int i=0; i<numGrassTags; i++) {
			
			float pos_x		= XML.getValue("item:pos_x", 33.0, i);
			float pos_y		= XML.getValue("item:pos_y", 33.0, i);
			float anchor_x  = XML.getValue("item:anchor_x", 33.0, i);
			float anchor_y  = XML.getValue("item:anchor_y", 33.0, i);
			
			Grass g;
			g.setDustEngine(dustEngine);
			g.setup(box2d->getWorld(), pos_x, pos_y);
			g.setAncorNode(anchor_x, anchor_y, true);
			grass.push_back(g);
			
			printf("--- grass added [%i] ---\n", grass.size());
			
		}
		
		XML.popTag();
		printf("--- Creatures Loaded ---");
		
	}
}

// ------------------------------------------------------ Add Some Grass
void CreatureEditor::addGrass(float x, float y) {
	Grass g;
	g.setup(box2d->getWorld(), x, y);
	g.setDustEngine(dustEngine);
	grass.push_back(g);
	printf("--- grass added [%i] ---\n", grass.size());
}

// ------------------------------------------------------
void CreatureEditor::addListeners() {
	ofAddListener(ofEvents.mousePressed, this, &CreatureEditor::mousePressed);
	ofAddListener(ofEvents.mouseMoved, this, &CreatureEditor::mouseMoved);
	ofAddListener(ofEvents.mouseDragged, this, &CreatureEditor::mouseDragged);
	ofAddListener(ofEvents.mouseReleased, this, &CreatureEditor::mouseReleased);	
	ofAddListener(ofEvents.keyPressed, this, &CreatureEditor::keyPressed);
	ofAddListener(ofEvents.keyReleased, this, &CreatureEditor::keyReleased);
}
void CreatureEditor::removeListeners() {
	ofRemoveListener(ofEvents.mousePressed, this, &CreatureEditor::mousePressed);
	ofRemoveListener(ofEvents.mouseMoved, this, &CreatureEditor::mouseMoved);
	ofRemoveListener(ofEvents.mouseDragged, this, &CreatureEditor::mouseDragged);
	ofRemoveListener(ofEvents.mouseReleased, this, &CreatureEditor::mouseReleased);	
	ofRemoveListener(ofEvents.keyPressed, this, &CreatureEditor::keyPressed);
	ofRemoveListener(ofEvents.keyReleased, this, &CreatureEditor::keyReleased);
	
}

// ------------------------------------------------------
void CreatureEditor::mouseMoved(ofMouseEventArgs &e) {
	mouse.set(e.x, e.y);
	
	//are we over the grass we want
	if(bEditGrass && grass.size() > 0) {
		for(int i=0; i<grass.size(); i++) {
			float d = ofDist(mouse.x, mouse.y, grass[i].anchorNode.getPosition().x, grass[i].anchorNode.getPosition().y);
			if(d < 10) grass[i].bGrassSelected = true;
			else	   grass[i].bGrassSelected = false;	
		}
	}
	
}
void CreatureEditor::mousePressed(ofMouseEventArgs &e) {
	
	//edit the grass
	if(bEditGrass && grass.size() > 0) {
		
		for(int i=0; i<grass.size(); i++) {
			float d = ofDist(mouse.x, mouse.y, grass[i].anchorNode.getPosition().x, grass[i].anchorNode.getPosition().y);
			if(d < 10) {
				draggingGrassID = i;
				grass[draggingGrassID].setAncorNode(mouse.x, mouse.y);	
			}
		}
	}
}
void CreatureEditor::mouseDragged(ofMouseEventArgs &e) {
	mouse.set(e.x, e.y);
	
	//edit the grass
	if(bEditGrass && grass.size() > 0 && draggingGrassID != -1) {
		grass[draggingGrassID].setAncorNode(mouse.x, mouse.y);
	}
	
}
void CreatureEditor::mouseReleased(ofMouseEventArgs &e) {
	if(bEditGrass && grass.size() > 0 && draggingGrassID != -1) {
		grass[draggingGrassID].setAncorNode(mouse.x, mouse.y, true);		
	}
	draggingGrassID = -1;
}
void CreatureEditor::keyPressed(ofKeyEventArgs &e) {
	
	if(e.key == 'w') {
		addGrass(mouse.x, mouse.y);
	}
	
	
	//edit the grass
	if(bEditGrass && grass.size() > 0) {
		if(e.key == ' ') {
			
			grass[selectedGrass].editMode(false);
			selectedGrass ++;
			selectedGrass %= grass.size();
			
		}
	}
}
void CreatureEditor::keyReleased(ofKeyEventArgs &e) {
	
}

// ------------------------------------------------------ clear the creatures
void CreatureEditor::clearCreatures() {
	/*bEditGrass = false;
	 for(int i=0; i<grass.size(); i++) {
	 grass[i].destroy();
	 }
	 grass.clear();*/
	XML.clear();
	XML.saveFile("creatures.xml");
}

// ------------------------------------------------------
void CreatureEditor::update() {
	
	//are we going to save the creatures
	if(bSaveCreatures) {
		saveCreatures();
		bSaveCreatures = false;
	}
	
	//clear the creatures
	if(bClearCreatures) {
		clearCreatures();
		bClearCreatures = false;
	}
	
	//the curious grass (fungi)
	for(int i=0; i<grass.size(); i++) {
		
		grass[i].editMode(bEditGrass);
		grass[i].bDrawVelocity   = bDrawGrassVelocity;
		grass[i].bDrawRestPoints = bDrawGrassRestPoints;
		grass[i].bDrawGrabPoint  = bDrawGrassGrabPoints;
		
		ofPoint gp = grass[i].getTopNode().getPosition();
		if(ofDist(mouse.x, mouse.y, gp.x, gp.y) < GRASS_MIN_GRAB_DIS) {
			grass[i].setTopNode(mouse.x, mouse.y);
		}
		
		grass[i].update();
	}
	
	//the fun dust
	dustEngine->update();
	
	//edit mode
	if(bEditGrass && grass.size() > 0) {
		for(int i=0; i<grass.size(); i++) grass[i].editMode(bEditGrass);
	}	
}

// ------------------------------------------------------
void CreatureEditor::draw() {
	
	

	for(int i=0; i<vision->contourSimple.size(); i++) {
		
		// camera points
		ofPoint cp = vision->contourSimple[i];
		ofPoint wp = cp; 
		wp.x = ofMap(cp.x, 0.0, (float)vision->getWidth(), 0.0, (float)ofGetWidth());
		wp.y = ofMap(cp.y, 0.0, (float)vision->getHeight(), 0.0, (float)ofGetHeight());
		
		// are we close enought to 
		// the grass for a touch
		for(int j=0; j<grass.size(); j++) {
			
			ofPoint gp = grass[j].getTopNode().getPosition();
			float dis  = ofDist(wp.x, wp.y, gp.x, gp.y);
			
			// are we close enough
			if(dis < 100) {
				
				// only if we are not
				// eating already
				if(!grass[j].bEating) {
					
					// add some dust
					Dust dust;
					dust.setPos(wp);
					dust.acc.y = .5;
					dustEngine->addDust(dust);
					
					// pixel grabbing dust
					/*
					 PixelDust pd;
					 pd.setPos(wp);
					 
					 
					 //get the video pixels
					 for(int vx=0; vx<grabPixelSize; vx++) {
					 for(int vy=0; vy<grabPixelSize; vy++) {
					 
					 int index = (vy*grabPixelSize+vx); 
					 
					 float cx = (cp.x + vx) + grabPixelSize; 
					 float cy = (cp.y + vy) + grabPixelSize;
					 
					 grabPixels[index*3+0] = vision->getPixelAt(cx, cy, 0);
					 grabPixels[index*3+1] = vision->getPixelAt(cx, cy, 1);
					 grabPixels[index*3+2] = vision->getPixelAt(cx, cy, 2);
					 
					 }
					 }			
					 pd.setPixels(wp.x, wp.y);
					 dustEngine->addPixelDust(pd);
					 */
					
				}
				
				grass[j].setTopNode(wp.x, wp.y);
				
			}
		}
		
		
	}

	for(int i=0; i<grass.size(); i++) grass[i].draw();
	dustEngine->draw();
	gui->draw();
}
