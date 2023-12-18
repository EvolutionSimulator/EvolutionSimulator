#include "movable_entity.h"

#include <gtest/gtest.h>
#include "math.h"

/*!
 * @file collisions.cpp
 *
 * @brief Unit tests for movement functionalities, friction and accelerations
 *
 * @details This file contains tests to validate the correctness of the movement
 * functionalities involving the friction forcess and the accelerations
 */

using namespace testing;

TEST(FrictionTests, FrictionAngle) {
    MovableEntity creature;
    creature.SetSize(10);
    creature.SetVelocity(10);
    creature.SetVelocityAngle(0.5 * M_PI);
    creature.SetAcceleration(0);

    EXPECT_DOUBLE_EQ(fmod(creature.GetEffectiveAccelerationAngle(), 2*M_PI), 1.5 * M_PI);
}
