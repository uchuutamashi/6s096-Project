#include <nbody/nbody.h>

namespace nbody{
  Body::Body(double Mass, Vector3d pos_i, Vector3d vel_i) : 
      _mass {Mass}, _pos{{pos_i}}, _vel{{vel_i}}, _acc{} { }

  void Body::evolve(const Vector3d accel){
    //replace with integrate
    _acc.push_back(accel); //a[n-1]=a
    _vel.push_back(_vel.back()+_acc.back()*DELTA);  //v[n]=v[n-1]+a[n-1]*dt
    _pos.push_back(_pos.back()+_vel.back()*DELTA+0.5*_acc.back()*DELTA*DELTA); //x[n]=x[n-1]+v[n-1]*dt+0.5*a[n-1]*dt^2
    
    
  }
} //namespace nbody
