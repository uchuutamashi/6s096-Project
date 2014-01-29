#include <nbody/Integrator.h>

namespace nbody {

typedef std::vector<Body> tBodyVec;

void Integrator::computeAccel( tBodyVec& bodies ) {
for ( size_t i = 0; i < bodies.size(); ++i) {
	bodies[i].accel() = Vector3d{0.0, 0.0, 0.0};
}
for( size_t i = 0; i < bodies.size(); ++i ) {
  for( size_t j = 0; j < bodies.size(); ++j ) {
    if( i != j ) {
      interactBodies( bodies, i, j, _softFactor );
    }
  }
}
}

void Integrator::doStep( tBodyVec& bodies ) {
	computeAccel( bodies );
	switch ( _integratorType ) {
		case Euler:
		    for( size_t i = 0; i < bodies.size(); ++i ) {
		      auto r = bodies[i].position();
		      auto v = bodies[i].velocity();
		      auto a = bodies[i].accel();

		      v = v + ( a * _timeStep );
		      v = v * _dampingFactor;
		      r = r + v * _timeStep;

		      bodies[i].position() = r;
		      bodies[i].velocity() = v;
		    }			
			break;
		case RK2: //TODO
			break;
		case RK4: //TODO
			break;
		default:
			break;
	}
}

} // namespace nbody