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
	
	strokeSize = 0.5;
	bStroke = false;
	bFill   = false;
	bDraw = true;
	bFirstPoint = false;
	
}
ofxCairo::~ofxCairo() {
	cairo_destroy (cr); 
	cairo_surface_destroy (surface);
}

// -----------------------
void ofxCairo::setup(int w, int h, int outputFormat) {
	
	width = w; height = h;
	output = outputFormat;	
	filename = ofToDataPath("cairo.pdf");
	
	
	surface = cairo_pdf_surface_create (filename.c_str(), w, h);
	cr = cairo_create (surface);
	
	setLineWidth(strokeSize);
	
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
void ofxCairo::beginShape() {
	ofBeginShape();
	if(bOutput) {
		bFirstPoint = true;
		cairo_new_path (cr);
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
void ofxCairo::endShape(bool bClose) {
	ofEndShape(bClose);
	if(bOutput) {
		
		
		cairo_move_to (cr, lastPoint.x, lastPoint.y);
		
		cairo_close_path(cr);
		cairo_stroke(cr);
	}
}

// -----------------------
void ofxCairo::vertex(float x, float y) {
	ofVertex(x, y);
	if(bOutput) {
		if(bFirstPoint) {
			cairo_move_to (cr, x, y);
			bFirstPoint = false;
		}
		cairo_line_to (cr, x, y);
		lastPoint.set(x, y);
		
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
	ofEnableAlphaBlending();
	cairo_new_sub_path( cr );
	
	if(bOutput) {
		arc( x, y, radius, 0, TWO_PI);	
	}
	
	ofFill();
	ofCircle(x, y, radius);
	
	if(bStroke) {
		ofNoFill();
		ofSetColor(strokeColor.r, strokeColor.g, strokeColor.b, strokeColor.a);
		ofCircle(x, y, radius);
	}
	ofDisableAlphaBlending();
}

// -----------------------
void ofxCairo::begin() {
	bOutput = true;
	
	// draw the background
	if(bOutput) {
		float * bgColor = ofBgColorPtr();
		cairo_set_source_rgb(cr, bgColor[0], bgColor[1], bgColor[2]);
		cairo_paint( cr );
	}
	ofEnableAlphaBlending();
}
void ofxCairo::end(string filename) {
	
	cairo_save (cr);
	bOutput = false;
	
	//snippet_do (cr, no, width, HEIGHT);
	//cairo_show_page (cr);
	//cairo_restore (cr);
	
	printf("PDF Saved...\n");
	ofDisableAlphaBlending();
}









