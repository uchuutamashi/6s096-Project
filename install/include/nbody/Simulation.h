#ifndef _NBODY_SIMULATION_H
#define _NBODY_SIMULATION_H

#include <nbody/System.h>

#include <iosfwd>
#include <string>

namespace nbody {

  class Simulation {
    System *_system;
    std::string _name;
    Simulation( const Simulation& sim ) = delete;
    Simulation& operator=( const Simulation& sim ) = delete;
    std::string generateName();
  public:
    Simulation() : _system{nullptr}, _name{ generateName() } {}
    Simulation( std::istream &input ) : _system{new System(input)}, _name{ generateName() } {}
    ~Simulation() { delete _system; }
    inline Vector3d getPosition( size_t id ) const{ return _system->getPosition(id); }
    inline Vector3d getColor( size_t id ) const{ return _system->getColor(id); }
    inline double getMass( size_t id ) const{ return _system->getMass(id); }
    inline size_t numBodies() const{ return _system->numBodies(); }
    void evolveSystem( int nSteps );
    void loadRun( std::istream &input );
    void saveRun() const;
  };

} // namespace nbody

#endif // _NBODY_SIMULATION_H
