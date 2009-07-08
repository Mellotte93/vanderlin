

class Particle {
  
  float theta;
  float   opactity;
  PVector pos, vel, acc;
  float   radius;
  float   maxSpeed;

  // ---------------------------------------------- constructor 
  Particle() {
  }


  // ---------------------------------------------- init
  void init() {

    theta = 0;

    opactity = 200;
    pos = new PVector();
    acc = new PVector();
    vel = new PVector();

    radius = random(30, 40);

    // set a acc
    acc.x = random(-0.2, 0.2);
    acc.y = 0.4;

    maxSpeed = 1.0;
  }

  // ---------------------------------------------- check bounds
  void checkBounds() {

    if(pos.x+(radius/2) >= width) {
      vel.x *= -1.0;
      acc.x *= -1.0;
    }
    if(pos.x <= radius/2) {
      vel.x *= -1.0;
      acc.x *= -1.0;
    }
    if(pos.y+(radius/2) >= height) {
      vel.y *= -1.0;
      acc.y *= -1.0;
    }
    if(pos.y <= radius/2) {
      vel.y *= -1.0;
      acc.y *= -1.0;
    }

  }

  // ---------------------------------------------- update
  void update() {

    opactity -= .03;

    vel.add(acc);
    pos.add(vel);

    vel.limit(maxSpeed);
  }

  // ---------------------------------------------- render
  void render() {

    fill(33, opactity);
    noStroke();
    ellipse(pos.x, pos.y, radius, radius);
  }


}










