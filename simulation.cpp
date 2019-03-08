#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

#define MASS 1      //Mass of each body
#define WIDTH 200
#define HEIGHT 100
#define DEPTH 400
#define TOTALSTEP 720000
#define PRINTSTEP 100   //Print after each 100 steps
#define DELTA 0.01    //Delta T
#define NUMBALLS 1000 //Number of Balls
#define RADIUS 0.5    //Radius of Ball
#define SKIPLINE 8    //Skip first 8 line of the input file

int numthreads;

struct X{
  double a, b, c;
};

class Body{

public:
  int collison; //body body collision
  double rx, ry, rz;  //position of center
  double vx, vy, vz;  //velocity
  double fx, fy, fz;  //force
  double hvx, hvy, hvz; //half-step velocity
  double tvx, tvy, tvz; //for handling velocity after body-body collision

  Body(): rx(0.0), ry(0.0), rz(0.0), vx(0.0), vy(0.0), vz(0.0), fx(0.0), fy(0.0), fz(0.0), hvx(0.0), hvy(0.0), hvz(0.0), collison(0), tvx(0.0), tvy(0.0), tvz(0.0){}
  void calculateforce(Body& b);
  void updatevelocity();
  void updateposition();
  void resetforce();
  void checkcollision(Body& b);
  void resettempvelocity();
  void updateVEL();
  void save(ofstream& of);
  void load(ifstream& inf);
  friend ostream& operator<<(ostream &out, const Body& b);
};





//Print Body Center positions
ostream& operator<<(ostream& out, const Body& b){
  cout << b.rx << " " << b.ry << " " << b.rz;
  return out;
}

//Calculate Euclidean Distance Mod between two bodies
double calculatedistance(Body& b1, Body& b2){
  double disx = b2.rx - b1.rx;
  double disy = b2.ry - b1.ry;
  double disz = b2.rz - b1.rz;
  return disx*disx + disy*disy + disz*disz;
}

//if distnace between them is less than equal to the sum of radius
void Body::checkcollision(Body& b){
  if(calculatedistance(*this, b) <= 2*RADIUS){
    this->collison = 1;
    this->tvx += b.vx;
    this->tvy += b.vy;
    this->tvz += b.vz;
  }
}


void Body::resettempvelocity(){
  this->tvx = this->tvy = this->tvz = 0.0;
  this->collison = 0;
}


void Body::updateVEL(){
  if(this->collison){
    this->vx = this->tvx;
    this->vy = this->tvy;
    this->vz = this->tvz;
  }
}


//Calculate force exerted by body b on this body and similarly on body b by this body
//formula taken from wiki
void Body::calculateforce(Body& b){
  double moddist = calculatedistance(*this, b);

  double modforce = 0.0;
  if(moddist != 0.0)
    modforce = (MASS * MASS)/ moddist;
  double disx = b.rx - this->rx;
  double disy = b.ry - this->ry;
  double disz = b.rz - this->rz;
  if(moddist == 0.0) ;
  else{
    this->fx += (modforce * disx)/ sqrt(moddist);
    this->fy += (modforce * disy)/ sqrt(moddist);
    this->fz += (modforce * disz)/ sqrt(moddist);
}

  //b.fx += -this->fx;
  //b.fy += -this->fy;
  //b.fz += -this->fz;
}



//Update velocity of this body and half-step velocity
void Body::updatevelocity(){
  this->hvx = this->vx + (this->fx * DELTA)/ (2 * MASS);
  this->hvy = this->vy + (this->fy * DELTA)/ (2 * MASS);
  this->hvz = this->vz + (this->fz * DELTA)/ (2 * MASS);
  /*
  if(isnan(this->hvx) or isnan(this->hvy) or isnan(this->hvz)){
    cout << "NAN Here" << endl;
  }
  */

  this->vx = this->hvx + (this->fx * DELTA) / (2 * MASS);
  this->vy = this->hvy + (this->fy * DELTA) / (2 * MASS);
  this->vz = this->hvz + (this->fz * DELTA) / (2 * MASS);
}

//Reset force to 0 after updating position
void Body::resetforce(){
  this->fx = this->fy = this->fz = 0.0;
}

//Update position of the particle to check the boundary of the wall
//if the current position of the center +- radius lies outside of the wall, then update the center of Body
//to be tangential to the wall and change the velocity to the opposite direction
void Body::updateposition(){
  this->rx = this->rx + this->hvx * DELTA;
  this->ry = this->ry + this->hvy * DELTA;
  this->rz = this->rz + this->hvz * DELTA;


  if((this->rx + RADIUS) >= WIDTH){
    this->rx = WIDTH - RADIUS;
    this->vx = -this->vx;
  }else if((this->rx - RADIUS) <= 0){
    this->rx = RADIUS;
    this->vx = -this->vx;
  }

  if((this->ry + RADIUS) >= HEIGHT){
    this->ry = HEIGHT - RADIUS;
    this->vy = -this->vy;
  }else if(this->ry - RADIUS <= 0){
    this->ry = RADIUS;
    this->vy = -this->vy;
  }

  if(this->rz + RADIUS >= DEPTH){
    this->rz = DEPTH - RADIUS;
    this->vz = -this->vz;
  }else if(this->rz - RADIUS <= 0){
    this->rz = RADIUS;
    this->vz = -this->vz;
  }

}


