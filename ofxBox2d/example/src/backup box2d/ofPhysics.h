

#ifndef OF_PHYSICS_H
#define OF_PHYSICS_H

#include "ofMain.h"
#include "Box2D.h"
#include "Render.h"
#include "Biped.h"

#define k_maxContactPoints	2048
#define MAX_SHAPES			4096 

//------------------------------------------------------------------- Contact Listener
class contactListener : public b2ContactListener {
	public:
		int contactCount;
		contactListener() {
			contactCount = 0;
		}
			
		void Add(const b2ContactPoint* point) {
			contactCount ++;
		
			/*if (test->m_pointCount == k_maxContactPoints)
			{
			return;
			}

			ContactPoint* cp = test->m_points + test->m_pointCount;
			cp->shape1 = point->shape1;
			cp->shape2 = point->shape2;
			cp->position = point->position;
			cp->normal = point->normal;
			cp->id = point->id;
			cp->state = e_contactAdded;

			++test->m_pointCount;*/
		}
		void Persist(const b2ContactPoint* point) {
		}
		void Remove(const b2ContactPoint* point) {
			contactCount --;
		}
		int getCount() {
			return contactCount;
		}
};

//------------------------------------------------------------------- Newton Type
class ofNewton {
	public:
		float	density;
		float	friction;	//value between 0.0 -> 1.0
		float   bounce;		//Value btw 0.0 -> 1.0 this is ho bouncy it is

		ofNewton(){ }
};
	
	
//------------------------------------------------------------------- Main Physics Class	
class ofPhysics {

public:

	//friend class DestructionListener;
	//friend class BoundaryListener;
	friend class contactListener;
	
	
	float				sz;
	float				scale;
	
	
	//ContactPoint			m_points[k_maxContactPoints];
	//int32					m_pointCount;
	//DestructionListener	m_destructionListener;
	//BoundaryListener		m_boundaryListener;
	
	contactListener		of_contactListener;
	b2AABB				of_worldAABB;
	DebugDraw			of_debugDraw;
	b2World*			of_world;
	b2Body*				m_bomb;
	b2MouseJoint*		m_mouseJoint;
	b2Body*				ground;
	b2Body*				mainBody;
	b2MouseJoint*		mouseJoint;

	//------------------------------------------------------------------- Construct Destruct
	ofPhysics()  { }
	~ofPhysics() { 
		delete of_world;
		of_world = NULL;
	}

	//------------------------------------------------------------------- Setup
	void setup() {
		
		m_mouseJoint = NULL;
		scale = 30.0;
				
		//Build the Physics of_world
		of_worldAABB.lowerBound.Set(-100.0f, -100.0f);
		of_worldAABB.upperBound.Set(100.0f, 100.0f);
		b2Vec2  gravity(0.0f, 10.0f);
		bool	doSleep = true;
		of_world		= new b2World(of_worldAABB, gravity, doSleep);
		
		
		//Build a ground
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set((300/2)/scale, 768/scale);
		//groundBodyDef.angle = 0.25f * b2_pi;
		ground = of_world->CreateBody(&groundBodyDef);
		//Ground
		b2PolygonDef groundShapeDef;
		groundShapeDef.SetAsBox(300/scale, 10.0/scale);		
		//ground->CreateShape(&groundShapeDef);
		
		//Drawing Redering
		of_debugDraw.setScale(scale);
		of_debugDraw.SetFlags(1);
		of_world->SetDebugDraw(&of_debugDraw);
		
		//Build A Box Ground
		//addClosedWorld();
		
		//Contact Listener
		of_world->SetContactListener(&of_contactListener);

	}
	
	//------------------------------------------------------------------- Set Params
	void setScale(float s) {
		scale = s;
		of_debugDraw.setScale(s);
	}
	void setGravity(float x, float y) {
		b2Vec2  gravity(x, y);
		of_world->SetGravity(gravity);
	}
	
