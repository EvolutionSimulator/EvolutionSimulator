#include "collisions.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include <iostream>

#include "creature.h"
#include "environment.h"
#include "food.h"
#include "simulationdata.h"

#include "geometry_primitives.h"

/*!
 * @file collisions.cpp
 *
 * @brief Unit tests for collision detection, creature interactions, and grid
 * functionalities in the simulation environment.
 *
 * @details This file contains tests to validate the correctness of collision
 * detection algorithms, creature behaviors in response to different scenarios,
 * grid initialization, and neighbor calculation algorithms within the
 * simulation environment.
 */

using namespace testing;

/*!
 * @brief Tests for various scenarios of circle-circle collision detection.
 *
 * @details Validates the CollisionCircleCircle function across different cases:
 *          touching circles, overlapping circles, circles with the same center,
 *          circles with one inside the other, and separated circles.
 */
TEST(CollisionTests, CollisionCircleCircle) {
  double tolerance = 0.1;

  // Case 1: Collision with circles touching at one point
  std::pair<double, double> center1_case1 = {1.0, 2.0};
  double radius1_case1 = 2.0;
  std::pair<double, double> center2_case1 = {5.0, 2.0};
  double radius2_case1 = 2.0;
  EXPECT_TRUE(CollisionCircleCircle(tolerance, center1_case1, radius1_case1,
                                    center2_case1, radius2_case1));

  // Case 2: Collision with circles overlapping
  std::pair<double, double> center1_case2 = {1.0, 2.0};
  double radius1_case2 = 3.0;
  std::pair<double, double> center2_case2 = {4.0, 6.0};
  double radius2_case2 = 2.0;
  EXPECT_TRUE(CollisionCircleCircle(tolerance, center1_case2, radius1_case2,
                                    center2_case2, radius2_case2));

  // Case 3: Collision with circles having the same center (touching but not
  // overlapping)
  std::pair<double, double> center1_case3 = {3.0, 5.0};
  double radius1_case3 = 3.0;
  std::pair<double, double> center2_case3 = {3.0,
                                             5.0};  // Same center as circle 1
  double radius2_case3 = 2.0;
  EXPECT_TRUE(CollisionCircleCircle(tolerance, center1_case3, radius1_case3,
                                    center2_case3, radius2_case3));

  // Case 4: Collision with one circle center inside the other
  std::pair<double, double> center1_case4 = {1.0, 2.0};
  double radius1_case4 = 5.0;
  std::pair<double, double> center2_case4 = {4.0, 6.0};
  double radius2_case4 = 2.0;
  EXPECT_TRUE(CollisionCircleCircle(tolerance, center1_case4, radius1_case4,
                                    center2_case4, radius2_case4));

  // Case 5: No collision with circles separated by a distance greater than
  // their combined radii
  std::pair<double, double> center1_case5 = {1.0, 2.0};
  double radius1_case5 = 2.0;
  std::pair<double, double> center2_case5 = {10.0, 10.0};
  double radius2_case5 = 5.0;
  EXPECT_FALSE(CollisionCircleCircle(tolerance, center1_case5, radius1_case5,
                                     center2_case5, radius2_case5));
}

/*!
 * @brief Tests for collision detection between a circle and a line segment.
 *
 * @details Validates the CollisionCircleLine function for intersecting and
 * non-intersecting scenarios, including vertical and horizontal line segments.
 */
