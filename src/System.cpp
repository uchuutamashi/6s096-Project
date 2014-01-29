#include <nbody/constants.h>
#include <nbody/System.h>
#include <nbody/Integrator.h>

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <iomanip>

namespace nbody {

  void System::integrateSystem( ) {
    _integrator.doStep(_body);
  }

  void System::update( ) {
  }

  Vector3d System::getPosition( size_t id ) const{
    if( id >= _body.size() ) {
      throw std::out_of_range("ID is larger than total number of bodies");
    }
    return _body[id].position();
  }

  void System::readState( std::istream &input ) {
    size_t N; // number of bodies
    input >> N;
    if( N > MAX_BODIES_RECOMMENDED ) {
      throw std::runtime_error( "Too many input bodies" );
    }
    _body.resize( N );
    for( size_t i = 0; i < N; ++i ) {
      input >> _body[i];
    }
  }

  void System::writeState( std::ostream &output ) const {
    output << _body.size() << "\n";
    for( size_t i = 0; i < _body.size(); ++i ) {
      output << _body[i] << "\n";
    }
  }

} // namespace nbody
