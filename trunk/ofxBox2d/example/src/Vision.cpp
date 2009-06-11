#include "Vision.h"

// ---------------------------------------------------------
Vision::Vision() {
	
}
Vision::~Vision() {
	delete maskPixels;
	delete cameraPixels;
}


// ---------------------------------------------------------
void Vision::setup() {
	
	bPause			  = false;
	verbose			  = true;
	debug			  = false;
	cameraWidth		  = 1024;
	cameraHeight	  = 768;
	hasBlobs		  = false;
	bLearnBakground   = true;
	bDrawProceesVideo = false;
	
	
	flipV			  = false;
	flipH			  = false;
	bMaskImage		  = false;
	
	
#ifdef USE_LIVE
	camera.initGrabber(cameraWidth, cameraHeight);
#else
	camera.loadMovie("videos/whitebackground.mov");
	camera.play();
	camera.setVolume(0);
	cameraWidth  = camera.width;
	cameraHeight = camera.height;
#endif
	
	
	
	// the scale of the process video
	float scale = 4.0;
	cvWidth     = cameraWidth/scale;
	cvHeight    = cameraHeight/scale;
	
	printf("--- Camera Size [%i][%i] ---\n", cvWidth, cvHeight);
	
	
	
	// the cv images
	// I am the oginal size because i 
	// get scaled into me
	colorImage.allocate(cameraWidth, cameraHeight);
	grayImage.allocate(cameraWidth, cameraHeight);
	
	// we are smaller based on the scale factor
	processImage.allocate(cvWidth, cvHeight);
	backGroundImage.allocate(cvWidth, cvHeight);
	diffImage.allocate(cvWidth, cvHeight);
	processImage.allocate(cvWidth, cvHeight);
	
	originalImage.allocate(cvWidth, cvHeight);
	maskImage.allocate(cvWidth, cvHeight, OF_IMAGE_COLOR_ALPHA);
	maskPixels = new unsigned char[cvWidth*cvHeight*4];
	
	
	//allocate for pixels
	cameraPixels = new unsigned char[cameraWidth*cameraHeight*3];
	
	
	//countour 
	contourAnalysis.setSize(cvWidth, cvHeight);
	
	sw = 160;
	sh = 120;
	
	//GUI
	removeListeners();
	addListeners();
	
	gui.config->offset.set(20, sh+60);
	gui.config->titleHeight  = 20;
	gui.config->toggleHeight = 20;
	gui.config->buttonHeight = 20;
	
	gui.addFPSCounter();
	
	gui.addSlider("Threshold", &threshold, 0, 255);
	gui.addSlider("Min Area", &minArea, 0, cameraWidth*cameraHeight);
	gui.addSlider("Max Area", &maxArea, 0, cameraWidth*cameraHeight);
	gui.addSlider("Considered", &nConsidered, 0, 100);
	gui.addSlider("Smooth Percent", &smoothPct, 0.0, 1.0);
	gui.addSlider("Simple Percent", &simplePct, 0.0, 10.0);
	
	gui.addToggle("Fullscreen Video", &bDrawFullScreenVideo);
	gui.addToggle("Invert", &bInvert);
	gui.addToggle("Blur", &bBlur);
	gui.addToggle("Erode", &bErode);
	gui.addToggle("Dilate", &bDilate);
	gui.addToggle("Background Sub", &bAbsDiff);
	gui.addToggle("Process Video", &bDrawProceesVideo);
	gui.addToggle("Flip H", &flipH);
	gui.addToggle("Flip V", &flipV);
	gui.addToggle("Brightness Boost", &bBrighStrech);
	
	contentHolder.push_back(ofxSimpleGuiContent("Original Image", &camera, sw));
	contentHolder.push_back(ofxSimpleGuiContent("Background Image", &backGroundImage, sw));
	contentHolder.push_back(ofxSimpleGuiContent("Diff Image", &diffImage, sw));
	contentHolder.push_back(ofxSimpleGuiContent("Gray Image", &processImage, sw));
	contentHolder.push_back(ofxSimpleGuiContent("Contour Finder", &contourFinder, sw));
	
	gui.loadFromXML("visionSettings.xml");
	gui.setDraw(debug);
	
}

