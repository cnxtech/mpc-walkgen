////////////////////////////////////////////////////////////////////////////////
///
///\file test-zebulon-com-constraint.cpp
///\brief Test the Zebulon CoM constraint function
///\author Lafaye Jory
///\date 20/07/13
///
////////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "../src/model/zebulon_base_model.h"
#include "../src/model/lip_model.h"
#include "../src/function/zebulon_com_constraint.h"

class ZebulonComConstraintTest: public ::testing::Test{};



TEST_F(ZebulonComConstraintTest, functionValue)
{
  using namespace MPCWalkgen;

  LIPModel m1;
  BaseModel m2;

  Scalar copLimitMin = 0.1;
  Scalar copLimitMax = 0.15;

  std::vector<Vector2> p(8);
  p[7] = Vector2(copLimitMax, 0.0);
  p[6] = Vector2(copLimitMin, -copLimitMin);
  p[5] = Vector2(0.0, -copLimitMax);
  p[4] = Vector2(-copLimitMin, -copLimitMin);
  p[3] = Vector2(-copLimitMax, 0.0);
  p[2] = Vector2(-copLimitMin, copLimitMin);
  p[1] = Vector2(0.0, copLimitMax);
  p[0] = Vector2(copLimitMin, copLimitMin);
  ConvexPolygon h(p);
  m2.setComSupportConvexPolygon(h);

  ComConstraint ctr(m1, m2);
  VectorX jerkInit(4);

  jerkInit(0) = 0.0;
  jerkInit(1) = 0.0;
  jerkInit(2) = 0.0;
  jerkInit(3) = 0.0;
  ASSERT_TRUE(ctr.getFunction(jerkInit)(0)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(1)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(2)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(3)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(4)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(5)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(6)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(7)<0.0);

  jerkInit(0) = 0.0;
  jerkInit(1) = 0.0;
  jerkInit(2) = 6.0*copLimitMax;
  jerkInit(3) = 0.0;
  ASSERT_TRUE(ctr.getFunction(jerkInit)(3)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(4)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(5)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(6)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(7)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(0)<0.0);
  ASSERT_NEAR(ctr.getFunction(jerkInit)(1), 0.0, EPSILON);
  ASSERT_NEAR(ctr.getFunction(jerkInit)(2), 0.0, EPSILON);

  jerkInit(0) = 0.0;
  jerkInit(1) = 0.0;
  jerkInit(2) = -6.0*copLimitMax;
  jerkInit(3) = 0.0;
  ASSERT_TRUE(ctr.getFunction(jerkInit)(3)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(4)<0.0);
  ASSERT_NEAR(ctr.getFunction(jerkInit)(5), 0.0, EPSILON);
  ASSERT_NEAR(ctr.getFunction(jerkInit)(6), 0.0, EPSILON);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(7)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(0)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(1)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(2)<0.0);

  jerkInit(0) = 0.0;
  jerkInit(1) = 0.0;
  jerkInit(2) = 0.0;
  jerkInit(3) = 6.0*copLimitMax;
  ASSERT_NEAR(ctr.getFunction(jerkInit)(3), 0.0, EPSILON);
  ASSERT_NEAR(ctr.getFunction(jerkInit)(4), 0.0, EPSILON);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(5)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(6)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(7)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(0)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(1)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(2)<0.0);


  jerkInit(0) = 0.0;
  jerkInit(1) = 0.0;
  jerkInit(2) = 0.0;
  jerkInit(3) = -6.0*copLimitMax;
  ASSERT_TRUE(ctr.getFunction(jerkInit)(3)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(4)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(5)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(6)<0.0);
  ASSERT_NEAR(ctr.getFunction(jerkInit)(7), 0.0, EPSILON);
  ASSERT_NEAR(ctr.getFunction(jerkInit)(0), 0.0, EPSILON);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(1)<0.0);
  ASSERT_TRUE(ctr.getFunction(jerkInit)(2)<0.0);

}


TEST_F(ZebulonComConstraintTest, sizeOfvalues)
{
  using namespace MPCWalkgen;

  int nbSamples = 3;
  Scalar samplingPeriod = 1.0;
  bool autoCompute = true;
  LIPModel m1(nbSamples, samplingPeriod, autoCompute);
  BaseModel m2(nbSamples, samplingPeriod, autoCompute);

  ComConstraint ctr(m1, m2);

  int M = m2.getComSupportConvexPolygon().getNbVertices();

  VectorX jerkInit(4*nbSamples);
  jerkInit.fill(0.0);

  ASSERT_EQ(ctr.getGradient().rows(), nbSamples*M);
  ASSERT_EQ(ctr.getGradient().cols(), 4*nbSamples);
  ASSERT_EQ(ctr.getFunction(jerkInit).rows(), nbSamples*M);
  ASSERT_EQ(ctr.getFunction(jerkInit).cols(), 1);

}