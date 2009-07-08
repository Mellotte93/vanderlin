

import processing.opengl.*;


ArrayList particles;


// ----------------------------------------------

void setup() {

  size(1024, 768);
  background(255);

  // init the arrayList 
  particles = new ArrayList();


}

// ----------------------------------------------
void draw() {

  //background(255);

  fill(255, 50);
  rect(0, 0, width, height);



  for(int i=0; i<particles.size(); i++) {

    Particle p = (Particle)particles.get(i);

    // check to see if we are colliding with 
    // the other particle
    for(int j=0; j<i; j++) {
      Particle other = (Particle)particles.get(j);
      
      // now check to see if we hit
      float distance = p.pos.dist(other.pos);
      float minDis   = p.radius/2 + other.radius/2;
      float scale = 4.9;
      
      if(distance < minDis) {
      
        float magnitude = distance / minDis;
       
        PVector forceVec = PVector.sub(other.pos, p.pos);
        forceVec.normalize();
        
        p.vel.x = p.vel.x - forceVec.x * scale * (1-magnitude);
        p.vel.y = p.vel.y - forceVec.y * scale * (1-magnitude);
       
        other.vel.x = other.vel.x + forceVec.x * scale * (1-magnitude);
        other.vel.y = other.vel.y + forceVec.y * scale * (1-magnitude);
     
      }
     


    }


    // lets check this particle to see if the opacity is less than 0 
    // if so lets kill it.
    if(p.opactity <= 0.0) {
      particles.remove(i); 
    }


    p.checkBounds();
    p.update();
    p.render();

  }



}

// ----------------------------------------------
void addParticle(float x, float y) {

  Particle p = new Particle();
  p.init();
  p.pos.x = x;
  p.pos.y = y;
  particles.add(p);
}

// ----------------------------------------------

void mouseDragged() {
  addParticle(mouseX, mouseY);
}









