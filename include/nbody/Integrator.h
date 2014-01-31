#ifndef _NBODY_INTEGRATOR_H
#define _NBODY_INTEGRATOR_H

#include <nbody/Body.h>
#include <nbody/Constants.h>

#include <vector>

namespace nbody {

typedef std::vector<Body> tBodyVec;
enum integrator_t {Euler, RK2, RK4}; // integrator type

class Integrator {
	integrator_t _integratorType;
	double _timeStep; 
    double _softFactor = 1e-9f;
    double _dampingFactor = 1.0f;
    std::vector<Vector3d> _rState; // the position at beginning of an iteration
    std::vector<Vector3d> _vState; // the velocity at beginning of an iteration
    std::vector<Vector3d> _aState; // the acceleration computed using the integration scheme
	Integrator( const Integrator &integrator ) = delete;
	Integrator& operator=( const Integrator &integrator ) = delete;
    inline void interactBodies( tBodyVec& bodies, size_t i, size_t j, double softFactor ) const
    {
	    Vector3d r = bodies[j].position() - bodies[i].position();
	    double distance = r.norm() + softFactor;
	    double invDist = 1.0 / distance;
	    double invDistCubed = cube( invDist );
	    bodies[i].accel() = bodies[i].accel() + NEWTON_G * bodies[j].mass() * invDistCubed * r;   	
    }
    void computeAccel( tBodyVec& bodies );
public:
	Integrator();
	Integrator( size_t numBodies );
	Integrator( integrator_t integratorType, double time, size_t numBodies );
	inline double timeStep() const { return _timeStep; }
	inline double& timeStep() { return _timeStep; }
	inline integrator_t integrator() const { return _integratorType; }
	inline integrator_t& integrator() { return _integratorType; }
	void resizeState( size_t numBodies );
	void saveState ( tBodyVec& bodies ); // saves current body params into state member variables
	void restoreState ( tBodyVec& bodies ); // restore the state to bodies (excluding acc)
	void restoreAccState ( tBodyVec& bodies ); // restores the acceleration
	void incAccState ( tBodyVec& bodies, double weight ); // used over and over in Runge-Kutta
	void doEulerStep ( tBodyVec& bodies, double time); // euler step with timeStep duration
	void doStep( tBodyVec& bodies ); // the actual integration
	~Integrator() { }
	};

} // namespace nbody

#endif // _NBODY_INTEGRATOR_H