TEST(CollisionTests, CollisionCircleLine) {
  const double tolerance = 0.1;

  // Case 1: Circle and line segment intersect
  std::pair<double, double> center1 = {2.0, 3.0};
  double radius1 = 2.0;
  std::pair<double, double> linePoint1_case1 = {1.0, 1.0};
  std::pair<double, double> linePoint2_case1 = {4.0, 5.0};
  EXPECT_TRUE(CollisionCircleLine(tolerance, center1, radius1, linePoint1_case1,
                                  linePoint2_case1));

  // Case 2: Circle and line segment do not intersect
  std::pair<double, double> center2 = {8.0, 8.0};
  double radius2 = 2.0;
  std::pair<double, double> linePoint1_case2 = {1.0, 1.0};
  std::pair<double, double> linePoint2_case2 = {4.0, 5.0};
  EXPECT_FALSE(CollisionCircleLine(tolerance, center2, radius2,
                                   linePoint1_case2, linePoint2_case2));

  // Case 3: Vertical line segment passes through center
  std::pair<double, double> center3 = {4.0, 3.0};
  double radius3 = 1.0;
  std::pair<double, double> linePoint1_case3 = {4.0, 1.0};
  std::pair<double, double> linePoint2_case3 = {4.0, 5.0};
  EXPECT_TRUE(CollisionCircleLine(tolerance, center3, radius3, linePoint1_case3,
                                  linePoint2_case3));

  // Case 4: Circle and vertical line segment intersect
  std::pair<double, double> center4 = {2.0, 3.0};
  double radius4 = 2.0;
  std::pair<double, double> linePoint1_case4 = {1.0, 2.0};
  std::pair<double, double> linePoint2_case4 = {1.0, 5.0};
  EXPECT_TRUE(CollisionCircleLine(tolerance, center4, radius4, linePoint1_case4,
                                  linePoint2_case4));

  // Case 5: Circle and horizontal line segment do not intersect
  std::pair<double, double> center5 = {2.0, 3.0};
  double radius5 = 1.0;
  std::pair<double, double> linePoint1_case5 = {1.0, 5.0};
  std::pair<double, double> linePoint2_case5 = {4.0, 5.0};
  EXPECT_FALSE(CollisionCircleLine(tolerance, center5, radius5,
                                   linePoint1_case5, linePoint2_case5));
}

/*!
 * @brief Tests for distance calculation between two entities without
 * considering map wrap-around.
 *
 * @details Ensures that the GetDistance method correctly calculates the
 * Euclidean distance between entities when wrap-around is not applicable.
 */
TEST(CollisionTests, GetDistance_NoWrapAround) {
  Entity e1(10.0, 10.0, 10.0);
  Entity e2(13.0, 14.0, 3.0);
  double mapWidth = 100;
  int mapHeight = 100;

  double expectedDistance = std::hypot(3, 4);
  ASSERT_NEAR(e1.GetDistance(e2, mapWidth, mapHeight), expectedDistance, 1e-6);
}

/*!
 * @brief Tests for distance calculation between entities considering map
 * wrap-around along the X-axis.
 *
 * @details Ensures that the GetDistance method correctly calculates the
 * shortest distance between entities, considering the wrap-around effect along
 * the X-axis of the map.
 */
TEST(CollisionTests, GetDistance_WrapAroundXCoordinate) {
  Entity e1(1.0, 10.0, 2.0);
  Entity e2(99.0, 10.0, 5.0);
  double mapWidth = 100;
  int mapHeight = 100;

  double expectedDistance = 2;
  ASSERT_NEAR(e1.GetDistance(e2, mapWidth, mapHeight), expectedDistance, 1e-6);
}

/*!
 * @brief Tests for distance calculation between entities considering map
 * wrap-around along the Y-axis.
 *
 * @details Ensures that the GetDistance method correctly calculates the
 * shortest distance between entities, considering the wrap-around effect along
 * the Y-axis of the map.
 */
TEST(CollisionTests, GetDistance_WrapAroundYCoordinate) {
  Entity e1(10.0, 1.0, 3.0);
  Entity e2(10.0, 99.0, 2.0);
  double mapWidth = 100;
  int mapHeight = 100;

  double expectedDistance = 2;
  ASSERT_NEAR(e1.GetDistance(e2, mapWidth, mapHeight), expectedDistance, 1e-6);
}

/*!
 * @brief Tests for distance calculation between entities considering map
 * wrap-around along both coordinates.
 *
 * @details Ensures that the GetDistance method correctly calculates the
 * shortest distance between entities, considering the wrap-around effect along
 * both the X-axis and Y-axis of the map.
 */
