#ifndef _NBODY_BODY_H
#define _NBODY_BODY_H

#include <nbody/Vector3.h>

#include <iosfwd>

namespace nbody {

  class Body {
    Vector3d _position;
    Vector3d _velocity;
    Vector3d _force;
    double _mass;
  public:
    Body() : _position{}, _velocity{}, _force{}, _mass{} {}
    inline Vector3d position() const { return _position; }
    inline Vector3d& position() { return _position; }
    inline Vector3d velocity() const { return _velocity; }
    inline Vector3d& velocity() { return _velocity; }
    inline Vector3d force() const { return _force; } //acceleration
    inline Vector3d& force() { return _force; }      //acceleration
    inline double mass() const { return _mass; }
    friend std::istream& operator>>( std::istream &is, Body &body );
    friend std::ostream& operator<<( std::ostream &os, const Body &body );
  };

} // namespace nbody

#endif // _NBODY_BODY_H
