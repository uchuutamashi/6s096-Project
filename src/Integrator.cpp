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

Integrator::Integrator() :
	_integratorType{Euler},
	_timeStep{1e-6},
	_rState(),
	_vState(),
	_aState() {
}

Integrator::Integrator( size_t numBodies ) :  
	_integratorType{Euler}, 
	_timeStep{1e-6}, 
	_rState( numBodies ), 
	_vState( numBodies ),
	_aState( numBodies ) { 
}

Integrator::Integrator( integrator_t integratorType, double time, size_t numBodies ) : 
	_integratorType{integratorType}, 
	_timeStep{time},
	_rState( numBodies ),
	_vState( numBodies ),
	_aState( numBodies ) { 
}

void Integrator::resizeState( size_t numBodies ) {
	_rState.resize( numBodies );
	_vState.resize( numBodies );
	_aState.resize( numBodies );
}

void Integrator::saveState( tBodyVec& bodies ) {
	for( size_t i = 0; i < bodies.size(); ++i ) {
		_rState[i] = bodies[i].position();
		_vState[i] = bodies[i].velocity();
		_aState[i] = Vector3d{0.0, 0.0, 0.0};
	}	
}

void Integrator::restoreState( tBodyVec& bodies ) {
	for( size_t i = 0; i < bodies.size(); ++i ) {
		bodies[i].velocity() = _vState[i];
		bodies[i].position() = _rState[i];
	}	
}

void Integrator::restoreAccState( tBodyVec& bodies ) {
	for( size_t i = 0; i < bodies.size(); ++i ) {
		bodies[i].accel() = _aState[i];
	}
}

void Integrator::incAccState ( tBodyVec& bodies, double weight ) {
	for( size_t i = 0; i < bodies.size(); ++i) {
		_aState[i] += weight * bodies[i].accel();
	}
}

void Integrator::doEulerStep( tBodyVec& bodies, double time ) { // euler step with time duration
    for( size_t i = 0; i < bodies.size(); ++i ) {
      auto r = bodies[i].position();
      auto v = bodies[i].velocity();
      auto a = bodies[i].accel();

      v += ( a * time );
      v *= _dampingFactor;
      r += v * time;

      bodies[i].position() = r;
      bodies[i].velocity() = v;
    }	
} 

void Integrator::doRK2Step( tBodyVec& bodies ) { 
	saveState( bodies );
	double halfStep = 0.5*_timeStep;
	doEulerStep( bodies, halfStep);
    computeAccel( bodies );
    incAccState( bodies, 1.0 );
    restoreState( bodies );
    restoreAccState( bodies );
    doEulerStep( bodies, _timeStep);
}

void Integrator::doRK4Step( tBodyVec& bodies ) {
	saveState( bodies );
	incAccState( bodies, 1.0/6.0);
	double halfStep = 0.5*_timeStep;
	doEulerStep( bodies, halfStep);
	computeAccel( bodies );
	incAccState( bodies, 1.0/3.0);
	restoreState( bodies );
	doEulerStep( bodies, halfStep);
	computeAccel( bodies );
	incAccState( bodies, 1.0/3.0);
	restoreState( bodies );
	doEulerStep( bodies, _timeStep);
	computeAccel( bodies );
	incAccState( bodies, 1.0/6.0);
	restoreState( bodies );
	restoreAccState( bodies );
	doEulerStep( bodies, _timeStep);
}

void Integrator::doStep( tBodyVec& bodies ) {
	computeAccel( bodies );
	switch ( _integratorType ) {
		case Euler: {
			doEulerStep( bodies, _timeStep );			
			return;
		}
		case RK2: {
			doRK2Step( bodies );
			return;
		}
		case RK4: {
			doRK4Step( bodies ); 
			return;
		}
		default:
			return;
	}
}

} // namespace nbody