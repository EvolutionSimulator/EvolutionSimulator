#include <gtest/gtest.h>

#include <iostream>

#include "egg.h"

TEST(ReproductionTest, SimulatePregnancy) {
  neat::Genome genome(4, 4);
  Mutable mutables;
  Creature male(genome, mutables);
  Creature female(genome, mutables);

  double maturity_age = male.GetMaleReproductiveSystem()->GetMaturityAge() + 10;
  male.GetMaleReproductiveSystem()->Update(maturity_age);
  female.GetFemaleReproductiveSystem()->Update(maturity_age);

  ASSERT_TRUE(male.GetMaleReproductiveSystem()->ReadyToProcreate());
  ASSERT_TRUE(female.GetFemaleReproductiveSystem()->ReadyToProcreate());

  male.GetMaleReproductiveSystem()->MateWithFemale();
  female.GetFemaleReproductiveSystem()->MateWithMale(&male, &female);
  male.AfterMate();
  female.AfterMate();

  double incubation_time =
      female.GetFemaleReproductiveSystem()->GetEgg().incubation_time;
  female.GetFemaleReproductiveSystem()->Update(incubation_time);

  ASSERT_TRUE(female.GetFemaleReproductiveSystem()->IsPregnant());
  ASSERT_TRUE(female.GetFemaleReproductiveSystem()->CanBirth());

  Egg egg = female.GetFemaleReproductiveSystem()->GiveBirth();
  ASSERT_FALSE(female.GetFemaleReproductiveSystem()->IsPregnant());
  ASSERT_FALSE(female.GetFemaleReproductiveSystem()->CanBirth());
}