TEST(CollisionTests, GetDistance_WrapAroundBothCoordinates) {
  Entity e1(1.0, 1.0, 2.0);
  Entity e2(99.0, 99.0, 3.0);
  double mapWidth = 100;
  int mapHeight = 100;

  double expectedDistance = std::hypot(2, 2);
  ASSERT_NEAR(e1.GetDistance(e2, mapWidth, mapHeight), expectedDistance, 1e-6);
}

/*!
 * @brief Tests for checking collision detection between two entities.
 *
 * @details Validates the CheckCollisionWithEntity function in various scenarios
 * including overlapping entities, entities placed far apart, entities with one
 * inside the other, and entities wrapped within the map bounds.
 */
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

  // Entities with coordinates outside of bounds, expected to be wrapped within
  // bounds
  entity1.SetCoordinates(90.0, 55.0, kMapWidth, kMapHeight);
  entity1.SetSize(4.0);
  entity2.SetCoordinates(90.0, 60.0, kMapWidth, kMapHeight);
  entity2.SetSize(3.0);
  EXPECT_TRUE(entity1.CheckCollisionWithEntity(tolerance, entity2));
}

/*!
 * @brief Tests the interaction between creatures and food entities upon
 * collision.
 *
 * @details Validates different scenarios like collision with alive and dead
 * food, and changes in creature's energy based on the nutritional value of the
 * food.
 */
TEST(CollisionTests, OnCollisionWithFood) {
  const double tolerance = 0.1;

  // Create a Creature and Food for testing
  neat::Genome genome(2, 3);
  Mutable mutables;
  Creature creature(genome, mutables);
  Food food;
  food.SetSize(1.0);

  // Case 1: Food is alive, and Creature collides with it
  food.SetState(Entity::Alive);
  food.SetNutritionalValue(10.0);
  creature.SetEnergy(30.0);
  double initialEnergy = creature.GetEnergy();
  creature.OnCollision(food, 100, 100);

  // Expected result: Creature's energy increases by nutritional value, and food
  // is dead
  EXPECT_EQ(creature.GetEnergy(), initialEnergy + 10.0);
  EXPECT_EQ(food.GetState(), Entity::Dead);

  // Case 2: Food is dead, and Creature collides with it
  food.SetState(Entity::Dead);
  food.SetNutritionalValue(10.0);
  initialEnergy = creature.GetEnergy();
  creature.OnCollision(food, 100, 100);

  // Expected result: Creature's energy remains unchanged, and food is still
  // dead
  EXPECT_EQ(creature.GetEnergy(), initialEnergy);
  EXPECT_EQ(food.GetState(), Entity::Dead);

  // Case 3: Food is alive, but Creature has zero energy
  creature.SetEnergy(0.0);
  food.SetState(Entity::Alive);
  food.SetNutritionalValue(10.0);
  creature.OnCollision(food, 100, 100);

  // Expected result: Creature's energy increases by nutritional value, and food
  // is dead
  EXPECT_EQ(creature.GetEnergy(), 10.0);
  EXPECT_EQ(food.GetState(), Entity::Dead);

  // Case 4: Food is dead, Creature has non-zero energy, and Creature collides
  // again
  double initialEnergy2 = creature.GetEnergy();
  creature.OnCollision(food, 100, 100);

  // Expected result: Creature's energy remains unchanged, and food is still
  // dead
  EXPECT_EQ(creature.GetEnergy(), initialEnergy2);
  EXPECT_EQ(food.GetState(), Entity::Dead);
}

/*!
 * @brief Tests for handling collision between a creature and food entities.
 *
 * @details Validates various collision scenarios between creatures and food,
 * including collisions with alive and dead food, changes in creature's energy
 * based on food's nutritional value, and scenarios where food is within and
 * outside the creature's reach.
 */
