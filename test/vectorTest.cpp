#include <nbody/Vector3.h>
#include <gtest/gtest.h>

TEST( vectorTest, dotProduct ) {
  auto v = Vector3f{ 1, 0, 0 };
  auto v_perp = Vector3f{ 0, 1, -1 };

  ASSERT_FLOAT_EQ( dot( v, v_perp ) , 0.0f );
}

TEST( vectorTest, crossProduct ) {
  auto v = Vector3f{ 1, 2, 3 };
  auto i = Vector3f{ 1, 0, 0 };
  auto j = Vector3f{ 0, 1, 0 };
  auto k = Vector3f{ 0, 0, 1 };
  ASSERT_FLOAT_EQ( cross( v, v ).norm(), 0.0f );
  ASSERT_FLOAT_EQ( ( cross( i, j ) - k ).norm(), 0.0f );
  ASSERT_FLOAT_EQ( ( cross( j, k ) - i ).norm(), 0.0f );
  ASSERT_FLOAT_EQ( ( cross( k, i ) - j ).norm(), 0.0f );
}

TEST( vectorTest, normsq ) {
  auto v1 = Vector3d{2.0, 1.0, 6.0};
  auto v2 = Vector3f{1.f, 0.f, -2.f};
  auto v3 = Vector3f{4.f, 1.f, -6.f};
  ASSERT_DOUBLE_EQ( v1.normsq(), 41.0);
  ASSERT_FLOAT_EQ( v2.normsq(), 5.f);
  ASSERT_FLOAT_EQ( v3.normsq(), 53.f);
}

TEST( vectorTest, norm ) {
  auto v1 = Vector3f{3.f, 4.f, 0.f};
  auto v2 = Vector3d{1.0, 12.0, 12.0};
  ASSERT_FLOAT_EQ( v1.norm(), 5.f);
  ASSERT_DOUBLE_EQ( v2.norm(), 17.0);
}

TEST( vectorTest, unaryMultiply ) {
  auto v1 = Vector3f{1.f, 2.f, 3.f};
  v1 *= 3.f;
  ASSERT_FLOAT_EQ( v1.x(), 3.f);
  ASSERT_FLOAT_EQ( v1.y(), 6.f);
  ASSERT_FLOAT_EQ( v1.z(), 9.f);   
}

TEST( vectorTest, unaryAdd ) {
  auto v1 = Vector3f{1.f, 2.f, 3.f};
  auto v2 = Vector3f{-1.f, -2.f, -3.f};
  v1 += v2;
  ASSERT_FLOAT_EQ( v1.x(), 0.f);
  ASSERT_FLOAT_EQ( v1.y(), 0.f);
  ASSERT_FLOAT_EQ( v1.z(), 0.f);   
}

TEST( vectorTest, add ) {
  auto v1 = Vector3f{1.f, 2.f, 3.f};
  auto v2 = Vector3f{-1.f, -2.f, -3.f};
  auto v3 = v1 + v2;
  ASSERT_FLOAT_EQ( v3.x(), 0.f);
  ASSERT_FLOAT_EQ( v3.y(), 0.f);
  ASSERT_FLOAT_EQ( v3.z(), 0.f);  
}

TEST( vectorTest, subtract ) {
  auto v1 = Vector3f{1.f, 2.f, 3.f};
  auto v2 = Vector3f{-1.f, -2.f, -3.f};
  auto v3 = v1 - v2;
  ASSERT_FLOAT_EQ( v3.x(), 2.f);
  ASSERT_FLOAT_EQ( v3.y(), 4.f);
  ASSERT_FLOAT_EQ( v3.z(), 6.f);  
}

TEST( vectorTest, multiply ) {
  auto v1 = Vector3f{1.f, 2.f, 3.f};
  auto v2 = 3.f * v1;
  auto v3 = v1 * 5.f;
  ASSERT_FLOAT_EQ( v2.x(), 3.f);
  ASSERT_FLOAT_EQ( v2.y(), 6.f);
  ASSERT_FLOAT_EQ( v2.z(), 9.f);
  ASSERT_FLOAT_EQ( v3.x(), 5.f);
  ASSERT_FLOAT_EQ( v3.y(), 10.f);
  ASSERT_FLOAT_EQ( v3.z(), 15.f);     
}

TEST( vectorTest, divide) {
  auto v1 = Vector3f{9.f, 18.f, 27.f};
  auto v2 = v1 / 3.f;
  ASSERT_FLOAT_EQ( v2.x(), 3.f);
  ASSERT_FLOAT_EQ( v2.y(), 6.f);
  ASSERT_FLOAT_EQ( v2.z(), 9.f);  
}