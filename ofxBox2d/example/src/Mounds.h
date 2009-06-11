#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxMSASpline.h"

class Mounds : public ofxBox2dBaseShape {
	
	
public:
	
	vector		<ofPoint>			pnts;
	vector      <ofxBox2dJoint>		joints;
	vector		<ofxBox2dCircle>	nodes;
	vector		<ofxBox2dCircle>	outerNodes;
	
	
	vector		<ofPoint>			spline;
	
	
	ofRectangle						bounds;
	ofxBox2dLine					surface;
	ofxBox2dCircle					centerNode;
	ofxMSASpline2D					spline2D;
	
	bool							bShapeCreated;
	float							splineStep;
	int								stepAmount;
	float							stregth, middleStrength;
	float							damp;
	
	//------------------------------------------------
	Mounds() {
		
		bShapeCreated	= false;
		splineStep		= 1.0f/30.0;
		stepAmount		= 35;
		
		middleStrength  = 3.0;
		stregth			= 3.0;
		damp			= 0.0;
		
		spline2D.clear();
		spline.clear();
		nodes.clear();
		outerNodes.clear();
		
		
	}
	//------------------------------------------------
	
	void setup(ofxBox2d* b2d) {
		world = b2d->getWorld();
		surface.setWorld(world);
	}
	
	//------------------------------------------------
	void addNode(float x, float y) {
		pnts.push_back(ofPoint(x, y));
	}
	
	//------------------------------------------------
	void createBounds() {
		bounds.x = 4000;		bounds.y = 4000;
		bounds.width = 0;	bounds.height = 0;
		
		for(int i=0; i<pnts.size(); i++) {
			if(pnts[i].x < bounds.x) bounds.x = pnts[i].x; 
			if(pnts[i].y < bounds.y) bounds.y = pnts[i].y; 
			if(pnts[i].x > bounds.width) bounds.width = pnts[i].x-bounds.x; 
			if(pnts[i].y > bounds.height) bounds.height = pnts[i].y-bounds.y; 
		}
	}
	
	//------------------------------------------------
	void createSpline() {
		
		for(int i=0; i<stepAmount; i++) {
			float f = ofMap(i, 0.0, (float)stepAmount, 0.0f, 1.0f);
			ofPoint pos = spline2D.sampleAt(f);
			spline.push_back(pos);
		}
		//spline.push_back(pnts.back());	
		//spline.push_back(ofPoint(pnts.back().x+20, pnts.back().y+20));	
		
	}
	
