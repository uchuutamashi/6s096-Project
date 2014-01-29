#include <nbody/System.h>
#include <gtest/gtest.h>

#define DELTA 0.0001

using namespace nbody;
typedef std::vector<Body> tBodyVec;

TEST( physicsTest, constAccelx ) {
  tBodyVec testobj;
  testobj.resize(1);
  testobj[0].position() = Vector3d{0,0,0};
  testobj[0].velocity() = Vector3d{0,0,0};
  testobj[0].accel()    = Vector3d{1,0,0};
  
  System *testsys = new System{testobj, Euler, DELTA};

  for(int i=1; i<100; i++){
    testsys->integrateSystem( );
    EXPECT_LT(testsys->getPosition(0).x()-0.5*i*DELTA*i*DELTA, DELTA); //tolerate 1st order err for now 
  };
  delete testsys;
}

TEST( physicsTest, constAccely ) {
  tBodyVec testobj;
  testobj.resize(1);
  testobj[0].position() = Vector3d{0,0,0};
  testobj[0].velocity() = Vector3d{0,0,0};
  testobj[0].accel()    = Vector3d{0,1,0};
  
  System *testsys = new System{testobj, Euler, DELTA};

  for(int i=1; i<100; i++){
    testsys->integrateSystem( );
    EXPECT_LT(testsys->getPosition(0).y()-0.5*i*DELTA*i*DELTA, DELTA); //tolerate 1st order err for now 
  };
  delete testsys;
}

TEST( physicsTest, constAccelz ) {
  tBodyVec testobj;
  testobj.resize(1);
  testobj[0].position() = Vector3d{0,0,0};
  testobj[0].velocity() = Vector3d{0,0,0};
  testobj[0].accel()    = Vector3d{0,0,1};
  
  System *testsys = new System{testobj, Euler, DELTA};

  for(int i=1; i<100; i++){
    testsys->integrateSystem( );
    EXPECT_LT(testsys->getPosition(0).z()-0.5*i*DELTA*i*DELTA, DELTA); //tolerate 1st order err for now 
  };
  delete testsys;
}
