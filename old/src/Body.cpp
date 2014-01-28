#include <nbody/nbody.h>

namespace nbody{
  Body::Body(double Mass, Vector3d pos_i, Vector3d vel_i) : 
      _mass {Mass}, _pos{{pos_i}}, _vel{{vel_i}}, _acc{} { }

  void Body::evolve(const Vector3d accel){
    //replace with integrate
    _acc.push_back(accel); //a[n-1]=a    
    _pos.push_back(Integrate(_pos,_vel)); //x[n]=x[n-1]+v[n-1]*dt
    _vel.push_back(Integrate(_vel,_acc));  //v[n]=v[n-1]+a[n-1]*dt    
  }
} //namespace nbody