TEST(CollisionTests, OnCollisionWithEntity) {
  Entity entity1, entity2;
  double kMapWidth = 100.0, kMapHeight = 100.0;

  // Set initial coordinates and sizes
  entity1.SetCoordinates(50.0, 50.0, kMapWidth, kMapHeight);
  entity1.SetSize(10.0);
  entity2.SetCoordinates(60.0, 60.0, kMapWidth, kMapHeight);
  entity2.SetSize(10.0);
  EXPECT_TRUE(entity1.CheckCollisionWithEntity(0.0, entity2));

  // Call OnCollision
  entity1.OnCollision(entity2, kMapWidth, kMapHeight);

  // Check if the distance between the entities is greater than the sum of their
  // sizes
  double distance = entity1.GetDistance(entity2, kMapWidth, kMapHeight);
  double sum_of_sizes = entity1.GetSize() + entity2.GetSize();
  EXPECT_FALSE(entity1.CheckCollisionWithEntity(0.0, entity2));
}

/*!
 * @brief Tests for Creature class functionalities.
 *
 * @details This test suite checks various behaviors of creatures, including
 * interactions with food entities and their positioning within the simulation
 * environment.
 */
class CreatureTest : public ::testing::Test {
 protected:
  // Helper function to set up the grid for testing
  std::vector<std::vector<std::vector<Entity*>>> CreateTestGrid() {
    std::vector<std::vector<std::vector<Entity*>>> grid;
    grid.assign(100, std::vector<std::vector<Entity*>>(100));

    Food* food1 = new Food(3.6, 4.9, 5.0);
    Food* food2 = new Food(9.2, 7.1, 8.0);

    grid[3][4].push_back(food1);
    grid[9][7].push_back(food2);

    return grid;
  }
};

TEST_F(CreatureTest, GetClosestFood_EmptyGrid) {
  // Create a Creature with a mock genome
  Mutable mutables;
  Creature creature(neat::Genome(2, 3), mutables);

  // Set up an empty grid
  std::vector<std::vector<std::vector<Entity*>>> grid;

  // Expectation: The grid is empty, so there should be no closest food
  EXPECT_EQ(creature.GetClosestFood(grid, 1.0), nullptr);
  /*
  ASSERT_DEATH({
      creature.GetClosestFood(grid, 1.0);
  }, ".*");*/
}

/*!
 * @brief Tests the Creature's ability to identify the closest food entity in an
 * empty grid.
 *
 * @details Validates that the Creature correctly identifies the absence of food
 * entities in an empty grid.
 */
TEST_F(CreatureTest, GetClosestFood_NoFoodInGrid) {
  // Create a Creature with a mock genome
  Mutable mutables;
  Creature creature(neat::Genome(2, 3), mutables);
  creature.SetCoordinates(0, 0, 10, 10);

  // Set up a grid with creatures but no food
  std::vector<std::vector<std::vector<Entity*>>> grid = {{{}, {}}, {{}, {}}};

  grid[0][0].push_back(&creature);  // Add a creature to the grid

  // Expectation: There is no food in the grid, so closestFood should be nullptr
  EXPECT_EQ(creature.GetClosestFood(grid, 1.0), nullptr);
  /*
  ASSERT_DEATH({
      creature.GetClosestFood(grid, 1.0);
  }, ".*");*/
}

/*!
 * @brief Tests the Creature's ability to identify the closest food entity in a
 * grid with multiple food entities.
 *
 * @details Validates that the Creature correctly identifies the nearest food
 * entity based on its position in the grid.
 */
