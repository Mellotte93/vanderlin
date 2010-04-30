/*
 *  ofxCairo.cpp
 *  Cario
 *
 *  Created by Todd Vanderlin on 4/28/10.
 *  Copyright 2010 Interactive Design. All rights reserved.
 *
 */

#include "ofxCairo.h"

// -----------------------
ofxCairo::ofxCairo() {
	
	surface = NULL;
	bOutput = false;
	
	strokeSize	   = 0.5;
	bStroke		   = false;
	bDraw		   = true;
	bFill		   = true;
	bFirstPoint	   = false;	
	bShouldClose   = false;
	whichShapeMode = 0;	
	
}
ofxCairo::~ofxCairo() {
	cairo_destroy (cr); 
	cairo_surface_destroy (surface);
}

// -----------------------
void ofxCairo::begin(string fileName, int w, int h, int outputFormat) {
	
	ofEnableAlphaBlending();
	bOutput = true;
	
	if(bOutput) {
	
		width = w; height = h;
		output = outputFormat;	
		filename = ofToDataPath(fileName);
		surface = cairo_pdf_surface_create (filename.c_str(), w, h);
		cr = cairo_create (surface);
		
		setLineWidth(strokeSize);
		
		// draw the background
		float * bgColor = ofBgColorPtr();
		cairo_set_source_rgb(cr, bgColor[0], bgColor[1], bgColor[2]);
		cairo_paint( cr );
	}
	
	
	
	
}

// -----------------------
void ofxCairo::end() {
	
	cairo_save (cr);
	bOutput = false;
	
	//snippet_do (cr, no, width, HEIGHT);
	//cairo_show_page (cr);
	//cairo_restore (cr);
	
	printf("PDF Saved...\n");
	ofDisableAlphaBlending();
}

// -----------------------
void ofxCairo::pushMatrix() {
	glPushMatrix();
}
void ofxCairo::popMatrix() {
	glPopMatrix();
	if(bOutput) {
		cairo_identity_matrix ( cr );
		//scale(1.0, 1.0);
	}
}

// -----------------------
void ofxCairo::translate(float x, float y) {
	glTranslatef(x, y, 0);
	if(bOutput) {
	 	//cairo_matrix_init_translate(&m, x, y);
		cairo_translate( cr, x, y );
	}
}

// -----------------------
void ofxCairo::rotate(float angle) {
	if(bOutput) {
		cairo_rotate( cr, ofDegToRad(angle) );
	}
	
	glRotatef(angle, 0, 0, 1);
}

// ----------------------- not working...
void ofxCairo::scale(float sx, float sy) {
	//glScalef(sx, sy, 0);
	//if(bOutput) cairo_scale( cr, sx, sy );
}

// -----------------------
void ofxCairo::pushClippingGroup() {
	if(bOutput) {
		cairo_push_group( cr );
	}
}
void ofxCairo::popClippingGroup() {
	if(bOutput) {
		cairo_pop_group_to_source( cr );
		cairo_paint( cr );
	}
}


// -----------------------
void ofxCairo::enableDraw() {
	bDraw = true;
}
void ofxCairo::disableDraw() {
	bDraw = false;
}

// -----------------------
void ofxCairo::beginShape() {
	ofBeginShape();
	if(bOutput) {
		bFirstPoint		= true;
		bShouldClose	= false;
		cairo_new_path (cr);
	}
	
}

