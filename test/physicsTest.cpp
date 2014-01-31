#include <nbody/System.h>
#include <gtest/gtest.h>

#define DELTA 1e-4
#define N 2226
#define TOL 0.02 // tolerance
using namespace nbody;
typedef std::vector<Body> tBodyVec;

TEST( physicsTest, constVelocity ) {
  tBodyVec testobj;
  testobj.resize(1);
  testobj[0].position() = Vector3d{0,0,0};
  testobj[0].velocity() = Vector3d{1,2,3};
  testobj[0].accel()    = Vector3d{0,0,0}; // should not matter, covered by next test
  
  System *testsys = new System{testobj, Euler, DELTA};

  for( int i = 1; i < N; i++ ) {
    testsys->integrateSystem();
    ASSERT_FLOAT_EQ( testsys->getPosition(0).x(), i*DELTA );
    ASSERT_FLOAT_EQ( testsys->getPosition(0).y(), 2*i*DELTA );
    ASSERT_FLOAT_EQ( testsys->getPosition(0).z(), 3*i*DELTA ); // float since double equality is too strict
  }
  delete testsys;
}

TEST( physicsTest, ensureConstVelocity ) {
  tBodyVec testobj;
  testobj.resize(1);
  testobj[0].position() = Vector3d{0,0,0};
  testobj[0].velocity() = Vector3d{1,2,3};
  testobj[0].accel()    = Vector3d{-1,-2,5}; // these values should get set to 0 by the integrator
  
  System *testsys = new System{testobj, Euler, DELTA};

  for( int i = 1; i < N; i++ ) {
    testsys->integrateSystem();
    ASSERT_FLOAT_EQ( testsys->getPosition(0).x(), i*DELTA );
    ASSERT_FLOAT_EQ( testsys->getPosition(0).y(), 2*i*DELTA );
    ASSERT_FLOAT_EQ( testsys->getPosition(0).z(), 3*i*DELTA );
  }
  delete testsys;  
}

TEST( physicsTest, simpleTwoBodySystem ) {
  tBodyVec testobj;
  testobj.resize(2);
  testobj[0].position() = Vector3d{0,0,0};
  testobj[0].velocity() = Vector3d{0,0,0};
  testobj[0].mass() = 100.0;
  testobj[1].mass() = 100.0;
  testobj[1].position() = Vector3d{2, 0, 0};
  testobj[1].velocity() = Vector3d{0, 0, 0};

  // the two bodies should move on a straight line and collide at their center of mass
  // this test is quite finicky, since when the two get very close, acceleration shoots up to infinity
  // however, its advantage is the simple trajectories
  System *testsys = new System{testobj, Euler, DELTA};  
  auto com = Vector3d{1.0, 0.0, 0.0}; // center of mass
  for( int i = 1; i < N; i++ ) {
    testsys->integrateSystem();
    if ( ( testsys->getPosition(0) - com ).norm() < TOL ) {
      ASSERT_TRUE( ( testsys->getPosition(0) - testsys->getPosition(1) ).norm() < 2*TOL ); 
      delete testsys;
      return;
    }
  }
  bool bMetAtCom{false};
  ASSERT_TRUE( bMetAtCom );
  delete testsys;
}