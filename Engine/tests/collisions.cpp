
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "collisions.h"
#include "creature.h"
#include "food.h"


using namespace testing;

TEST(CollisionTests, CollisionCircleCircle) {
    double tolerance = 0.1;

    // Case 1: Collision with circles touching at one point
    std::pair<double, double> center1_case1 = {1.0, 2.0};
    double radius1_case1 = 2.0;
    std::pair<double, double> center2_case1 = {5.0, 2.0};
    double radius2_case1 = 2.0;
    EXPECT_TRUE(CollisionCircleCircle(tolerance, center1_case1, radius1_case1, center2_case1, radius2_case1));

    // Case 2: Collision with circles overlapping
    std::pair<double, double> center1_case2 = {1.0, 2.0};
    double radius1_case2 = 3.0;
    std::pair<double, double> center2_case2 = {4.0, 6.0};
    double radius2_case2 = 2.0;
    EXPECT_TRUE(CollisionCircleCircle(tolerance, center1_case2, radius1_case2, center2_case2, radius2_case2));

    // Case 3: Collision with circles having the same center (touching but not overlapping)
    std::pair<double, double> center1_case3 = {3.0, 5.0};
    double radius1_case3 = 3.0;
    std::pair<double, double> center2_case3 = {3.0, 5.0};  // Same center as circle 1
    double radius2_case3 = 2.0;
    EXPECT_TRUE(CollisionCircleCircle(tolerance, center1_case3, radius1_case3, center2_case3, radius2_case3));

    // Case 4: Collision with one circle center inside the other
    std::pair<double, double> center1_case4 = {1.0, 2.0};
    double radius1_case4 = 5.0;
    std::pair<double, double> center2_case4 = {4.0, 6.0};
    double radius2_case4 = 2.0;
    EXPECT_TRUE(CollisionCircleCircle(tolerance, center1_case4, radius1_case4, center2_case4, radius2_case4));

    // Case 5: No collision with circles separated by a distance greater than their combined radii
    std::pair<double, double> center1_case5 = {1.0, 2.0};
    double radius1_case5 = 2.0;
    std::pair<double, double> center2_case5 = {10.0, 10.0};
    double radius2_case5 = 5.0;
    EXPECT_FALSE(CollisionCircleCircle(tolerance, center1_case5, radius1_case5, center2_case5, radius2_case5));
}

TEST(CollisionTests, CollisionCircleLine) {
    const double tolerance = 0.1;

    // Case 1: Circle and line segment intersect
    std::pair<double, double> center1 = {2.0, 3.0};
    double radius1 = 2.0;
    std::pair<double, double> linePoint1_case1 = {1.0, 1.0};
    std::pair<double, double> linePoint2_case1 = {4.0, 5.0};
    EXPECT_TRUE(CollisionCircleLine(tolerance, center1, radius1, linePoint1_case1, linePoint2_case1));

    // Case 2: Circle and line segment do not intersect
    std::pair<double, double> center2 = {8.0, 8.0};
    double radius2 = 2.0;
    std::pair<double, double> linePoint1_case2 = {1.0, 1.0};
    std::pair<double, double> linePoint2_case2 = {4.0, 5.0};
    EXPECT_FALSE(CollisionCircleLine(tolerance, center2, radius2, linePoint1_case2, linePoint2_case2));

    // Case 3: Vertical line segment passes through center
    std::pair<double, double> center3 = {4.0, 3.0};
    double radius3 = 1.0;
    std::pair<double, double> linePoint1_case3 = {4.0, 1.0};
    std::pair<double, double> linePoint2_case3 = {4.0, 5.0};
    EXPECT_TRUE(CollisionCircleLine(tolerance, center3, radius3, linePoint1_case3, linePoint2_case3));

    // Case 4: Circle and vertical line segment intersect
    std::pair<double, double> center4 = {2.0, 3.0};
    double radius4 = 2.0;
    std::pair<double, double> linePoint1_case4 = {1.0, 2.0};
    std::pair<double, double> linePoint2_case4 = {1.0, 5.0};
    EXPECT_TRUE(CollisionCircleLine(tolerance, center4, radius4, linePoint1_case4, linePoint2_case4));

    // Case 5: Circle and horizontal line segment do not intersect
    std::pair<double, double> center5 = {2.0, 3.0};
    double radius5 = 1.0;
    std::pair<double, double> linePoint1_case5 = {1.0, 5.0};
    std::pair<double, double> linePoint2_case5 = {4.0, 5.0};
    EXPECT_FALSE(CollisionCircleLine(tolerance, center5, radius5, linePoint1_case5, linePoint2_case5));

}