//Read file for initial position
void readfile(const char* filename, Body* bodies){
  fstream fin(filename);
  string line;
  for(int i = 0 ; i < SKIPLINE ; i++){
    getline(fin, line);
  }

  double rx, ry, rz;
  int i = 0;
  while(fin >> rx >> ry >> rz){
    bodies[i].rx = rx;
    bodies[i].ry = ry;
    bodies[i].rz = rz;
    i++;
    //cout << bodies[i] << endl;
  }

  /*
  for(i = 0 ; i < NUMBALLS ; i++){
    cout << bodies[i] << endl;
  }
  */
}


void run_simulation(Body* bodies){
  int i,j;
  #pragma omp parallel for num_threads(numthreads) private(i,j)
  for(i = 0 ; i < NUMBALLS ; i++){
    for(j = 0 ; j < NUMBALLS ; j++){
      if(j != i)
        bodies[i].calculateforce(bodies[j]);
      }
  }

  #pragma omp parallel for num_threads(numthreads) private(i)
  for(i = 0 ; i < NUMBALLS ; i++)
    bodies[i].updatevelocity();

  #pragma omp parallel for num_threads(numthreads) private(i)
  for(i = 0 ; i < NUMBALLS ; i++)
    bodies[i].updateposition();

  #pragma omp parallel for num_threads(numthreads) private(i)
  for(i = 0 ; i < NUMBALLS ; i++)
    bodies[i].resetforce();

  /*

  #pragma omp parallel for num_threads(numthreads) private(i)
  for(i = 0 ; i < NUMBALLS ; i++){
    for(j = 0 ; j < NUMBALLS ; j++){
      if(j != i)
        bodies[i].checkcollision(bodies[j]);
    }
  }

  #pragma omp parallel for num_threads(numthreads)  private(i)
  for(i = 0 ; i < NUMBALLS ; i++)
    bodies[i].updateVEL();

  #pragma omp parallel for num_threads(numthreads)  private(i)
  for(i = 0 ; i < NUMBALLS ; i++)
    bodies[i].resettempvelocity();
  */

}


void writefile(const char* filename, Body* bodies){
  //TODO
  ofstream outfile;
  outfile.open(filename, ios::binary | ios::out | ios::app);
  for(int i = 0 ; i < NUMBALLS ; i++){
    X x;
    x.a = bodies[i].rx;
    x.b = bodies[i].ry;
    x.c = bodies[i].rz;
    outfile.write(reinterpret_cast<char*>(&x), sizeof(x));
    outfile.write("\n", 1);
  }
}

void Body::save(ofstream& of){
  of.write((char*)&rx, sizeof(rx));
  of.write((char*)&ry, sizeof(ry));
  of.write((char*)&rz, sizeof(rz));
}


void Body::load(ifstream& inf){
  inf.read((char*)&rx, sizeof(rx));
  inf.read((char*)&ry, sizeof(ry));
  inf.read((char*)&rz, sizeof(rz));
}

void writeBinary(ofstream& of, Body* bodies){
  for(int i = 0 ; i < NUMBALLS ; i++){
    bodies[i].save(of);
  }
}

void readBinary(ifstream& inf, Body* bodies){
  for(int i = 0 ; i < NUMBALLS ; i++){
    bodies[i].load(inf);
  }
}

void printBodies(Body* bodies){
  for(int i = 0 ; i < NUMBALLS ; i++){
    cout << bodies[i] << endl;
    cout << bodies[i].vx << " " << bodies[i].vy << " " << bodies[i].vz << endl;
  }
}


int main(int argc, char* argv[]){

  //cout << "How many Threads?" << endl;
  //cin >> numthreads;
  if(argc != 2){
    cout << "Usage: ./a.out <numthreads>" << endl;
    exit(EXIT_FAILURE);
  }

  numthreads = atoi(argv[1]);
  cout << numthreads << endl;

  Body* bodies = new Body[NUMBALLS];
  readfile("Trajectory.txt", bodies);

  ofstream myfile;
  myfile.open("Output_3.dat", ios::binary | ios::out | ios::app);
  writeBinary(myfile, bodies);

  double start = omp_get_wtime();

  for(int i = 0 ; i < TOTALSTEP ; i++){
    run_simulation(bodies);
    if((i+1)%PRINTSTEP == 0){
      writeBinary(myfile, bodies);
      //printBodies(bodies);
      cout << "Writing Done..." << endl;
    }
  }
  myfile.close();
  cout << "Complete" << endl;

  cout << "Time: " << omp_get_wtime()-start << endl;


  /*
  ifstream inf;
  inf.open("Output.dat", ios::in);
  while(!inf.eof()){
    readBinary(inf, bodies);
    for(int i = 0 ; i < NUMBALLS ; i++){
      cout << bodies[i] << endl;
    }
  }
  */

  delete[] bodies;





}