// -----------------------
void ofxCairo::endShape(bool bClose) {
	if(bDraw){
		ofEndShape(bClose);
	}
	if(bOutput){
		
		//catmull roms - we need at least 4 points to draw
		if( whichShapeMode == 1 && curvePts.size() > 3){												
			
			//we go through and we calculate the bezier of each 
			//catmull rom curve - smart right? :)
			for (int i = 1; i< curvePts.size()-2; i++) {
				
				ofPoint prevPt(	curvePts[i-1][0],	curvePts[i-1][1]);
				ofPoint startPt(curvePts[i][0],		curvePts[i][1]);							
				ofPoint endPt(	curvePts[i+1][0],	curvePts[i+1][1]);
				ofPoint nextPt(	curvePts[i+2][0],	curvePts[i+2][1]);
				
				//SUPER WEIRD MAGIC CONSTANT = 1/6
				//Someone please explain this!!! 
				//It works and is 100% accurate but wtf!
				ofPoint cp1 = startPt + ( endPt - prevPt ) * (1.0/6);
				ofPoint cp2 = endPt + ( startPt - nextPt ) * (1.0/6);						
				
				
				//if this is the first line we are drawing 
				//we have to start the path at a location
				if( i == 1 ){
					cairo_move_to( cr, startPt.x, startPt.y );
					bShouldClose = true;
				}
				cairo_curve_to( cr, cp1.x, cp1.y, cp2.x, cp2.y, endPt.x, endPt.y );
			}						
		}
		
		// need to work on this
		if(bShouldClose){		
			
			
			bShouldClose = false;
		}
		
		// move to the last point
		cairo_move_to (cr, lastPoint.x, lastPoint.y);
		cairo_close_path(cr);
		
		if(bFill) {
			cairo_fill( cr );
		}
		else {
			cairo_stroke( cr );
		}
		
		
		//we want to clear all the vertices
		//otherwise we keep adding points from
		//the previous file - cool but not what we want!
		clearAllVertices();
		
	}
}

// -----------------------
void ofxCairo::vertex(float x, float y) {
	
	if(bDraw) ofVertex(x, y);
	
	if(bOutput) {
		
		//clear curve vertices
		if(whichShapeMode == 1){
			clearAllVertices();	
		}
		
		whichShapeMode = 0;
		
		if(bFirstPoint){
			cairo_move_to (cr, x, y);
			bFirstPoint = false;
			bShouldClose = true;
		}else{
			cairo_line_to (cr, x, y);
		}
		
		lastPoint.set(x, y);
		
	}
}

// -----------------------
void ofxCairo::curveVertex(float x, float y) {
	if(bDraw){
		ofCurveVertex(x, y);
	}
	if(bOutput){
		
		whichShapeMode = 1;
		
		double* point = new double[2];
		point[0] = x;
		point[1] = y;
		curvePts.push_back(point);
	}
}

//----------------------------------------------------------	
//this takes three arguments as it is based off the idea that at least one 
//inital polyVertex has already been set. 					
void ofxCairo::bezierVertex(float x1, float y1, float x2, float y2, float x3, float y3){
	if(bDraw){
		ofBezierVertex(x1, y1, x2, y2, x3, y3);
	}
	if(bOutput){
		
		//clear curve vertices
		if(whichShapeMode == 1){
			clearAllVertices();	
		}
		
		whichShapeMode = 2;					
		
		//we can only add a bezier curve if the curve
		//has started with a polyVertex
		//so as long as this is not the first point we add it
		if(!bFirstPoint){
			cairo_curve_to( cr, x1, y1, x2, y2, x3, y3);
		}
		
	}
}	

// -----------------------
void ofxCairo::bezier(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
	if(bDraw){
		ofBezier(x1, y1, x2, y2, x3, y3, x4, y4);
	}
	if(bOutput){
		
		cairo_move_to( cr, x1, y1 );
		cairo_curve_to( cr, x2, y2, x3, y3, x4, y4);
		
		if(bFill){
			cairo_fill( cr );
		}
		else {
			cairo_stroke( cr );
		}
	}
}

// -----------------------
void ofxCairo::curve(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
	
	ofCurve(x1, y1, x2, y2, x3, y3, x4, y4);						
	
	if(bOutput){
		
		//we treat the catmull rom curve as a bezier 
		//by finding the approrpriate control points 
		//to make the same curve
		
		//x1 y1 = p1
		//x2 y2 = p2
		//x3 y3 = p3
		//x4 y4 = p4
		
		//control point at p2 is parallel 
		//to the line defined by p3 - p1
		
		//control point at p3 is parallel
		//to the line defined by p4 - p2
		
		ofPoint p1(x1, y1);
		ofPoint p2(x2, y2);
		ofPoint p3(x3, y3);
		ofPoint p4(x4, y4);
		
		//SUPER WEIRD MAGIC CONSTANT = 1/6 (this works 100% can someone explain it?)
		ofPoint cp1 = p2 + ( p3 - p1 ) * (1.0/6);
		ofPoint cp2 = p3 + ( p2 - p4 ) * (1.0/6);						
		
		cairo_move_to( cr, x2, y2 );
		cairo_curve_to( cr, cp1.x, cp1.y, cp2.x, cp2.y, x3, y3 );
		
		if(bFill) {
			cairo_fill( cr );
		}
		else {
			cairo_stroke( cr );
		}
	}
	
}

