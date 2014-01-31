#include <nbody/System.h>
#include <gtest/gtest.h>

#define DELTA 1
#define N 5
using namespace nbody;
typedef std::vector<Body> tBodyVec;

TEST( physicsTest, constVelocity ) {
  tBodyVec testobj;
  testobj.resize(1);
  testobj[0].position() = Vector3d{0,0,0};
  testobj[0].velocity() = Vector3d{1,2,3};
  testobj[0].accel()    = Vector3d{0,0,0};
  
  System *testsys = new System{testobj, Euler, DELTA};

  for( int i = 1; i < N; i++ ){
    testsys->integrateSystem();
    ASSERT_DOUBLE_EQ( testsys->getPosition(0).x()-i*DELTA, 0.0 );
    ASSERT_DOUBLE_EQ( testsys->getPosition(0).y()-2*i*DELTA, 0.0 );
    ASSERT_DOUBLE_EQ( testsys->getPosition(0).z()-3*i*DELTA, 0.0 );
  };
  delete testsys;
}

