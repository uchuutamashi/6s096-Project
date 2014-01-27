#ifndef _6S096_NBODY_H
#define _6S096_NBODY_H

#define DELTA 0.01 //length of time step for integration

#include <fstream>
#include <vector>
#include <stddef>
#include <Vector3.h>

typedef Vector3<double> Vector3d;

namespace nbody{
  class Body{
      double _mass;
      std::vector<Vector3d> _pos;  //time series for position (i=0,1,..,n)
      std::vector<Vector3d> _vel;  //time series for velocity (i=0,1,..,n)
      std::vector<Vector3d> _acc;  //time series for acceleration (i=0,1,..,n-1 , the current accel needs to be input)
    public:                  
      Body(double,Vector3d,Vector3d) //create mass with inital position and velocity 
      void evolve(Vector3d accel)    //takes acceleration and find position & velocity for the next timestep
  } //class Body

  class Simulation{
      std::vector<Body> _body;
      size_t tnow; //number of time steps already evolved
    public:
      void loadRu(std::ifstream&);
      void saveRu(std::ofstream&) const;
      void evolveSystemFor(double t);
  } //class Simulation
} //namespace nbody

#endif // _6S096_RATIONAL_H
