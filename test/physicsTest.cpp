#include <nbody/System.h>
#include <gtest/gtest.h>

#define DELTA 0.0001

using namespace nbody;

TEST( physicsTest, constAccel ) {
  Body *testobj = new Body;
  testobj->position() = Vector3f{0,0,0};
  testobj->velocity() = Vector3f{0,0,0};
  testobj->force()    = Vector3f{1,0,0};
  
  System *testsys = new System{1,testobj};

  for(int i=1; i<100; i++){
    testsys->integrateSystem(DELTA);
    EXPECT_LT(testsys->getPosition(0).x()-0.5*i*DELTA*i*DELTA, DELTA); //tolerate 1st order err for now 
  };
}