// -----------------------
void ofxCairo::triangle(float x1,float y1, float x2, float y2, float x3, float y3){
	if(bDraw){
		ofTriangle(x1, y1,  x2,  y2,  x3,  y3);
	}
	if(bOutput){
		
		cairo_move_to(cr, x1, y1);
		cairo_line_to(cr, x2, y2);
		cairo_line_to(cr, x3, y3);
		
		if(bFill) cairo_fill( cr );
		else cairo_stroke( cr );
	}
}	

// -----------------------
void ofxCairo::line(float x1,float y1,float x2,float y2) {
	ofLine(x1, y1, x2, y2);
	if(bOutput) {
		cairo_move_to(cr, x1, y1);
		cairo_line_to(cr, x2, y2);
		cairo_stroke(cr);
	}
}

// -----------------------
void ofxCairo::rect(float x, float y, float w, float h) {
	ofRect(x, y, w, h);
	
	if (bOutput) {
		if(ofGetRectMode() == OF_RECTMODE_CENTER) {
			cairo_rectangle(cr, x-w/2, y-h/2, w, h);
			cairo_fill(cr);
		}
		else {
			cairo_rectangle(cr, x, y, w, h);
			cairo_fill(cr);			
		}
		
	}
}

// -----------------------
void ofxCairo::arc( double x, double y, double radius, double angle1, double angle2 ) {
	if(bOutput) {
		
		
		cairo_arc( cr, x, y, radius, angle1, angle2 );
		if(bFill) cairo_fill(cr);
		else {
			//setColor(strokeColor);
			cairo_stroke(cr);
		}		
		
		
	}
}

// -----------------------
void ofxCairo::noStoke() {
	bStroke = false;
	
}
void ofxCairo::noFill() {
	ofNoFill();
	bFill = false;
}
void ofxCairo::fill() {
	ofFill();
	bFill = true;
}


// -----------------------
void ofxCairo::setLineWidth(float lineWidth) {
	strokeSize = lineWidth;
	if(bOutput) {
		cairo_set_line_width( cr, lineWidth );
	}
	ofSetLineWidth(lineWidth);
}

// -----------------------
void ofxCairo::setStrokeColor(ofColor &c) {
	setStrokeColor(c.r, c.g, c.b, c.a);
}
void ofxCairo::setStrokeColor(float r, float g, float b, float a) {
	strokeColor.r = r;
	strokeColor.g = g;
	strokeColor.b = b;
	strokeColor.a = a;
	bStroke = true;
}
void ofxCairo::setColor(int hexColor) {
	int r = (hexColor >> 16) & 0xff;
	int g = (hexColor >> 8) & 0xff;
	int b = (hexColor >> 0) & 0xff;
	setColor(r,g,b);
}

// -----------------------
void ofxCairo::setColor(ofColor &c) {
	setColor(c.r, c.g, c.b, c.a);
}
void ofxCairo::setColor(int r, int g, int b, int a) {
	ofSetColor(r, g, b, a);
	
	if(bOutput) {
		cairo_set_source_rgba(cr, (float)r/255.0, (float)g/255.0, (float)b/255.0, (float)a/255.0);
	}
}

// -----------------------
void ofxCairo::circle(float x, float y, float radius) {
	if(bDraw) {
		ofFill();
		ofCircle(x, y, radius);
		
		if(bStroke) {
			ofNoFill();
			ofSetColor(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);
			ofCircle(x, y, radius);
		}
	}
	if(bOutput) {
		cairo_new_sub_path( cr );
		arc( x, y, radius, 0, TWO_PI);	
	}
	
}


// -----------------------
void ofxCairo::clearAllVertices(){
	for(vector<double*>::iterator itr=curvePts.begin();
		itr!=curvePts.end();
		++itr){
		delete [] (*itr);
	}
	curvePts.clear();
}	








