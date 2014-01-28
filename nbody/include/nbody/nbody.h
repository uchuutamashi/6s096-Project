#ifndef _6S096_NBODY_H
#define _6S096_NBODY_H

#define DELTA 0.01 //length of time step for integration

#include <iostream>
#include <vector>
#include <Vector3.h>
#include <stdexcept>

typedef std::vector<Vector3d> TimeSeries;

namespace nbody{
  class Body{
      double _mass;
      TimeSeries _pos;  //time series for position (i=0,1,..,n)
      TimeSeries _vel;  //time series for velocity (i=0,1,..,n)
      TimeSeries _acc;  //time series for acceleration (i=0,1,..,n-1 , the current accel needs to be input)
    public:                  
      Body(double, Vector3d, Vector3d); //create mass with inital position and velocity 
      void evolve(const Vector3d accel);    //takes acceleration and find position & velocity for the next timestep
      inline double mass() const{ return _mass; };
      inline Vector3d pos() const{ return _pos.back(); };
      inline Vector3d vel() const{ return _vel.back(); };      
      inline Vector3d pos(size_t t) const{ 
        if(t<_pos.size()){
          return _pos[t];
        }else{
          throw std::out_of_range ("Out of range when requesting position.");
        } 
      };

      inline Vector3d vel(size_t t) const{ 
        if(t<_pos.size()){
          return _pos[t];
        }else{
          throw std::out_of_range ("Out of range when requesting position.");
        }
      };
  }; //class Body

  class Simulation{
      std::vector<Body> _body;
      size_t _tnow; //number of time steps already evolved
    public:
      Simulation();
      Simulation(std::vector<Body>);
      void loadRun(std::istream&);
      void saveRun(std::ostream&) const;
      void evolveSystemFor(const double t);
  }; //class Simulation

  //math functions
  inline Vector3d Gravity(const Body& source, const Body& body){ //calculate the acceleration due to source on the body
    Vector3d r = source.pos()-body.pos();
    return (source.mass()/r.normsq())*r;
  }

  inline Vector3d Integrate(const TimeSeries& init_val, const TimeSeries& rate){ //calculate the next value from old value and rate of change
    return init_val.back() + rate.back() * DELTA;
  }
} //namespace nbody

#endif // _6S096_NBODY_H
