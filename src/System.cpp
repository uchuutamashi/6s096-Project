#include <nbody/Constants.h>
#include <nbody/System.h>

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <iomanip>

namespace nbody {

  System::System( std::istream &input ) : _body{}, _integrator{} {
    readState( input );
    _integrator.resizeState( _body.size() );
  }

  // TODO: reading from filename string
  /*System::System( std::string filename ) : _body{}, _integrator{} {
    readState( filename );
    _integrator.resizeState( _body.size() );
  } */

  void System::integrateSystem( ) {
    _integrator.doStep(_body);
  }

  void System::update( ) {
    integrateSystem( );
  }

  Vector3d System::getPosition( size_t id ) const{
    if( id >= _body.size() ) {
      throw std::out_of_range("ID is larger than total number of bodies");
    }
    return _body[id].position();
  }

  double System::getMass( size_t id ) const{
    if( id >= _body.size() ) {
      throw std::out_of_range("ID is larger than total number of bodies");
    }
    return _body[id].mass();
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
    input.ignore(256, '#'); // 256 is just a dummy value
    int integrator;
    input >> integrator;
    _integrator.integrator_type() = ( integrator_t ) integrator;
    input >> _integrator.timeStep();
  }

  void System::readState( std::string filename ) {
    std::filebuf fb;
    if ( fb.open ( filename,std::ios::in ) ) {
      std::istream is( &fb );
      readState( is );
    }
  }

  void System::writeState( std::ostream &output ) const {
    output << _body.size() << "\n";
    for( size_t i = 0; i < _body.size(); ++i ) {
      output << _body[i] << "\n";
    }
    // write out the integrator type, time step after "# ""
    output << "# " << _integrator.integrator_type()  << " " << _integrator.timeStep() << "\n";
  }

  void System::writeState( std::string filename ) const {
    std::filebuf fb;
    if ( fb.open ( filename,std::ios::out ) ) {
      std::ostream os( &fb );
      writeState( os );
    }    
  }

} // namespace nbody