	//------------------------------------------------------------------- Update
	void update() {
		
		/*
		sz += .04;
		b2CircleDef cd;
		cd.radius = 3.0+cos(sz);
		cd.density = 3.0f;
		
		b2BodyDef bd;
		bd.position.Set((1024/2)/scale, (768/2)/scale);
		b2Body* body = of_world->CreateBody(&bd);
		body->CreateShape(&cd);
		body->SetMassFromShapes();
		*//*
		b2Body* node = of_world->GetBodyList();
		while(node) {
			b2Body* b = node;
			node = node->GetNext();
			//GameActor* myActor = (GameActor*)b->GetUserData();
			if(node->IsSleeping()) {
				of_world->DestroyBody(b);
			}
		}*/

		ofNoFill();
		ofSetColor(255, 255, 255);
		ofRect(0, 0, ofGetWidth(), ofGetHeight());


		float32 timeStep = 1.0f / 30.0f;
		int32 iterations = 10;

		of_world->Step(timeStep, iterations);
		of_world->Validate();		
		
		
		string  debug = "";
				debug += "\n\nBody Count: "+ofToString(of_world->GetBodyCount());
				debug += "\n\nScale Factor:" + ofToString(scale);
				debug += "\n\nContact Count: "+ofToString(of_contactListener.getCount());
		//ofDrawBitmapString(debug, 5, 0);
		//ofDrawFPS();
		//of_world->DestroyBody(body);	
	}
	
	/*
	The idea with adding shapes is that you can do it as simple as possible.
	You can just add a shape with min params. like:
	
	x	y	w	h
	
	from there you can pass more a ofNewton type. This type has varibles with 
	the newtons laws.
	
	density		friction	bounce
	
	of you pass the ofNewton type you need to also pass a angle. This is because
	ofPhysics figures you are trying to create a special shape so pass everything
	to it and make it happy;
		
	*/	
		
	//------------------------------------------------------------------- Add Rect
	void addRect(float x, float y, float w, float h) {
		ofNewton para;
		para.friction = 0.3f;
		para.density  = 5.0f;
		para.bounce   = 0.1f;
		addRect(x, y, w, h, 0, para);
	}						
	void addRect(float x, float y, float w, float h, float angle, bool fixed) {
		if(fixed) {
			ofNewton para;
			para.friction = 0.3f;
			para.density  = 0.0f;
			para.bounce   = 0.1f;
			addRect(x, y, w, h, angle, para);
		}
		else {
			ofNewton para;
			para.friction = 0.3f;
			para.density  = 5.0f;
			para.bounce   = 0.1f;
			addRect(x, y, w, h, angle, para);
		}
	}
	void addRect(float x, float y, float w, float h, float angle, ofNewton newton) {
		
		//Rect Shape
		b2PolygonDef sd;
		sd.SetAsBox(w/scale, h/scale);
		sd.density		= newton.density;
		sd.restitution  = newton.bounce;
		//sd.filter.categoryBits = 0x0002;
		//sd.filter.maskBits = ~0x0002;
	
		//Build Body for shape
		b2BodyDef bodyDef;
		
		bodyDef.position.Set(x/scale, y/scale);
		bodyDef.angle = angle;

		mainBody = of_world->CreateBody(&bodyDef);
		mainBody->CreateShape(&sd);
		mainBody->SetMassFromShapes();
	}

	//------------------------------------------------------------------- Add Circle	
	void addCircle(float x, float y, float r, bool fixed) {
		if(fixed) {
			ofNewton para;
			para.friction = 0.0;
			para.density  = 0.0;
			para.bounce   = 0.0;
			addCircle(x, y, r, para);
		}
		else {
			ofNewton para;
			para.friction = 0.0;
			para.density  = 1.0;
			para.bounce   = 0.0;
			addCircle(x, y, r, para);
		}
	}
	void addCircle(float x, float y, float r) {
		ofNewton para;
		para.friction = 0.0;
		para.density  = 1.0;
		para.bounce   = 0.0;
		addCircle(x, y, r, para);
	}	
	void addCircle(float x, float y, float r, ofNewton newton) {
		//if(of_contactListener.getCount() < MAX_SHAPES) {
			//Circle Shape
			b2CircleDef circle;
			circle.radius   = r/scale;
			circle.density		= newton.density;
			circle.restitution  = newton.bounce;
				
			//Build Body for shape
			b2BodyDef bodyDef;
			bodyDef.position.Set(x/scale, y/scale);
			mainBody = of_world->CreateBody(&bodyDef);
			mainBody->CreateShape(&circle);
			mainBody->SetMassFromShapes();
		//}
	}

	/*
	These are some fancy utils. They are fun and handy for quick
	things that you want to make.
	*/
	
	//------------------------------------------------------------------- addCloseWorld
	void addClosedWorld() {
		//addRect(0, 0, ofGetWidth(), 10, 0, true);
		//addRect(0, 1400, 10, ofGetHeight(), 0, true);
		//addRect(0, ofGetHeight()-10, ofGetWidth(), 10, 0, true);
		//addRect(ofGetWidth()-10, 1400, 10, ofGetHeight(), 0, true);
	}
	
