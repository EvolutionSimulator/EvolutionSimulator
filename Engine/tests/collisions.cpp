
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


// Test suite for Creature class
class CreatureTest : public ::testing::Test {
protected:
    // Helper function to set up the grid for testing
    std::unordered_map<int, std::unordered_map<int, std::vector<Entity*>>> CreateTestGrid() {
        std::unordered_map<int, std::unordered_map<int, std::vector<Entity*>>> grid;

        Food food1(3.6, 4.9, 5.0);
        Food food2(9.2, 7.1, 8.0);

        grid[3][4].push_back(&food1);
        grid[9][7].push_back(&food2);

        return grid;
    }
};

TEST_F(CreatureTest, GetClosestFood_EmptyGrid) {
    // Create a Creature with a mock genome
    Creature creature(neat::Genome(2,3));

    // Set up an empty grid
    std::unordered_map<int, std::unordered_map<int, std::vector<Entity*>>> grid;

    // Expectation: The grid is empty, so there should be no closest food
    ASSERT_DEATH({
        creature.GetClosestFood(grid, 1.0);
    }, ".*");
}

TEST_F(CreatureTest, GetClosestFood_NoFoodInGrid) {
    // Create a Creature with a mock genome
    Creature creature(neat::Genome(2,3));

    // Set up a grid with creatures but no food
    std::unordered_map<int, std::unordered_map<int, std::vector<Entity*>>> grid;
    grid[0][0].push_back(&creature);  // Add a creature to the grid

    // Expectation: There is no food in the grid, so closestFood should be nullptr
    ASSERT_DEATH({
        creature.GetClosestFood(grid, 1.0);
    }, ".*");
}

TEST_F(CreatureTest, GetClosestFoodTest) {
    // Create a Creature with a mock genome
    Creature creature1(neat::Genome(2, 2));
    Creature creature2(neat::Genome(3, 4));
    Creature creature3(neat::Genome(5, 6));

    // Set creature coordinates and sizes
    creature1.SetCoordinates(3.5, 4.5, 100.0, 100.0);
    creature2.SetCoordinates(10.5, 7.5, 100.0, 100.0);
    creature3.SetCoordinates(5.5, 5.5, 100.0, 100.0);

    creature1.SetSize(5.0);
    creature2.SetSize(10.0);
    creature3.SetSize(8.0);

    // Set up the grid
    std::unordered_map<int, std::unordered_map<int, std::vector<Entity*>>> grid = CreateTestGrid();

    // Call the GetClosestFood function
    Food* closestFood1 = creature1.GetClosestFood(grid, 1.0);  // Assuming grid cell size is 1.0
    Food* closestFood2 = creature2.GetClosestFood(grid, 1.0);
    Food* closestFood3 = creature3.GetClosestFood(grid, 1.0);

    // Expectations: Check if the closest foods are as expected
    ASSERT_EQ(dynamic_cast<Entity*>(closestFood1), grid[3][4][1]);  // Index 1 is the food1 in the grid at (3,4)
    ASSERT_EQ(dynamic_cast<Entity*>(closestFood2), grid[9][7][0]);  // Index 0 is the food2 in the grid at (9,7)
    ASSERT_EQ(dynamic_cast<Entity*>(closestFood3), grid[3][4][1]);  // Index 1 is the food1 in the grid at (3,4)
}