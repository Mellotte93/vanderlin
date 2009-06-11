
#pragma once
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxContourAnalysis.h"
#include "ofxSimpleGuiToo.h"
#include "ofxDelaunay.h"

//#define USE_LIVE


class Vision {
	
public:
	
	
#ifdef USE_LIVE
	ofVideoGrabber		camera;
#else
	ofVideoPlayer		camera;
#endif
	
	// ---------------------------------------------------------
	
	ofPoint					mouse;
	bool					verbose, debug;
	
	bool					hasBlobs;
	bool					bPause;
	bool					bDrawFullScreenVideo;
	bool					bDrawProceesVideo;
	
	// ---------------------------------------------------------
	ofxSimpleGuiToo					gui;
	vector <ofxSimpleGuiContent>	contentHolder;
	
	// ---------------------------------------------------------
	
	int						cameraWidth, cameraHeight;
	
	int						width, sw;
	int						height, sh;
	int						cvWidth, cvHeight;
	int						threshold;
	
	int						minArea, maxArea;
	int						nConsidered;
	bool					bInvert;
	bool					bBlur, bErode, bDilate;
	bool					bAbsDiff, bLearnBakground;
	bool					bBrighStrech;
	
	bool					flipV;
	bool					flipH;
	bool					bMaskImage;
	
	// ---------------------------------------------------------
	
	ofPoint *				cropPoints[4];
	
	float					simplePct, smoothPct;
	vector <ofPoint>		contourSmooth;
	vector <ofPoint>		contourSimple;
	ofxDelaunay				triangles;
	
	// ---------------------------------------------------------
	
	unsigned char *			cameraPixels;
	
	ofxCvColorImage			colorImage;
	ofxCvGrayscaleImage		grayImage;
	ofxCvGrayscaleImage		backGroundImage;
	ofxCvGrayscaleImage		diffImage;
	ofxCvGrayscaleImage		processImage;
	
	ofxCvContourFinder		contourFinder;
	ofxContourAnalysis		contourAnalysis;
	
	ofxCvColorImage			originalImage;
	ofImage					maskImage;
	unsigned char *			maskPixels;
	
	
	void drawCameraFullScreen();
	void drawProcessVideo();
	void drawMaskImage();
	
	// ---------------------------------------------------------
	int getCameraWidth()  { return cameraWidth; }
	int getCameraHeight() { return cameraHeight; }
	
	int getWidth()  { return cvWidth; }
	int getHeight() { return cvHeight; }
	
	// --------------------------------------------------------- get triangle info
	int numTriangles() { return triangles.numTriangles(); }
	DelaunayTriangle& getTriangle(int i) {
		return triangles.getTriangle(i);
	}
	
	// ---------------------------------------------------------
	ofColor getColorAt(float x, float y) {
		
		if(x > cameraWidth) x = cameraWidth;
		else if(x < 0) x = 0;
		else if(y > cameraHeight) y = cameraHeight;
		else if(y < 0) y = 0;
		
		
		
		unsigned char * px = camera.getPixels();
		ofColor color;
		int index = (int)y*cameraWidth+(int)x;
		
		color.r = px[index*3+0];
		color.g = px[index*3+1];
		color.b = px[index*3+2];
		color.a = 255;
		
		return color;
	}

	// ---------------------------------------------------------
	int getPixelAt(float x, float y, int color) {
		
		if(x > cameraWidth) x = cameraWidth;
		else if(x < 0) x = 0;
		else if(y > cameraHeight) y = cameraHeight;
		else if(y < 0) y = 0;
		
		if(color > 3) color = 3;
		else if(color < 0) color = 0;
		
		return cameraPixels[((int)y*cameraWidth+(int)x)*3+color];
	}
	
	// ---------------------------------------------------------
	Vision();
	~Vision();
	
	// ---------------------------------------------------------
	void setup();
	void update();
	void draw();

	// ---------------------------------------------------------
	void removeListeners();
	void addListeners();
	void mouseMoved(ofMouseEventArgs &e);
	void mousePressed(ofMouseEventArgs &e);	
	void mouseDragged(ofMouseEventArgs &e);	
	void mouseReleased(ofMouseEventArgs &e);
	void keyPressed(ofKeyEventArgs &e);
	void keyReleased(ofKeyEventArgs &e);
	
	
	
};






