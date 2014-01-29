#ifndef _NBODY_INTEGRATOR_H
#define _NBODY_INTEGRATOR_H

#include <nbody/Body.h>
#include <nbody/constants.h>

#include <vector>


namespace nbody {

typedef std::vector<Body> tBodyVec;
enum integrator_t {Euler, RK2, RK4}; // integrator type

class Integrator {
	integrator_t _integratorType;
	double _timeStep; 
    double _softFactor = 1e-9f;
    double _dampingFactor = 1.0f;
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
	Integrator(): _integratorType{Euler}, _timeStep{1e-6} { }
	Integrator( integrator_t integratorType ) : _integratorType{integratorType}, _timeStep{} { }
	Integrator( integrator_t integratorType, double time ) : _integratorType{integratorType}, _timeStep{time} { }
	inline double timeStep() const { return _timeStep; }
	inline double& timeStep() { return _timeStep; }
	inline integrator_t integrator() const { return _integratorType; }
	inline integrator_t& integrator() { return _integratorType; }
	void doStep( tBodyVec& bodies ); // the actual integration
	~Integrator() { }
	};

} // namespace nbody

#endif // _NBODY_INTEGRATOR_H

