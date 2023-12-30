#include "entities/movable_entity.h"
#include "core/settings.h"
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

double positive_angle_mod(double angle){
    double positive_angle = fmod(angle, 2*M_PI);
    if (positive_angle < 0) {
        positive_angle += 2*M_PI;
    }
    return positive_angle;
}

using namespace testing;

TEST(EffectiveAngle, NoAcceleration) {
    MovableEntity creature1;
    creature1.SetSize(10);
    creature1.SetVelocity(10);
    creature1.SetVelocityAngle(0.5 * M_PI);
    creature1.SetAcceleration(0);
    double positive_angle1 = positive_angle_mod(creature1.GetEffectiveAccelerationAngle());

    ASSERT_NEAR(positive_angle1, 1.5 * M_PI, 1e-6);

    MovableEntity creature2;
    creature2.SetSize(10);
    creature2.SetVelocity(10);
    creature2.SetVelocityAngle(-0.259);
    creature2.SetAcceleration(0);

    double positive_angle2 = positive_angle_mod(creature2.GetEffectiveAccelerationAngle());
    if (positive_angle2 < 0) {
        positive_angle2 += 2*M_PI;
    }

    ASSERT_NEAR(positive_angle2, M_PI - 0.259, 1e-6);
}

TEST(EffectiveAngle, AlignedAcceleration){
    MovableEntity creature1;
    creature1.SetSize(10);
    creature1.SetAcceleration(11 * SETTINGS.environment.frictional_coefficient);
    creature1.SetAccelerationAngle(0);
    creature1.SetVelocity(10);
    creature1.SetVelocityAngle(0);

    double positive_angle1 = positive_angle_mod(creature1.GetEffectiveAccelerationAngle());

    ASSERT_NEAR(positive_angle1, 0, 1e-6);

    creature1.SetAcceleration(9 * SETTINGS.environment.frictional_coefficient);

    positive_angle1 = positive_angle_mod(creature1.GetEffectiveAccelerationAngle());

    ASSERT_NEAR(positive_angle1, M_PI, 1e-6);

    MovableEntity creature2;
    creature2.SetSize(10);
    creature2.SetAcceleration(21 * SETTINGS.environment.frictional_coefficient);
    creature2.SetAccelerationAngle(0.5 * M_PI);
    creature2.SetVelocity(10);
    creature2.SetVelocityAngle(0.5 * M_PI);

    double positive_angle2 = positive_angle_mod(creature2.GetEffectiveAccelerationAngle());

    ASSERT_NEAR(positive_angle2, 0.5 * M_PI, 1e-6);

    creature2.SetAcceleration(10 * SETTINGS.environment.frictional_coefficient);

    positive_angle2 = positive_angle_mod(creature2.GetEffectiveAccelerationAngle());

    ASSERT_NEAR(positive_angle2, 1.5 * M_PI, 1e-6);
}


