#ifndef _NBODY_SYSTEM_H
#define _NBODY_SYSTEM_H

#include <nbody/Vector3.h>
#include <nbody/Body.h>

#include <iosfwd>
#include <string>

namespace nbody {

  class System {
    size_t _nBodies;
    Body *_body;
    double _softFactor = 1e-9f;
    double _dampingFactor = 1.0f;
    System() = delete;
    System( const System &sys ) = delete;
    System& operator=( const System &sys ) = delete;
  public:
    System( size_t N ) : _nBodies{N}, _body{ new Body[N] } { initRandomState(); }
    System( size_t N , Body *bodies) : _nBodies{N}, _body{ bodies } { }
    System( std::istream &input ) : _nBodies{}, _body{nullptr} { readState( input ); }
    System( std::string filename ) : _nBodies{}, _body{nullptr} { readState( filename ); }
    ~System() { delete [] _body; }
    inline size_t numBodies() const{ return _nBodies; }
    void interactBodies( size_t i, size_t j, double softFactor, Vector3d &acc ) const;
    void computeGravitation();
    void integrateSystem( double dt );
    Vector3d getPosition( size_t id ) const;
    void readState( std::istream &input );
    void readState( std::string filename );
    void writeState( std::ostream &output ) const;
    void writeState( std::string filename ) const;
    void initRandomState();
    void update( double dt );
    void setSoftening( double soft ) { _softFactor = soft; }
    void setDamping( double damp ) { _dampingFactor = damp; }
  };

} // namespace nbody

#endif // _NBODY_SYSTEM_H