	//------------------------------------------------
	void createShape() {
		
		int count = (int)pnts.size();
		
		//get the spline from input points
		for(int i=0; i<pnts.size(); i++) {
			spline2D.push_back(ofPoint(pnts[i].x, pnts[i].y+20));	
		}
		
		createBounds();		//get the bound
		createSpline();		//create the new spline points
		
		// shape nodes. these are made based of the 
		// first spline. we then make normal nodes.
		for(int i=1; i<spline.size(); i++) {
			
			bool bFixed = false;
			if(i == 1 || i == spline.size()-1) {
				bFixed = true;
			}
			
			ofPoint pos = spline[i];
			ofxBox2dCircle n;
			n.setFixedRotation(true);
			n.setRotationFriction(100);
			n.enableGravity(false);
			n.setPhysics(1.0, 0.4, 0.0);
			n.setup(world, pos.x, pos.y, 5, true);
			nodes.push_back(n);
		}
		
		/*
		 // outter joint and center joint connnections
		 for(int i=0; i<nodes.size(); i++) {
		 
		 //joint with eachother
		 if(i > 0) {
		 ofxBox2dJoint joint;
		 joint.setWorld(world);
		 joint.addJoint(nodes[i].body, nodes[i-1].body, 5.9f, 1.0f);
		 joints.push_back(joint);
		 }
		 
		 //joints to the middle node
		 ofxBox2dJoint jointMiddle;
		 jointMiddle.setWorld(world);
		 jointMiddle.addJoint(nodes[i].body, centerNode.body, 1.9f, 1.0f);
		 joints.push_back(jointMiddle);
		 
		 }
		 */
		// outer joint connections
		/*for(int i=1; i<nodes.size(); i++) {
		 ofxBox2dJoint jointMiddle;
		 jointMiddle.setWorld(world);
		 jointMiddle.addJoint(nodes[i].body, nodes[i-1].body, 19.9f, 3.0f);
		 joints.push_back(jointMiddle);
		 }*/
		
		
		//normals are fun
		int direction = 1;
		ofxVec2f normal;
		ofxVec2f delta;
		ofxPoint2f mid;
		ofxPoint2f normalScaled;
		
		// these are the normalize nodes from the spline.
		// these will be the outer nodes.
		
		for(int i=1; i<spline.size(); i++) {
			
			bool bFixed = false;
			if(i == 1 || i == spline.size()-1) {
				bFixed = true;
			}		
			
			if(i > 0) {
				delta = spline[i] - spline[i-1];
			
			
			delta.normalize();
			
			if(direction) normal.set(delta.y, -delta.x);
			else normal.set(-delta.y, delta.x);
			
			normalScaled = spline[i] + (normal * 20);
			
			ofxBox2dCircle n;
			n.setFixedRotation(true);
			n.setRotationFriction(100);
			n.enableGravity(false);
			n.setPhysics(1.0, 0.4, 0.3);
			n.setup(world, normalScaled.x, normalScaled.y, 1, bFixed);
			outerNodes.push_back(n);
			}
		}
		
		//link joints
		for(int i=0; i<outerNodes.size(); i++) {
			if(i > 0) {
				ofxBox2dJoint jointMiddle;
				jointMiddle.setWorld(world);
				jointMiddle.addJoint(outerNodes[i].body, outerNodes[i-1].body, middleStrength, damp);
				joints.push_back(jointMiddle);
			}
		}
		
		//cross joints
		int incount = 0;
		for(int i=0; i<nodes.size(); i++) {
			if(incount < (int)outerNodes.size()) {
				ofxBox2dJoint jointMiddle;
				jointMiddle.setWorld(world);
				jointMiddle.addJoint(nodes[i].body, outerNodes[incount].body, stregth, damp);
				joints.push_back(jointMiddle);
			}
			incount ++;
		}
		
		//cross half joints
		int crossInc = 0;
		for(int i=0; i<outerNodes.size(); i++) {
			
			if(crossInc < (int)nodes.size()-1) {
				ofxBox2dJoint jointMiddleA;
				jointMiddleA.setWorld(world);
				jointMiddleA.addJoint(outerNodes[crossInc].body, nodes[crossInc+1].body, stregth, damp);
				joints.push_back(jointMiddleA);
				
				if(i > 0) {
					ofxBox2dJoint jointMiddleB;
					jointMiddleB.setWorld(world);
					jointMiddleB.addJoint(outerNodes[crossInc].body, nodes[crossInc-1].body, stregth, damp);
					joints.push_back(jointMiddleB);
				}
			}
			
			if(i > 0) {
				ofxBox2dJoint jointMiddleBC;
				jointMiddleBC.setWorld(world);
				jointMiddleBC.addJoint(outerNodes[i].body, outerNodes[i-1].body, stregth, damp);
				joints.push_back(jointMiddleBC);
			}
			
			crossInc ++;
		}
		
		for(int i=0; i<nodes.size(); i++) {
			b2FilterData data;
			data.categoryBits = 0x0000;
			data.maskBits = 0x0000;
			data.groupIndex = -3;
			nodes[i].setFilterData(data);
		}
		for(int i=0; i<outerNodes.size(); i++) {
			b2FilterData data;
			data.categoryBits = 0x0003;
			data.maskBits = 0x1;
			data.groupIndex = -3;
			outerNodes[i].setFilterData(data);
		}		
		
		
		bShapeCreated = true;
	}
	
