#include <creature.h>
#include <gtest/gtest.h>

/*!
 * @file creature.cpp
 *
 * @brief Unit tests for the Creature class functionalities.
 *
 * @details This file contains tests to validate the basic functionalities of
 * the Creature class, including its construction, and getting and setting
 * energy and health values.
 */

/*!
 * @brief Tests the Creature's constructor for correct initialization.
 *
 * @details Validates that a Creature object is correctly initialized with
 * default energy and health values.
 */
TEST(CreatureTests, CreatureConstructor) {
  neat::Genome genome(0, 0);
  Creature creature(genome);
  double initial_h = 100;
  double initial_e = 100;

  EXPECT_EQ(creature.GetEnergy(), initial_e);
  EXPECT_EQ(creature.GetHealth(), initial_h);
}

/*!
 * @brief Tests the Creature's constructor for correct initialization.
 *
 * @details Validates that a Creature object is correctly initialized with
 * default energy and health values.
 */
TEST(CreatureTests, GetEnergy) {
  neat::Genome genome(0, 0);
  Creature creature(genome);

  EXPECT_EQ(creature.GetEnergy(), 100);
}

/*!
 * @brief Tests the Creature's constructor for correct initialization.
 *
 * @details Validates that a Creature object is correctly initialized with
 * default energy and health values.
 */
TEST(CreatureTests, SetEnergy) {
  neat::Genome genome(0, 0);
  Creature creature(genome);
  creature.SetEnergy(50);

  EXPECT_EQ(creature.GetEnergy(), 50);
}

/*!
 * @brief Tests the Creature's ability to retrieve its health value.
 *
 * @details Ensures that the GetHealth method correctly returns the Creature's
 * current health.
 */
TEST(CreatureTests, GetHealth) {
  neat::Genome genome(0, 0);
  Creature creature(genome);

  EXPECT_EQ(creature.GetHealth(), 100);
}

/*!
 * @brief Tests the Creature's ability to set its health value.
 *
 * @details Validates that the SetHealth method correctly updates the Creature's
 * health.
 */
TEST(CreatureTests, SetHealth) {
  neat::Genome genome(0, 0);
  Creature creature(genome);
  creature.SetHealth(50);

  EXPECT_EQ(creature.GetHealth(), 50);
}

// TEST(CreatureTests, UpdateEnergy){
//     neat::Genome genome(0,0);
//     Creature creature(genome);
//     creature.SetSize(10);
//     creature.SetRotationalVelocity(1);
//     creature.SetVelocityForward(1);

//    double deltaTime = 0.05;
//    creature.UpdateEnergy(deltaTime);

//    EXPECT_EQ(creature.GetEnergy(), 80);
//}

// TEST(CreatureTests, BalanceHealthEnergy1){
//     neat::Genome genome(0,0);
//     Creature creature(genome);
//     creature.SetSize(10);
//     creature.SetRotationalVelocity(1);
//     creature.SetVelocityForward(1);

//    creature.SetEnergy(95);
//    creature.SetHealth(100);
//    double deltaTime = 0.05;
//    creature.UpdateEnergy(deltaTime);

//    EXPECT_EQ(creature.GetEnergy(), 75);
//    EXPECT_EQ(creature.GetHealth(), 100);
//}

// TEST(CreatureTests, BalanceHealthEnergy2){
//     neat::Genome genome(0,0);
//     Creature creature(genome);
//     creature.SetSize(10);
//     creature.SetRotationalVelocity(1);
//     creature.SetVelocityForward(1);
//     creature.SetEnergy(70);
//     creature.SetHealth(95);
//     double deltaTime = 0.05;
//     creature.UpdateEnergy(deltaTime);

//    EXPECT_EQ(creature.GetEnergy(), 55);
//    EXPECT_EQ(creature.GetHealth(), 90);

//}

// TEST(CreatureTests, BalanceHealthEnergy3){
//     neat::Genome genome(0,0);
//     Creature creature(genome);
//     creature.SetSize(10);
//     creature.SetRotationalVelocity(1);
//     creature.SetVelocityForward(1);
//     creature.SetEnergy(25);
//     creature.SetHealth(95);
//     double deltaTime = 0.05;
//     creature.UpdateEnergy(deltaTime);

//    EXPECT_EQ(creature.GetEnergy(), 10);
//    EXPECT_EQ(creature.GetHealth(), 90);

//}

// TEST(CreatureTests, BalanceHealthEnergy4){
//     neat::Genome genome(0,0);
//     Creature creature(genome);
//     creature.SetSize(10);
//     creature.SetRotationalVelocity(1);
//     creature.SetVelocityForward(1);
//     creature.SetEnergy(10);
//     creature.SetHealth(95);
//     double deltaTime = 0.05;
//     creature.UpdateEnergy(deltaTime);

//    EXPECT_EQ(creature.GetEnergy(), 5);
//    EXPECT_EQ(creature.GetHealth(), 80);

//}

// TEST(CreatureTests, BalanceHealthEnergy5){
//     neat::Genome genome(0,0);
//     Creature creature(genome);
//     creature.SetSize(1);
//     creature.SetRotationalVelocity(1);
//     creature.SetVelocityForward(1);
//     creature.SetEnergy(100);
//     creature.SetHealth(95);
//     double deltaTime = 0.05;
//     creature.UpdateEnergy(deltaTime);

//    EXPECT_EQ(creature.GetEnergy(), 93);
//    EXPECT_EQ(creature.GetHealth(), 100);
//}



TEST(CreatureTests, GetCellsInSight) {
  neat::Genome genome(3, 4);
  Creature creature(genome);

  std::vector<std::vector<std::vector<Entity *>>> testGrid;
  double gridCellSize = 1.0;
  testGrid.assign(10, std::vector<std::vector<Entity *>>(10));

  creature.SetCoordinates(5,5,10,10);
  creature.SetOrientation(0.0);
  creature.SetVision(3.0, M_PI / 2);

  auto cells_in_sight = creature.GetGridCellsInSight(testGrid, gridCellSize);

  std::set<std::pair<int, int>> expected_cells = {
      {5, 4}, {5, 5}, {5, 6},
      {6, 3}, {6, 4}, {6, 5}, {6,6}, {6,7},
      {7, 3}, {7, 4}, {7, 5}, {7,6}, {7,7},
      {8,5}
  };

  std::set<std::pair<int, int>> actual_cells(cells_in_sight.begin(), cells_in_sight.end());

  ASSERT_EQ(actual_cells, expected_cells);
}
