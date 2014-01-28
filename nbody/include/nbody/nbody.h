#ifndef _6S096_NBODY_H
#define _6S096_NBODY_H

#define DELTA 0.1 //length of time step for integration

#include <fstream>
#include <vector>
#include <Vector3.h>

namespace nbody{
  class Body{
      double _mass;
      std::vector<Vector3d> _pos;  //time series for position (i=0,1,..,n)
      std::vector<Vector3d> _vel;  //time series for velocity (i=0,1,..,n)
      std::vector<Vector3d> _acc;  //time series for acceleration (i=0,1,..,n-1 , the current accel needs to be input)
    public:                  
      Body(double, Vector3d, Vector3d); //create mass with inital position and velocity 
      void evolve(const Vector3d accel);    //takes acceleration and find position & velocity for the next timestep
      inline double mass() const{ return _mass; };
      inline Vector3d pos() const{ return _pos.back(); };    
      inline Vector3d vel() const{ return _vel.back(); };
  }; //class Body

  class Simulation{
      std::vector<Body> _body;
      size_t tnow; //number of time steps already evolved
    public:
      void loadRu(std::ifstream&);
      void saveRu(std::ofstream&) const;
      void evolveSystemFor(double t);
  }; //class Simulation

  Vector3d Gravity(const Body& source, const Body& body); //calculate the acceleration due to source on the body

} //namespace nbody

#endif // _6S096_RATIONAL_H