TEST_F(CreatureTest, GetClosestFoodTest) {
  // Create a Creature with a mock genome
  Mutable mutables;
  Creature creature1(neat::Genome(2, 2), mutables);
  Creature creature2(neat::Genome(3, 4), mutables);
  Creature creature3(neat::Genome(5, 6), mutables);

  // Set creature coordinates and sizes
  creature1.SetCoordinates(3.5, 4.5, 100.0, 100.0);
  creature2.SetCoordinates(10.5, 7.5, 100.0, 100.0);
  creature3.SetCoordinates(5.5, 5.5, 100.0, 100.0);

  creature1.SetSize(5.0);
  creature2.SetSize(10.0);
  creature3.SetSize(8.0);

  // Set up the grid
  std::vector<std::vector<std::vector<Entity*>>> grid = CreateTestGrid();

  // Call the GetClosestFood function
  Food* closestFood1 =
      creature1.GetClosestFood(grid, 1.0);  // Assuming grid cell size is 1.0
  Food* closestFood2 = creature2.GetClosestFood(grid, 1.0);
  Food* closestFood3 = creature3.GetClosestFood(grid, 1.0);

  // Expectations: Check if the closest foods are as expected
  ASSERT_EQ(dynamic_cast<Entity*>(closestFood1),
            grid[3][4][0]);  // Index 0 is the food1 in the grid at (3,4)
  ASSERT_EQ(dynamic_cast<Entity*>(closestFood2),
            grid[9][7][0]);  // Index 0 is the food2 in the grid at (9,7)
  ASSERT_EQ(dynamic_cast<Entity*>(closestFood3),
            grid[3][4][0]);  // Index 0 is the food1 in the grid at (3,4)
  delete grid[3][4][0];
  delete grid[9][7][0];
}

/*!
 * @brief Tests retrieving food entities at a specific distance from a given
 * position in the grid.
 *
 * @details Ensures that food entities at a specified distance from a given
 * position are correctly identified and retrieved.
 */
TEST_F(CreatureTest, GetFoodAtDistanceTest) {
  // Define the grid
  std::vector<std::vector<std::vector<Entity*>>> grid;
  grid.assign(100, std::vector<std::vector<Entity*>>(100));

  Food* f1 = new Food();
  Food* f2 = new Food();
  Food* f3 = new Food();
  Entity* e1 = new Entity();
  grid[3][1].push_back(f1);
  grid[3][3].push_back(f2);
  grid[3][3].push_back(e1);
  grid[2][1].push_back(f3);

  std::vector<Food*> result = get_food_at_distance(grid, 1, 1, 1);

  EXPECT_EQ(result.size(), 1);
  delete f1;
  delete f2;
  delete e1;
  delete f3;
}

/*!
 * @brief Tests the Creature's ability to identify the closest food entity under
 * different scenarios.
 *
 * @details Validates that the Creature correctly identifies the closest food
 * entity, considering various positions and states of food entities.
 */
TEST_F(CreatureTest, GetClosestFoodTest2) {
  // Define the grid
  std::vector<std::vector<std::vector<Entity*>>> grid;
  grid.assign(100, std::vector<std::vector<Entity*>>(100));

  double grid_cell_size = 1;
  Mutable mutables;
  Creature* cr = new Creature(neat::Genome(2, 2), mutables);
  cr->SetCoordinates(1.6, 1.1, 100.0, 100.0);
  Food* f1 = new Food(2.5, 0.5);
  Food* f2 = new Food(2.2, 0.9);
  Food* f3 = new Food(2.9, 1.9);
  Entity* e1 = new Entity(1.5, 1.1, 0.5);
  grid[1][1].push_back(e1);
  grid[2][1].push_back(f3);
  grid[2][0].push_back(f2);
  grid[2][0].push_back(f1);

  Food* result = cr->GetClosestFood(grid, grid_cell_size);
  EXPECT_EQ(result->GetCoordinates().first, 2.2);
  EXPECT_EQ(result->GetCoordinates().second, 0.9);

  delete f1;
  delete f2;
  delete e1;
  delete f3;
  delete cr;
}

/*!
 * @brief Tests for correct initialization of the simulation grid.
 *
 * @details Ensures that the grid is initialized with the expected number of
 * cells based on the simulation settings.
 */
