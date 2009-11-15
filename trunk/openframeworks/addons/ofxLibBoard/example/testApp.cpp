

#include "testApp.h"
#include "stdio.h"

//--------------------------------------------------------------
void testApp::setup() {
	
	ofBackground(255, 255, 255);
	ofSetVerticalSync(true);
	
	bExport = false;
}

//--------------------------------------------------------------
void testApp::update(){
	

	
}

//--------------------------------------------------------------
void testApp::draw() {
	
	ofEnableAlphaBlending();
	
	if(bExport) {
		output.beginEPS("SVG");
	}
	
	output.fill();
	output.setColor(0, 0, 0);
	output.rect(mouseX, mouseY, 40, 40);
	
	float cx = ofGetWidth()/2;
	float cy = ofGetHeight()/2;
	
	for(int k=0; k< 10; k++) {
		output.pushGroup();
		for(int i=0; i<100; i++) {
			
			float n  = (float)i / 99.0;
			float px = cy + (cos(n * TWO_PI) * (n * 300));
			float py = cx + (sin(n * TWO_PI) * (n * 300));
			
			output.fill();
			output.setColor(i, n*30, 255-(n*255), 10 + (n * 20));
			output.circle(px, py, 4 + (n*20));
			
		}
		output.popGroup();
	}
	
	
	if(bExport) {
		bExport = false;
		output.endEPS();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	bExport = true;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

