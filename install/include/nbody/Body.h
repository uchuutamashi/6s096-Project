#ifndef _NBODY_BODY_H
#define _NBODY_BODY_H

#include <nbody/Vector3.h>

#include <iosfwd>
#include <cstdlib>
#include <time.h>

namespace nbody {

  class Body {
    Vector3d _position;
    Vector3d _velocity;
    Vector3d _accel;    
    double _mass;
    Vector3d _color;
  public:
    Body() : _position{}, _velocity{}, _accel{},  _mass{}, _color{Vector3d{ (double) (rand()%255)/255, (double) (rand()%255)/255, (double) (rand()%255)/255}} {}
    inline Vector3d position() const { return _position; }
    inline Vector3d& position() { return _position; }
    inline Vector3d velocity() const { return _velocity; }
    inline Vector3d& velocity() { return _velocity; }
    inline Vector3d accel() const { return _accel; } 
    inline Vector3d& accel() { return _accel; }    
    inline Vector3d color() const { return _color; } 
    inline Vector3d& color() { return _color; }  
    inline double mass() const { return _mass; }
    inline double& mass() { return _mass; }
    friend std::istream& operator>>( std::istream &is, Body &body );
    friend std::ostream& operator<<( std::ostream &os, const Body &body );
  };

} // namespace nbody

#endif // _NBODY_BODY_H