TEST(CollisionTests, CheckCollisionWithEntity) {
    const double tolerance = 0.1;
    const double kMapWidth = 100.0;
    const double kMapHeight = 50.0;

    // Create two entities for testing
    Entity entity1;
    Entity entity2;

    // Set coordinates and size for entity1
    entity1.SetCoordinates(1.0, 2.0, kMapWidth, kMapHeight);
    entity1.SetSize(2.0);

    // Set coordinates and size for entity2
    entity2.SetCoordinates(4.0, 6.0, kMapWidth, kMapHeight);
    entity2.SetSize(3.0);

    // Entities are expected to collide
    EXPECT_TRUE(entity1.CheckCollisionWithEntity(tolerance, entity2));

    // Move entity2 away, no collision expected
    entity2.SetCoordinates(10.0, 10.0, kMapWidth, kMapHeight);
    EXPECT_FALSE(entity1.CheckCollisionWithEntity(tolerance, entity2));

    // Entities with one inside the other, expected to collide
    entity1.SetCoordinates(1.0, 2.0, kMapWidth, kMapHeight);
    entity1.SetSize(5.0);
    entity2.SetCoordinates(4.0, 6.0, kMapWidth, kMapHeight);
    entity2.SetSize(2.0);
    EXPECT_TRUE(entity1.CheckCollisionWithEntity(tolerance, entity2));

    // Entities with combined radii, expected to collide
    entity1.SetCoordinates(1.0, 2.0, kMapWidth, kMapHeight);
    entity1.SetSize(2.0);
    entity2.SetCoordinates(4.0, 6.0, kMapWidth, kMapHeight);
    entity2.SetSize(3.0);
    EXPECT_TRUE(entity1.CheckCollisionWithEntity(tolerance, entity2));

    // Entities with coordinates outside of bounds, expected to be wrapped within bounds
    entity1.SetCoordinates(90.0, 55.0, kMapWidth, kMapHeight);
    entity1.SetSize(4.0);
    entity2.SetCoordinates(90.0, 60.0, kMapWidth, kMapHeight);
    entity2.SetSize(3.0);
    EXPECT_TRUE(entity1.CheckCollisionWithEntity(tolerance, entity2));
}


TEST(CollisionTests, OnCollisionWithFood) {
    const double tolerance = 0.1;

    // Create a Creature and Food for testing
    neat::Genome genome(2, 3);
    Creature creature(genome);
    Food food;

    // Case 1: Food is alive, and Creature collides with it
    food.SetState(Entity::Alive);
    food.SetNutritionalValue(10.0);
    double initialEnergy = creature.GetEnergy();
    creature.OnCollision(food);

    // Expected result: Creature's energy increases by nutritional value, and food is dead
    EXPECT_EQ(creature.GetEnergy(), initialEnergy + 10.0);
    EXPECT_EQ(food.GetState(), Entity::Dead);

    // Case 2: Food is dead, and Creature collides with it
    food.SetState(Entity::Dead);
    food.SetNutritionalValue(10.0);
    initialEnergy = creature.GetEnergy();
    creature.OnCollision(food);

    // Expected result: Creature's energy remains unchanged, and food is still dead
    EXPECT_EQ(creature.GetEnergy(), initialEnergy);
    EXPECT_EQ(food.GetState(), Entity::Dead);

    // Case 3: Food is alive, but Creature has zero energy
    creature.SetEnergy(0.0);
    food.SetState(Entity::Alive);
    food.SetNutritionalValue(10.0);
    creature.OnCollision(food);

    // Expected result: Creature's energy increases by nutritional value, and food is dead
    EXPECT_EQ(creature.GetEnergy(), 10.0);
    EXPECT_EQ(food.GetState(), Entity::Dead);

    // Case 4: Food is dead, Creature has non-zero energy, and Creature collides again
    double initialEnergy2 = creature.GetEnergy();
    creature.OnCollision(food);

    // Expected result: Creature's energy remains unchanged, and food is still dead
    EXPECT_EQ(creature.GetEnergy(), initialEnergy2);
    EXPECT_EQ(food.GetState(), Entity::Dead);
}