TEST(SimulationDataTest, GridInitialization) {
  myEnvironment::Environment environment;
  SimulationData simData(environment);
  simData.InitializeGrid();

  int expectedNumCellsX =
      static_cast<int>(
          std::ceil(static_cast<double>(settings::environment::kMapWidth) /
                    settings::environment::kGridCellSize)) +
      1;
  int expectedNumCellsY =
      static_cast<int>(
          std::ceil(static_cast<double>(settings::environment::kMapHeight) /
                    settings::environment::kGridCellSize)) +
      1;

  EXPECT_EQ(simData.GetGrid().size(), expectedNumCellsX);
  for (const auto& row : simData.GetGrid()) {
    EXPECT_EQ(row.size(), expectedNumCellsY);
  }
}

/*!
 * @brief Tests for updating the grid with alive entities.
 *
 * @details Validates that the grid correctly places alive entities and ignores
 * dead ones.
 */
TEST(SimulationDataTest, UpdateGridWithAliveEntities) {
  myEnvironment::Environment environment;
  SimulationData simData(environment);
  simData.InitializeGrid();
  simData.creatures_.clear();
  simData.food_entities_.clear();
  simData.ClearGrid();

  Mutable mutables;
  Creature creature_1(neat::Genome(2, 3), mutables);
  creature_1.SetCoordinates(2.1, 3.4, 100.0, 100.0);
  creature_1.SetSize(10.0);
  simData.creatures_.push_back(creature_1);

  Creature creature_2(neat::Genome(2, 3), mutables);
  creature_2.SetCoordinates(1.1, 2.4, 100.0, 100.0);
  creature_2.SetSize(10.0);
  creature_2.SetState(Entity::Dead);
  simData.creatures_.push_back(creature_2);

  simData.food_entities_.push_back(Food(2.5, 3.4, 5.0));
  simData.food_entities_.push_back(Food(2.7, 9, 5.0));
  Food food(5.3, 6.7, 4.0);
  food.SetState(Entity::Dead);
  simData.food_entities_.push_back(food);

  simData.UpdateGrid();

  for (const auto& row : simData.GetGrid()) {
    for (const auto& cell : row) {
      for (const auto& entity : cell) {
        EXPECT_EQ(entity->GetState(), Entity::Alive);
      }
    }
  }
}

/*!
 * @brief Tests for correct placement of entities in the grid.
 *
 * @details Ensures that entities are placed in the correct grid cells even when
 *          their coordinates are outside the nominal bounds of the map.
 */
TEST(SimulationDataTest, CorrectEntityPlacementInGrid) {
  myEnvironment::Environment environment;
  SimulationData simData(environment);
  simData.InitializeGrid();
  simData.creatures_.clear();
  simData.food_entities_.clear();
  simData.ClearGrid();

  Mutable mutables;
  Creature creature_1(neat::Genome(2, 3), mutables);
  creature_1.SetCoordinates(200.1, 300.4, settings::environment::kMapWidth,
                            settings::environment::kMapHeight);
  creature_1.SetSize(10.0);
  simData.creatures_.push_back(creature_1);

  Creature creature_2(neat::Genome(2, 3), mutables);
  creature_2.SetCoordinates(100.1, 350.4, settings::environment::kMapWidth,
                            settings::environment::kMapHeight);
  creature_2.SetSize(10.0);
  creature_2.SetState(Entity::Dead);
  simData.creatures_.push_back(creature_2);

  Food food_1(200.5, 300.4, 5.0);
  Food food_2(200.7, 200, 5.0);
  simData.food_entities_.push_back(food_1);
  simData.food_entities_.push_back(food_2);
  Food food_3(50.3, 60.7, 4.0);
  food_3.SetState(Entity::Dead);
  simData.food_entities_.push_back(food_3);

  simData.UpdateGrid();

  auto creatureCoordinates = creature_1.GetCoordinates();
  int creatureGridX = static_cast<int>(creatureCoordinates.first /
                                       settings::environment::kGridCellSize);
  int creatureGridY = static_cast<int>(creatureCoordinates.second /
                                       settings::environment::kGridCellSize);

  auto foodCoordinates = food_1.GetCoordinates();
  int foodGridX = static_cast<int>(foodCoordinates.first /
                                   settings::environment::kGridCellSize);
  int foodGridY = static_cast<int>(foodCoordinates.second /
                                   settings::environment::kGridCellSize);

  EXPECT_NE(simData.GetGrid()[creatureGridX][creatureGridY].empty(), true);
  EXPECT_NE(simData.GetGrid()[foodGridX][foodGridY].empty(), true);
}

