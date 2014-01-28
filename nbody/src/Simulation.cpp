#include <nbody/nbody.h>

namespace nbody{
  Simulation::Simulation() : _body{}, _tnow{0} {}
  Simulation::Simulation(std::vector<Body> Bodies) : _body{Bodies}, _tnow{0} {}

  void Simulation::loadRu(std::istream&){
    //create an array of bodies and initialize them with corresponding mass, pos_i, vel_i
    //in the file and set _body to the new array

  }

  void Simulation::saveRu(std::ostream&) const{
    //do the reverse of loadRu, save the latest positions and velocities along with masses
  
  }

  void Simulation::evolveSystemFor(double t){
    size_t step = t / DELTA; //convert t into units of DELTA

    while(step>0){
      //for each body, sum all the accelerations (use Gravity(Body&, Body&))
      //and then use Body::evolve to calculate the new pos and vel
  
      step--;
      _tnow++;
    }
  }
}

