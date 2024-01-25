#include <creature.h>
#include <gtest/gtest.h>

#include "geometry_primitives.h"
#include "settings.h"

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
  Mutable mutables;
  Creature creature(genome, mutables);

  double size = SETTINGS.physical_constraints.d_baby_size;
  double initial_health = SETTINGS.physical_constraints.d_integrity * pow(size, 2);
  double initial_energy = SETTINGS.physical_constraints.d_energy_density * pow(size, 2);
  EXPECT_EQ(creature.GetEnergy(), initial_energy);
  EXPECT_EQ(creature.GetHealth(), initial_health);
}

// /*!
//  * @brief Tests the Creature's constructor for correct initialization.
//  *
//  * @details Validates that a Creature object is correctly initialized with
//  * default energy and health values.
//  */
// TEST(CreatureTests, GetEnergy) {
//   neat::Genome genome(0, 0);
//   Mutable mutables;
//   Creature creature(genome, mutables);

//   EXPECT_EQ(creature.GetEnergy(), 100);
// }

// /*!
//  * @brief Tests the Creature's constructor for correct initialization.
//  *
//  * @details Validates that a Creature object is correctly initialized with
//  * default energy and health values.
//  */
// TEST(CreatureTests, SetEnergy) {
//   neat::Genome genome(0, 0);
//   Mutable mutables;
//   Creature creature(genome, mutables);
//   creature.SetEnergy(50);

//   EXPECT_EQ(creature.GetEnergy(), 50);
// }

// /*!
//  * @brief Tests the Creature's ability to retrieve its health value.
//  *
//  * @details Ensures that the GetHealth method correctly returns the
//  Creature's
//  * current health.
//  */
// TEST(CreatureTests, GetHealth) {
//   neat::Genome genome(0, 0);
//   Mutable mutables;
//   Creature creature(genome, mutables);

//   EXPECT_EQ(creature.GetHealth(), 100);
// }

// /*!
//  * @brief Tests the Creature's ability to set its health value.
//  *
//  * @details Validates that the SetHealth method correctly updates the
//  Creature's
//  * health.
//  */
// TEST(CreatureTests, SetHealth) {
//   neat::Genome genome(0, 0);
//   Mutable mutables;
//   Creature creature(genome, mutables);
//   creature.SetHealth(50);

//   EXPECT_EQ(creature.GetHealth(), 50);
// }

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

TEST(CreatureTests, IsGridCellPotentiallyInsideCone) {
  double grid_cell_size = 1.0;

  auto grid_in_sight_1 = IsGridCellPotentiallyInsideCone(
      Point(5, 5), grid_cell_size, Point(5, 5), 3.0, OrientedAngle(-M_PI / 4),
      OrientedAngle(M_PI / 4), 10.0, 10.0);

  auto grid_in_sight_2 = IsGridCellPotentiallyInsideCone(
      Point(6, 4), grid_cell_size, Point(5, 5), 3.0, OrientedAngle(-M_PI / 4),
      OrientedAngle(M_PI / 4), 10.0, 10.0);

  auto grid_in_sight_3 = IsGridCellPotentiallyInsideCone(
      Point(7, 6), grid_cell_size, Point(5, 5), 3.0, OrientedAngle(-M_PI / 4),
      OrientedAngle(M_PI / 4), 10.0, 10.0);

  auto grid_out_of_sight = IsGridCellPotentiallyInsideCone(
      Point(15, 19), grid_cell_size, Point(1, 1), 3.0, OrientedAngle(-M_PI / 4),
      OrientedAngle(M_PI / 4), 10.0, 10.0);

  EXPECT_TRUE(grid_in_sight_1);
  EXPECT_TRUE(grid_in_sight_2);
  EXPECT_TRUE(grid_in_sight_3);
  EXPECT_FALSE(grid_out_of_sight);
}

TEST(CreatureTests, GetClosestFoodInSight_MultipleFoods) {
  neat::Genome genome(3, 4);
  Mutable mutables;
  Creature creature(genome, mutables);

  std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> grid;
  double gridCellSize = 1.0;
  grid.assign(10, std::vector<std::vector<std::shared_ptr<Entity>>>(10));

  std::shared_ptr<Meat> meat_1, meat_2, meat_3;
  meat_1->SetCoordinates(3.79138, 2.77046, 10, 10); // distance = 0.95
  grid[3][2].push_back(meat_1);
  meat_2->SetCoordinates(2.93273, 2.87064, 10, 10); // distance = 1.52
  grid[2][2].push_back(meat_2);
  meat_3->SetCoordinates(3.87724,2.52718, 10, 10); // distance = 1.14
  grid[3][2].push_back(meat_3);

  double creature_x = 4.26364, creature_y = 3.60048;
  creature.SetCoordinates(creature_x, creature_y, 10, 10);
  double target_x = 2.84687, target_y = 2.26958;
  creature.SetOrientation(
      OrientedAngle(Point(creature_x, creature_y), Point(target_x, target_y), 10.0, 10.0)
          .GetAngle());
  creature.SetVision(2.0, M_PI / 3);

  auto closest_food =
      creature.GetClosestEntityInSight(grid, gridCellSize, 10.0, 10.0);

  ASSERT_EQ(closest_food[0], meat_1);
}