/*!
 * @brief Tests for calculating neighboring cells in a grid.
 *
 * @details Validates the GetNeighbours function for various scenarios,
 * including cells at corners, edges, and outside the grid boundaries.
 */

/*!
 * @brief Tests the basic functionality of the GetNeighbours function.
 *
 * @details Ensures that the correct neighboring grid cells are identified for a
 * given central cell in a standard scenario.
 */
TEST(GetNeighboursTest, BasicFunctionality) {
  auto neighbours = GetNeighbours(10, 10, {5, 5}, 1);
  std::vector<std::pair<int, int>> expected = {
      {4, 4}, {4, 5}, {4, 6}, {5, 4}, {5, 5}, {5, 6}, {6, 4}, {6, 5}, {6, 6}};
  EXPECT_EQ(neighbours, expected);
}

/*!
 * @brief Tests neighbor calculation when the center cell is at a corner of the
 * grid.
 *
 * @details Validates that GetNeighbours correctly identifies neighbors for a
 * cell located at the corner of the grid, considering wrap-around effects.
 */
TEST(GetNeighboursTest, CenterAtCorner) {
  auto neighbours = GetNeighbours(10, 10, {0, 0}, 1);
  std::vector<std::pair<int, int>> expected = {
      {9, 9}, {9, 0}, {9, 1}, {0, 9}, {0, 0}, {0, 1}, {1, 9}, {1, 0}, {1, 1}};
  EXPECT_EQ(neighbours, expected);
  EXPECT_EQ(neighbours, expected);
}

/*!
 * @brief Tests neighbor calculation when the center cell is on the edge of the
 * grid along the X-axis.
 *
 * @details Ensures that GetNeighbours correctly identifies neighbors for a cell
 * located on the horizontal edge of the grid, accounting for wrap-around.
 */
TEST(GetNeighboursTest, CenterAtXEdge) {
  auto neighbours = GetNeighbours(10, 10, {0, 5}, 1);
  std::vector<std::pair<int, int>> expected = {
      {9, 4}, {9, 5}, {9, 6}, {0, 4}, {0, 5}, {0, 6}, {1, 4}, {1, 5}, {1, 6}};
  EXPECT_EQ(neighbours, expected);
}

/*!
 * @brief Tests the GetNeighbours function when the layer number is zero.
 *
 * @details Validates that GetNeighbours correctly identifies only the central
 * cell when the layer number is zero.
 */
TEST(GetNeighboursTest, ZeroLayerNumber) {
  auto neighbours = GetNeighbours(10, 10, {5, 5}, 0);
  std::vector<std::pair<int, int>> expected = {{5, 5}};
  EXPECT_EQ(neighbours, expected);
}

/*!
 * @brief Tests neighbor calculation when the center cell is outside the grid
 * boundaries.
 *
 * @details Ensures that GetNeighbours handles out-of-bound center cell
 * coordinates correctly, applying wrap-around logic to identify neighbors.
 */
TEST(GetNeighboursTest, CenterOutsideGrid) {
  auto neighbours = GetNeighbours(10, 10, {11, 11}, 1);
  std::vector<std::pair<int, int>> expected = {
      {0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}};
  EXPECT_EQ(neighbours, expected);
}


TEST(OrientedAngleTest, Normalize) {
  OrientedAngle angle1(3 * M_PI);
  EXPECT_NEAR(angle1.GetAngle(), -M_PI, settings::engine::EPS);

  OrientedAngle angle2(-4 * M_PI);
  EXPECT_NEAR(angle2.GetAngle(), 0.0, settings::engine::EPS);

  OrientedAngle angle3(7 * M_PI / 4);
  EXPECT_NEAR(angle3.GetAngle(), -M_PI/4, settings::engine::EPS);

  OrientedAngle angle4(-11 * M_PI / 6);
  EXPECT_NEAR(angle4.GetAngle(), M_PI/6, settings::engine::EPS);
}

