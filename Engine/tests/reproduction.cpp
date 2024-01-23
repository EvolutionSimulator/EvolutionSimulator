#include <gtest/gtest.h>

#include <iostream>

#include "egg.h"

TEST(ReproductionTest, SimulatePregnancy) {
  neat::Genome genome(4, 4);
  Mutable mutables;
  Creature male(genome, mutables);
  Creature female(genome, mutables);

  double maturity_age = male.MaleReproductiveSystem::GetMaturityAge() + 10;
  male.MaleReproductiveSystem::Update(maturity_age);
  female.FemaleReproductiveSystem::Update(maturity_age);

  ASSERT_TRUE(male.MaleReproductiveSystem::ReadyToProcreate());
  ASSERT_TRUE(female.FemaleReproductiveSystem::ReadyToProcreate());

  male.MaleReproductiveSystem::MateWithFemale();
  female.FemaleReproductiveSystem::MateWithMale(&male, &female);
  male.AfterMate();
  female.AfterMate();

  double incubation_time =
      female.FemaleReproductiveSystem::GetEgg().incubation_time;
  female.FemaleReproductiveSystem::Update(incubation_time);

  ASSERT_TRUE(female.FemaleReproductiveSystem::IsPregnant());
  ASSERT_TRUE(female.FemaleReproductiveSystem::CanBirth());

  std::shared_ptr<Egg> egg =
      female.FemaleReproductiveSystem::GiveBirth(female.GetCoordinates());
  ASSERT_FALSE(female.FemaleReproductiveSystem::IsPregnant());
  ASSERT_FALSE(female.FemaleReproductiveSystem::CanBirth());
}