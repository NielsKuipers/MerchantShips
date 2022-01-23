//
// Created by Niels on 23/01/22.
//

#include "gtest/gtest.h"
#include "../include/harbor/Harbor.h"

class ShipTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        ship.boughtItem("paint", 20, 0);
    }

    std::tuple<std::string, int, int, int, int, std::string> newShip{"sloep", 19000, 100, 14, 110, "log"};
    Ship ship{"Pinnace", 10000, 200, 8, 100, "klein", 500000};
};

TEST_F(ShipTest, buyItem)
{
    std::string item{"cabbage"};
    int amount{10};
    int spent{100};
    int goldBefore{ship.getGold()};
    int cargoBefore{ship.getTotalCargo()};

    ship.boughtItem(item, amount, spent);

    ASSERT_EQ(ship.getItemAmount(item), amount);
    ASSERT_EQ(ship.getGold(), (goldBefore - spent));
    ASSERT_EQ(ship.getTotalCargo(), (cargoBefore + amount));
}

TEST_F(ShipTest, sellItem)
{
    int goldBefore{ship.getGold()};
    int amount{ship.getItemAmount("paint")};
    int earned{200};
    int cargoBefore{ship.getTotalCargo()};

    ship.soldItem("paint", amount, earned);

    ASSERT_EQ(ship.getItemAmount("paint"), 0);
    ASSERT_EQ(ship.getGold(), (goldBefore + earned));
    ASSERT_EQ(ship.getTotalCargo(), (cargoBefore - amount));
}

TEST_F(ShipTest, takesDamage)
{
    int dmg{10};
    int healthBefore{ship.getCurrentHealth()};
    ship.takeDamage(dmg);

    ASSERT_EQ(ship.getCurrentHealth(), (healthBefore - dmg));
}

TEST_F(ShipTest, canBuyNewShip)
{
    ship.changeShip(newShip);

    ASSERT_EQ(ship.getType(), "sloep");
}

TEST_F(ShipTest, removeCargoIfTooSmall)
{
    std::string item{"cabbage"};
    int amount{200};

    ship.boughtItem("cabbage", amount, 0);
    ship.changeShip(newShip);

    ASSERT_EQ(ship.getCargoSpace(), 0);
}
