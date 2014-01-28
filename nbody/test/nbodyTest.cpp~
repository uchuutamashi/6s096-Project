#include <nbody/nbody.h>
#include <gtest/gtest.h>

using namespace nbody;

TEST(BodyTests, constantAcceleration){
  Body testBody = Body{1, Vector3d{0,0,0}, Vector3d{0,0,0}};
  const Vector3d accel = Vector3d{1,0,0};
  for(int i=1; i<10; i++){
    testBody.evolve(accel);
    EXPECT_LT(testBody.pos().x()-0.5*i*DELTA*i*DELTA, DELTA*DELTA);
  };
}

/*
TEST( rationalTests, equality ) {
  ASSERT_TRUE( Rational( 1, 2 ) == Rational( 2, 4 ) );
  ASSERT_TRUE( Rational( -1, 3 ) == Rational( 1, -3 ) );
  ASSERT_TRUE( Rational( -4, 1 ) == Rational( 8, -2 ) );
  ASSERT_TRUE( Rational( -0, 1 ) == Rational( 0, 5 ) );
}

TEST( rationalTests, sum ) {
  ASSERT_EQ( Rational( 1, 2 ) + Rational( 1, 2 ), 1 );
  ASSERT_EQ( Rational( 3, 4 ) + Rational( 1, 4 ), 1 );
  ASSERT_EQ( Rational( 3, 4 ) + Rational( 3, -4 ), 0 );
}

TEST( rationalTests, prod ) {
  auto a = Rational( 5, 6 );
  auto b = Rational( 3, 7 );
  ASSERT_EQ( a * b, Rational( 5, 14 ) );
}
*/