// ---------------------------------------------------------
void Vision::update() {
	
	camera.update();
	
	if(camera.isFrameNew()) {
		
		//set the camera pixles from new frame
		cameraPixels = camera.getPixels();
		
		//get the camera pixels
		colorImage.setFromPixels(camera.getPixels(), cameraWidth, cameraHeight);
		
		//lets flip it
		if(flipV || flipH) {
			colorImage.mirror(flipV, flipH);
		}
		
		grayImage = colorImage;
		
		//we need to resize to a faster size
		processImage.scaleIntoMe(grayImage);
		originalImage.scaleIntoMe(colorImage);
		
		//if(bBrighStrech) processImage.brightnessStretch();
		
		// grab the background for differancing
		if(bAbsDiff && bLearnBakground) {
			backGroundImage = processImage;
			bLearnBakground = false;
		}
		
		// abs diff image
		if(bAbsDiff) {
			processImage.absDiff(backGroundImage);
		}
		
		//set the diff image
		diffImage = processImage;
		
		
		
		// image processing filters
		if(bBlur) processImage.blur();
		if(bErode) processImage.erode();
		if(bDilate) processImage.dilate();
		
		//threshold
		processImage.threshold(threshold, bInvert);
		
		
		if(bMaskImage) {
			//maks out the pixels
			unsigned char * orgPixels = originalImage.getPixels();
			unsigned char * bgPixels = backGroundImage.getPixels();
			unsigned char * processPixels = processImage.getPixels();
			
			for(int i=0; i<cvWidth*cvHeight; i++) {
				
				if(processPixels[i] == 0) {
					maskPixels[i*4+0] = bgPixels[i];
					maskPixels[i*4+1] = bgPixels[i];	
					maskPixels[i*4+2] = bgPixels[i];	
					maskPixels[i*4+3] = 0;
				}
				else {
					maskPixels[i*4+0] = orgPixels[i*3+0];
					maskPixels[i*4+1] = orgPixels[i*3+1];	
					maskPixels[i*4+2] = orgPixels[i*3+2];	
					maskPixels[i*4+3] = 255;
				}
				
			}
			
			maskImage.setFromPixels(maskPixels, cvWidth, cvHeight, OF_IMAGE_COLOR_ALPHA);
		}
		
		
		//countour processing
		contourFinder.findContours(processImage, minArea, maxArea, nConsidered, false, true);
		
		if(contourFinder.nBlobs > 0) {
			
			//hasBlobs = true;
			int length_of_contour = contourFinder.blobs[0].pts.size();
			
			//smooth the contour
			contourSmooth.clear();
			contourSmooth.assign(length_of_contour, ofPoint());
			contourAnalysis.smooth(contourFinder.blobs[0].pts, contourSmooth, smoothPct);
			
			//simplify the contour
			contourSimple.clear();
			contourSimple.assign(length_of_contour, ofPoint());
			contourAnalysis.simplify(contourSmooth, contourSimple, simplePct);
			
			//find the delauny triangle in the contour
			triangles.findTriangles(contourSimple);
			
		}
		else {
			hasBlobs = false;
		}
	}
}

// ---------------------------------------------------------
void Vision::draw() {
	
	
	if(debug) {
		
		ofSetColor(0xffffff);
		contourAnalysis.draw(contourSmooth, sw*4, sh, sw, sh, true);	
		contourAnalysis.draw(contourSimple, sw*5, sh, sw, sh, true);	
		
		ofPoint p;
		float minDis = 50;
		for(int i=0; i<contourSimple.size(); i++) {
			
			p = contourSimple[i];
			float d = ofDist(mouse.x, mouse.y, p.x, p.y);	
			
			if(d < minDis) {
				ofSetColor(0xff00ff);
				ofLine(mouse.x, mouse.y, p.x, p.y);	
			}
		}
		for(int i=0; i<contentHolder.size(); i++) {
			contentHolder[i].draw(i*sw, 0);	
		}
	}
	
	gui.draw();
	
}

// ---------------------------------------------------------
void Vision::drawCameraFullScreen() {
	ofSetColor(0xffffff);
	if(bDrawFullScreenVideo) grayImage.draw(0, 0, ofGetWidth(), ofGetHeight());
}
void Vision::drawProcessVideo() {
	ofSetColor(0xffffff);
	if(bDrawProceesVideo) diffImage.draw(0, 0, ofGetWidth(), ofGetHeight());
}
void Vision::drawMaskImage() {
	ofEnableAlphaBlending();
	ofSetColor(0xffffff);
	maskImage.draw(0, 0, ofGetWidth(), ofGetHeight());
	ofDisableAlphaBlending();
}

// ---------------------------------------------------------
void Vision::addListeners() {
	ofAddListener(ofEvents.mousePressed, this, &Vision::mousePressed);
	ofAddListener(ofEvents.mouseMoved, this, &Vision::mouseMoved);
	ofAddListener(ofEvents.mouseDragged, this, &Vision::mouseDragged);
	ofAddListener(ofEvents.mouseReleased, this, &Vision::mouseReleased);	
	ofAddListener(ofEvents.keyPressed, this, &Vision::keyPressed);
	ofAddListener(ofEvents.keyReleased, this, &Vision::keyReleased);
}
void Vision::removeListeners() {
	ofRemoveListener(ofEvents.mousePressed, this, &Vision::mousePressed);
	ofRemoveListener(ofEvents.mouseMoved, this, &Vision::mouseMoved);
	ofRemoveListener(ofEvents.mouseDragged, this, &Vision::mouseDragged);
	ofRemoveListener(ofEvents.mouseReleased, this, &Vision::mouseReleased);	
	ofRemoveListener(ofEvents.keyPressed, this, &Vision::keyPressed);
	ofRemoveListener(ofEvents.keyReleased, this, &Vision::keyReleased);
}

// ---------------------------------------------------------
void Vision::mouseMoved(ofMouseEventArgs &e) {
	mouse.set(e.x, e.y);
}

// ---------------------------------------------------------
void Vision::mousePressed(ofMouseEventArgs &e) {
}

// ---------------------------------------------------------
void Vision::mouseDragged(ofMouseEventArgs &e) {
}

// ---------------------------------------------------------
void Vision::mouseReleased(ofMouseEventArgs &e) {
}

// ---------------------------------------------------------
void Vision::keyPressed(ofKeyEventArgs &e) {
	
	
	if(e.key == 'd') {
		debug = !debug;
		gui.setDraw(debug);
	}
	if(e.key == 'p') {
#ifndef USE_LIVE
		bPause = !bPause;
		if(bPause) camera.setSpeed(0.0);
		else camera.setSpeed(1.0);
#endif
	}
	if(e.key == ' ') bLearnBakground = true;
}

// ---------------------------------------------------------
void Vision::keyReleased(ofKeyEventArgs &e) {
}