	//------------------------------------------------------------------- addBridge
	void addBridge(float x, float y, int numPlanks, float w, float h) {
				
		//This is the plank
		b2PolygonDef sd;
		sd.SetAsBox(w/scale, h/scale);
		sd.density	= 20.0f;
		sd.friction = 0.2f;
		
		b2RevoluteJointDef jd;		
		b2Body* prevBody = ground;
		
		for(int i=0; i<numPlanks; ++i) {
			
			b2Vec2 pos(x/scale+(1.0f*i), y/scale);
			
			b2BodyDef bd;
			bd.position  = pos;
			b2Body* body = of_world->CreateBody(&bd);
			body->CreateShape(&sd);
			body->SetMassFromShapes();
			  
			jd.Initialize(prevBody, body, pos);
			of_world->CreateJoint(&jd);
			prevBody = body;
		}

		b2Vec2 anchor(x/scale+(1.0f*numPlanks), y/scale);
		jd.Initialize(prevBody, ground, anchor);
		of_world->CreateJoint(&jd);
	
	}

	//------------------------------------------------------------------- add Pyramid
	void addPyramid() {
		
		float	w = 14;
		int		size = ofRandom(10, 40);
		float	intx = 1024-(w*size)*2;
		float	inty = 768-(w*size)/2;
		
		b2PolygonDef sd;
		float32 a = 0.5f;
		sd.SetAsBox(a, a);
		sd.density = 5.0f;

		b2Vec2 x(-10.0f, 0.75f);
		b2Vec2 y;
		b2Vec2 deltaX(0.5625f, 2.0f);
		b2Vec2 deltaY(1.125f, 0.0f);

		for (int32 i = 0; i < 25; ++i)
		{
			y = x;

			for (int32 j = i; j < 25; ++j)
			{
				b2BodyDef bd;
				bd.position = y;
				b2Body* body = of_world->CreateBody(&bd);
				body->CreateShape(&sd);
				body->SetMassFromShapes();

				y += deltaY;
			}

			x += deltaX;
		}
	}

	//------------------------------------------------------------------- add Rag Doll
	void addRagDoll(float x, float y) {
		Biped* m_biped = new Biped(of_world, b2Vec2(x/scale, y/scale));
	}
	
	
	//------------------------------------------------------------------- Mouse Events
	void mousePressed(float x, float y) {
			//if(of_contactListener.getCount() < MAX_SHAPES) {
			// Make a small box.
			b2Vec2 p(x/scale, y/scale);
			b2AABB aabb;
			b2Vec2 d;
			d.Set(.1/scale, .1/scale);
			aabb.lowerBound =  - d;
			aabb.upperBound = p + d;

			// Query the of_world for overlapping shapes.
			const int32 k_maxCount = 1000;
			b2Shape* shapes[k_maxCount];
			int32 count = of_world->Query(aabb, shapes, k_maxCount);
			b2Body* body = NULL;
			
			for(int i=0; i<count; ++i) {
				b2Body* shapeBody = shapes[i]->GetBody();
				if(shapeBody->IsStatic() == false && shapeBody->GetMass() > 0.0f) {
					bool inside = shapes[i]->TestPoint(shapeBody->GetXForm(), p);
					if(inside) {
						body = shapes[i]->GetBody();
						break;
					}
				}
			}

			if(body){
				b2MouseJointDef md;
				md.body1 = of_world->GetGroundBody();
				md.body2 = body;
				md.target = p;
				md.maxForce = 1000.0f * body->GetMass();
				mouseJoint = (b2MouseJoint*)of_world->CreateJoint(&md);
				body->WakeUp();
				
				b2Shape* shape = body->GetShapeList();
				b2CircleShape* circle = (b2CircleShape*)body;
				//circle->radius = 40;
				//shape->restitution = 1.0;
				
				//shape.
			}
		//}	
	}
	void mouseDragged(float x, float y) {
		b2Vec2 p(x/scale, y/scale);
		b2Vec2 k(ofRandom(0, 3), 0);

		if(mouseJoint) {
			b2PolygonDef sd;
			sd.SetAsBox(15/scale, 2/scale);
			sd.density	= 20.0f;
			sd.friction = 0.2f;

			mouseJoint->SetTarget(p);
			//mouseJoint->CreateShape(&sd);
		}
	}
	void mouseReleased() {
		if(mouseJoint) {
			of_world->DestroyJoint(mouseJoint);
			mouseJoint = NULL;
		}
	}

};
#endif














