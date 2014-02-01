#include <nbody/System.h>
#include <gtest/gtest.h>

#define DELTA 1e-4
#define N 2226 // the 2226 is not important; >= 2226 is what matters here
#define TOL 0.02 // tolerance
using namespace nbody;
typedef std::vector<Body> tBodyVec;

TEST( physicsTest, constVelocity ) {
  tBodyVec testobj;
  testobj.resize(1);
  testobj[0].position() = Vector3d{0,0,0};
  testobj[0].velocity() = Vector3d{1,2,3};
  testobj[0].accel()    = Vector3d{0,0,0}; // should not matter, covered by next test
  
  System *testsysEuler = new System{testobj, Euler, DELTA};
  System *testsysRK2 = new System{testobj, RK2, DELTA};
  System *testsysRK4 = new System{testobj, RK4, DELTA};

  for( int i = 1; i < N; i++ ) {
    testsysEuler->integrateSystem();
    testsysRK2->integrateSystem();
    testsysRK4->integrateSystem();
    ASSERT_FLOAT_EQ( testsysEuler->getPosition(0).x(), i*DELTA );
    ASSERT_FLOAT_EQ( testsysEuler->getPosition(0).y(), 2*i*DELTA );
    ASSERT_FLOAT_EQ( testsysEuler->getPosition(0).z(), 3*i*DELTA ); 
    ASSERT_FLOAT_EQ( testsysRK2->getPosition(0).x(), i*DELTA );
    ASSERT_FLOAT_EQ( testsysRK2->getPosition(0).y(), 2*i*DELTA );
    ASSERT_FLOAT_EQ( testsysRK2->getPosition(0).z(), 3*i*DELTA ); 
    ASSERT_FLOAT_EQ( testsysRK4->getPosition(0).x(), i*DELTA );
    ASSERT_FLOAT_EQ( testsysRK4->getPosition(0).y(), 2*i*DELTA );
    ASSERT_FLOAT_EQ( testsysRK4->getPosition(0).z(), 3*i*DELTA ); // float since double equality is too strict
  }
  delete testsysEuler;
  delete testsysRK2;
  delete testsysRK4;
}

TEST( physicsTest, ensureConstVelocity ) {
  tBodyVec testobj;
  testobj.resize(1);
  testobj[0].position() = Vector3d{0,0,0};
  testobj[0].velocity() = Vector3d{1,2,3};
  testobj[0].accel()    = Vector3d{-1,-2,5}; // these values should get set to 0 by the integrator
  
  System *testsysEuler = new System{testobj, Euler, DELTA};
  System *testsysRK2 = new System{testobj, RK2, DELTA};
  System *testsysRK4 = new System{testobj, RK4, DELTA};

  for( int i = 1; i < N; i++ ) {
    testsysEuler->integrateSystem();
    testsysRK2->integrateSystem();
    testsysRK4->integrateSystem();
    ASSERT_FLOAT_EQ( testsysEuler->getPosition(0).x(), i*DELTA );
    ASSERT_FLOAT_EQ( testsysEuler->getPosition(0).y(), 2*i*DELTA );
    ASSERT_FLOAT_EQ( testsysEuler->getPosition(0).z(), 3*i*DELTA ); 
    ASSERT_FLOAT_EQ( testsysRK2->getPosition(0).x(), i*DELTA );
    ASSERT_FLOAT_EQ( testsysRK2->getPosition(0).y(), 2*i*DELTA );
    ASSERT_FLOAT_EQ( testsysRK2->getPosition(0).z(), 3*i*DELTA ); 
    ASSERT_FLOAT_EQ( testsysRK4->getPosition(0).x(), i*DELTA );
    ASSERT_FLOAT_EQ( testsysRK4->getPosition(0).y(), 2*i*DELTA );
    ASSERT_FLOAT_EQ( testsysRK4->getPosition(0).z(), 3*i*DELTA ); // float since double equality is too strict
  }
  delete testsysEuler;
  delete testsysRK2;
  delete testsysRK4; 
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
  // this test can be quite finicky, since when the two get very close, acceleration shoots up to infinity
  // however, its advantage is the simple trajectories
  System *testsysEuler = new System{testobj, Euler, DELTA};
  System *testsysRK2 = new System{testobj, RK2, DELTA};
  System *testsysRK4 = new System{testobj, RK4, DELTA};
  bool bMetAtComEuler{false};
  bool bMetAtComRK2{false};
  bool bMetAtComRK4{false};

  auto com = Vector3d{1.0, 0.0, 0.0}; // center of mass

  for( int i = 1; i < N; i++ ) {
    testsysEuler->integrateSystem();
    if ( ( testsysEuler->getPosition(0) - com ).norm() < TOL ) {
      ASSERT_TRUE( ( testsysEuler->getPosition(0) - testsysEuler->getPosition(1) ).norm() < 2*TOL ); 
      bMetAtComEuler = true;
      break;
    }
  }

  for( int i = 1; i < N; i++ ) {
    testsysRK2->integrateSystem();
    if ( ( testsysRK2->getPosition(0) - com ).norm() < TOL ) {
      ASSERT_TRUE( ( testsysRK2->getPosition(0) - testsysRK2->getPosition(1) ).norm() < 2*TOL ); 
      bMetAtComRK2 = true;
      break;
    }
  }

  for( int i = 1; i < N; i++ ) {
    testsysRK4->integrateSystem();
    if ( ( testsysRK4->getPosition(0) - com ).norm() < TOL ) {
      ASSERT_TRUE( ( testsysRK4->getPosition(0) - testsysRK4->getPosition(1) ).norm() < 2*TOL ); 
      bMetAtComRK4 = true;
      break;
    }
  }

  ASSERT_TRUE( bMetAtComEuler );
  ASSERT_TRUE( bMetAtComRK2 );
  ASSERT_TRUE( bMetAtComRK4 ); 
  delete testsysEuler;
  delete testsysRK2;
  delete testsysRK4;
}