TEST(CreatureTests, GetClosestFoodInSight_NoFoodInSight) {
  neat::Genome genome(3, 4);
  Mutable mutables;

  Creature creature(genome, mutables);

  std::vector<std::vector<std::vector<std::shared_ptr<Entity>>>> grid;
  double gridCellSize = 1.0;
  grid.assign(10, std::vector<std::vector<std::shared_ptr<Entity>>>(10));

  std::shared_ptr<Meat> meat_1, meat_2, meat_3;
  meat_1->SetSize(0.02);
  meat_1->SetCoordinates(4.82, 3.06, 10, 10); // distance = 0.78, angle = 90
  grid[3][2].push_back(meat_1);
  meat_2->SetSize(0.02);
  meat_2->SetCoordinates(2.93273, 2.87064, 10, 10); // distance = 1.52
  grid[2][2].push_back(meat_2);
  meat_3->SetSize(0.02);
  meat_3->SetCoordinates(3.87724,2.52718, 10, 10); // distance = 1.14
  grid[3][2].push_back(meat_3);

  double creature_x = 4.26364, creature_y = 3.60048;
  creature.SetCoordinates(creature_x, creature_y, 10, 10);
  double target_x = 2.84687, target_y = 2.26958;
  creature.SetOrientation(
      OrientedAngle(Point(creature_x, creature_y), Point(target_x, target_y), 10.0, 10.0)
          .GetAngle());
  creature.SetVision(1, M_PI / 3);

  auto closest_food =
      creature.GetClosestEntityInSight(grid, gridCellSize, 10.0, 10.0);

  ASSERT_EQ(closest_food[0], nullptr);
}

TEST(CreatureTests, Digest) {
  // Initialize creature with specific energy and stomach fullness
  neat::Genome genome(3, 4);
  Mutable mutables;
  Creature creature(genome, mutables);

  double deltaTime = 1.0;
  std::shared_ptr<Plant> plant = std::make_shared<Plant>(3.0);
  creature.DigestiveSystem::Bite(plant);
  creature.AddAcid(1.0);

  double initialEnergy = creature.GetEnergy();
  double initialStomachFullness = creature.GetStomachFullness();

  creature.SetMaxEnergy(100);
  creature.Digest(deltaTime);

  ASSERT_GT(creature.GetEnergy(), initialEnergy);
  ASSERT_LT(creature.GetStomachFullness(), initialStomachFullness);
}

TEST(CreatureTests, BiteFood) {
  neat::Genome genome(3, 4);
  Mutable mutables;
  Creature creature(genome, mutables);
  std::shared_ptr<Plant> food = std::make_shared<Plant>(3.0);

  double initialFoodSize = food->GetSize();
  double initialStomachFullness = creature.GetStomachFullness();

  creature.DigestiveSystem::Bite(food);

  ASSERT_LT(food->GetSize(), initialFoodSize);
  ASSERT_GT(creature.GetStomachFullness(), initialStomachFullness);
}

TEST(CreatureTests, BiteCreature) {
  neat::Genome genome1(3, 4);
  neat::Genome genome2(3, 4);
  Mutable mutables;
  Creature creature1(genome1, mutables);
  Creature creature2(genome2, mutables);

  double initialHealth = creature2.GetHealth();

  creature1.Bite(std::make_shared<Creature>(creature2));

  ASSERT_LT(creature2.GetHealth(), initialHealth);
}
/*
TEST(CreatureTests, Grab) {
  neat::Genome genome(3, 4);
  Mutable mutables;
  Creature creature(genome, mutables);
  Entity entity;

  double initialEnergy = creature.GetEnergy();

  creature.Grab(std::make_shared<Entity>(entity));

  ASSERT_EQ(creature.GetGrabbedEntity(), std::make_shared<Entity>(entity));
  //ASSERT_TRUE(dynamic_cast<GrabbingEntity*>(&entity)->IsBeingGrabbedBy(&creature));
  ASSERT_LT(creature.GetEnergy(), initialEnergy);
}
*/

TEST(CreatureTests, EmptinessPercent) {
  neat::Genome genome(3, 4);
  Mutable mutables;
  Creature creature(genome, mutables);

  double expectedEmptiness = 0;

  ASSERT_EQ(creature.GetEmptinessPercent(), 100.0);

  std::shared_ptr<Plant> food = std::make_shared<Plant>(3.0);
  creature.DigestiveSystem::Bite(food);

  ASSERT_LT(creature.GetStomachFullness(), 100.0);
}