TEST(OrientedAngleTest, IsInsideCone_1) {
  OrientedAngle left_boundary(0);
  OrientedAngle right_boundary(M_PI / 2);

  OrientedAngle insideAngle(M_PI / 4);
  EXPECT_TRUE(insideAngle.IsInsideCone(left_boundary, right_boundary));

  OrientedAngle onBoundary(M_PI / 2);
  EXPECT_TRUE(onBoundary.IsInsideCone(left_boundary, right_boundary));

  OrientedAngle outsideAngle(M_PI);
  EXPECT_FALSE(outsideAngle.IsInsideCone(left_boundary, right_boundary));
}

TEST(OrientedAngleTest, IsInsideCone_2) {
  OrientedAngle left_boundary(3 * M_PI / 2);
  OrientedAngle right_boundary(M_PI / 2);

  EXPECT_NEAR(left_boundary.GetAngle(), -M_PI/2, settings::engine::EPS);
  EXPECT_NEAR(right_boundary.GetAngle(), M_PI/2, settings::engine::EPS);

  OrientedAngle insideAngle1(0);
  EXPECT_TRUE(insideAngle1.IsInsideCone(left_boundary, right_boundary));

  OrientedAngle insideAngle2(7 * M_PI / 4);
  EXPECT_TRUE(insideAngle2.IsInsideCone(left_boundary, right_boundary));

  OrientedAngle onLeftBoundary(3 * M_PI / 2);
  EXPECT_TRUE(onLeftBoundary.IsInsideCone(left_boundary, right_boundary));

  OrientedAngle onRightBoundary(M_PI / 2);
  EXPECT_TRUE(onRightBoundary.IsInsideCone(left_boundary, right_boundary));

  OrientedAngle outsideAngle1(M_PI);
  EXPECT_FALSE(outsideAngle1.IsInsideCone(left_boundary, right_boundary));

  OrientedAngle outsideAngle2(5 * M_PI / 4);
  EXPECT_NEAR(outsideAngle2.GetAngle(), -3*M_PI/4, settings::engine::EPS);
  EXPECT_NEAR(outsideAngle2.AngleDistanceToCone(left_boundary, right_boundary), M_PI/4, settings::engine::EPS);
  EXPECT_FALSE(outsideAngle2.IsInsideCone(left_boundary, right_boundary));
}

TEST(OrientedAngleTest, IsInsideCone_3) {
  OrientedAngle left_boundary(M_PI / 2);
  OrientedAngle right_boundary(- M_PI / 2);

  OrientedAngle outsideAngle1(0);
  EXPECT_FALSE(outsideAngle1.IsInsideCone(left_boundary, right_boundary));

  OrientedAngle outsideAngle2(7 * M_PI / 4);
  EXPECT_FALSE(outsideAngle2.IsInsideCone(left_boundary, right_boundary));
  EXPECT_NEAR(outsideAngle2.AngleDistanceToCone(left_boundary, right_boundary), M_PI/4, settings::engine::EPS);

  OrientedAngle onLeftBoundary(M_PI / 2);
  EXPECT_TRUE(onLeftBoundary.IsInsideCone(left_boundary, right_boundary));

  OrientedAngle onRightBoundary(3 * M_PI / 2);
  EXPECT_TRUE(onRightBoundary.IsInsideCone(left_boundary, right_boundary));

  OrientedAngle insideAngle1(M_PI);
  EXPECT_TRUE(insideAngle1.IsInsideCone(left_boundary, right_boundary));

  OrientedAngle insideAngle2(5 * M_PI / 4);
  EXPECT_TRUE(insideAngle2.IsInsideCone(left_boundary, right_boundary));
}


