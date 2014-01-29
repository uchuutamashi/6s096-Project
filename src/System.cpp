#include <nbody/constants.h>
#include <nbody/System.h>
#include <nbody/Vector3.h>

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <iomanip>

namespace nbody {

  inline void System::interactBodies( size_t i, size_t j, double softFactor, Vector3d &acc ) const {
    Vector3d r = _body[j].position() - _body[i].position();
    double distance = r.norm() + softFactor;
    double invDist = 1.0 / distance;
    double invDistCubed = cube( invDist );
    acc = acc + NEWTON_G * _body[j].mass() * invDistCubed * r;
  }

  void System::computeGravitation() {
    for( size_t i = 0; i < _nBodies; ++i ) {
      Vector3d acc{ 0.0, 0.0, 0.0 };
      for( size_t j = 0; j < _nBodies; ++j ) {
        if( i != j ) {
          interactBodies( i, j, _softFactor, acc );
        }
      }
      _body[i].force() = acc;
    }
  }

  void System::integrateSystem( double dt ) {
    Vector3d r, v, a;
    for( size_t i = 0; i < _nBodies; ++i ) {
      r = _body[i].position();
      v = _body[i].velocity();
      a = _body[i].force();

      v = v + ( a * dt );
      v = v * _dampingFactor;
      r = r + v * dt;

      _body[i].position() = r;
      _body[i].velocity() = v;
    }
  }

  void System::update( double dt ) {
    computeGravitation();
    integrateSystem( dt );
  }

  Vector3d System::getPosition( size_t id ) const{
    if( id >= _nBodies ) {
      throw std::out_of_range("ID is larger than total number of bodies");
    }
    return _body[id].position();
  }

  void System::readState( std::istream &input ) {
    input >> _nBodies;
    if( _nBodies > MAX_BODIES_RECOMMENDED ) {
      throw std::runtime_error( "Too many input bodies" );
    }
    _body = new Body[_nBodies];
    for( size_t i = 0; i < _nBodies; ++i ) {
      input >> _body[i];
    }
  }

  void System::writeState( std::ostream &output ) const {
    output << _nBodies << "\n";
    for( size_t i = 0; i < _nBodies; ++i ) {
      output << _body[i] << "\n";
    }
  }

} // namespace nbody
