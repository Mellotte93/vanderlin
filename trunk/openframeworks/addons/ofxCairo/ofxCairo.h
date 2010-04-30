/*
 *  ofxCairo.h
 *  Cario
 *
 *  Created by Todd Vanderlin on 4/28/10.
 *  Copyright 2010 Interactive Design. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "cairo-features.h"
#include "cairo-pdf.h"
#include "cairo.h"

// ----------------------------------------------------

enum {
	OF_OUTPUT_PDF
};

// ----------------------------------------------------

class ofxCairo {
private:
	cairo_matrix_t m;
	cairo_path_t  * path;
	cairo_surface_t *surface;
	cairo_t *cr;
	
	int output;
	float width, height;
	bool bOutput;
	
	string filename;
	
	ofColor strokeColor;
	float strokeSize;
	bool bStroke, bFill;
	bool bDraw;
	ofPoint lastPoint;
	
	bool bShouldClose;
	bool bFirstPoint;
	int  whichShapeMode;
	vector<double *>curvePts;
	void clearAllVertices();
	
public:
	
	// -----------------------
	ofxCairo();
	~ofxCairo();
	
	// -----------------------
	void begin(string fileName, int w, int h, int outputFormat);
	void end();
	
	
	// -----------------------
	void enableDraw();
	void disableDraw();
	
	void setLineWidth(float lineWidth);
	void setStrokeColor(float r, float g, float b, float a=255);
	void setStrokeColor(ofColor &c);
	
	void noStoke();
	void noFill();
	void fill();
	
	void setColor(int hexColor);
	void setColor(ofColor &c);
	void setColor(int r, int g, int b, int a=255);
	void setStroke(float size);
	
	// -----------------------
	void pushClippingGroup();
	void popClippingGroup();
	void pushGroup();
	void popGroup();
	
	// -----------------------
	void translate(float x, float y);
	void rotate(float angle);
	void scale(float sx, float sy);
	void pushMatrix();
	void popMatrix();
	
	
	
	
	// -----------------------
	void rect(float x, float y, float w, float h);
	void circle(float x, float y, float radius);
	void arc( double x, double y, double radius, double angle1, double angle2 );
	void line(float x1, float y1, float x2, float y2);
	void bezier(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
	void curve(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
	void triangle(float x1,float y1, float x2, float y2, float x3, float y3);

	// -----------------------
	void beginShape();
	void endShape(bool bClose=false);
	void vertex(float x, float y);
	void curveVertex(float x, float y);
	void bezierVertex(float x1, float y1, float x2, float y2, float x3, float y3);


};








