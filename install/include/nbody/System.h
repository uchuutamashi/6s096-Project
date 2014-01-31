#ifndef _NBODY_SYSTEM_H
#define _NBODY_SYSTEM_H

#include <nbody/Integrator.h>

#include <vector>
#include <iosfwd>
#include <string>

namespace nbody {

typedef std::vector<Body> tBodyVec;

  class System {
    tBodyVec _body;
    double _softFactor = 1e-9f;
    double _dampingFactor = 1.0f;
    Integrator _integrator;
    System() = delete;
    System( const System &sys ) = delete;
    System& operator=( const System &sys ) = delete;
  public:
    System( size_t N ) : _body( N ), _integrator{ N } { initRandomState(); }
    System( tBodyVec bodies) : _body{ bodies }, _integrator{} { }
    System( tBodyVec bodies, integrator_t integratorType) : _body{ bodies }, _integrator{ integratorType } { }
    System( tBodyVec bodies, integrator_t integratorType, double timeStep) : 
    _body{ bodies }, 
    _integrator{ integratorType, timeStep, bodies.size() } {
    }   
    System( std::istream &input );
    System( std::string filename );
    ~System() { }
    inline size_t numBodies() const{ return _body.size(); }
    void integrateSystem( );
    Vector3d getPosition( size_t id ) const;
    void readState( std::istream &input );
    void readState( std::string filename );
    void writeState( std::ostream &output ) const;
    void writeState( std::string filename ) const;
    void initRandomState();
    void update( );
    void setSoftening( double soft ) { _softFactor = soft; }
    void setDamping( double damp ) { _dampingFactor = damp; }
  };

} // namespace nbody

#endif // _NBODY_SYSTEM_H