	//------------------------------------------------
	void update() {
		
		
		if(pnts.size() >= 3 && bShapeCreated) {
			
			createBounds();		//get bounds of the points
			
			surface.clear();	//clear all surface points
			//createSpline();		//create the new spline points
			
			for(int i=0; i<outerNodes.size(); i++) {
				ofPoint pos = outerNodes[i].getPosition();
				//surface.edgeDef.filter.categoryBits = 0x0002;
				//surface.edgeDef.filter.maskBits = 0x0001;
				surface.edgeDef.filter.groupIndex = -3;
				surface.addPoint(pos.x, pos.y);
			}
			surface.createShape();

			
			// add the points into the mound
			/*
			 for(int i=0; i<stepAmount; i++) {
			 float f = ofMap(i, 0.0, (float)stepAmount, 0.0f, 1.0f);
			 ofPoint pos = spline2D.sampleAt(f); //spline[i]
			 surface.edgeDef.filter.categoryBits = 0x0004;
			 surface.edgeDef.filter.maskBits = 0x0002;
			 surface.addPoint(pos.x, pos.y);	
			 }
			 
			 
			 for(float f=0; f<=1-splineStep; f+=splineStep) {
			 ofPoint pos = spline2D.sampleAt(f + splineStep - 0.0001f);
			 surface.edgeDef.filter.categoryBits = 0x0004;
			 surface.edgeDef.filter.maskBits = 0x0002;
			 surface.addPoint(pos.x, pos.y);	
			 }
			 */
			/*
			 for(float f=0; f<=1-splineStep; f+=splineStep) {
			 //ofPoint pos = spline2D.sampleAt(f);
			 ofPoint pos = spline2D.sampleAt(f + splineStep - 0.0001f);
			 ofSetColor(160, 160, 160);
			 ofNoFill();
			 ofCircle(pos.x, pos.y, 2);
			 }	
			 */			
			
			
			b2FilterData data;
			data.categoryBits = 0x0002;
			data.maskBits = ~0x0002&0x0001;
			//surface.setFilterData(data);
			
		}
	}
	
	//------------------------------------------------
	void draw() {
		
		//bouing box
		/*ofNoFill();
		 ofSetColor(133, 10, 200);
		 ofRect(bounds.x, bounds.y, bounds.width, bounds.height);*/
		
		
		
		if(bShapeCreated) {
			
			//the shape of the spline
			ofEnableAlphaBlending();
			ofSetColor(100, 100, 100);
			ofFill();
			ofBeginShape();
			//ofVertex(outerNodes.front().x, outerNodes.front().y+100);
			for(int i=0; i<outerNodes.size(); i++) {
				ofPoint p = outerNodes[i].getPosition();
				ofVertex(p.x, p.y);
			}
			//ofVertex(spline.back().x, spline.back().y+100);
			ofEndShape(false);
			ofDisableAlphaBlending();
			
			//normals are fun
			int direction = 1;
			ofxVec2f normal;
			ofxVec2f delta;
			ofxPoint2f mid;
			ofxPoint2f normalScaled;
			for(int i=1; i<outerNodes.size(); i++) {
				
				delta = outerNodes[i].getPosition() - outerNodes[i-1].getPosition();
                delta.normalize();
				
                if(direction){
                    normal.set(delta.y, -delta.x);
                }else{
                    normal.set(-delta.y, delta.x);
                }
				
				//mid = (contourSmooth[i] + contourSmooth[i-1]) * 0.5;
				normalScaled = outerNodes[i].getPosition() + (normal * 20);
				ofLine(outerNodes[i].getPosition().x, outerNodes[i].getPosition().y, 
					   normalScaled.x, normalScaled.y);
				
				outerNodes[i].addAttractionPoint(normalScaled, 0.6);
			}
		/*	
			//draw the surface and center node
			surface.draw();
			
			//nodes and joints
			for(int i=0; i<outerNodes.size(); i++)  outerNodes[i].draw();
			for(int i=0; i<nodes.size(); i++)		nodes[i].draw();
			for(int i=0; i<joints.size(); i++)		joints[i].draw(80);
		*/	
		}
		
		//the node points
		for(int i=0; i<pnts.size(); i++) {
			ofPoint p = pnts[i];
			ofFill();
			ofSetColor(100, 100, 100);
			ofCircle(p.x, p.y, 3);				
		}
	}
};