//
// Created by Niels on 24/12/21.
//

#include "sea/Sea.h"
#include "game/DBManager.h"

void Sea::handleInput(int key)
{
    while (ship.getState() == ShipStates::SAILING)
        playRound();

    std::tuple<int, int> menuPos{menuHandler::handleInput(options, key, minLine, options.size() + minLine)};
    int posY{std::get<1>(menuPos)};

    if (ship.getState() == ShipStates::COMBAT && key == KEY_SPACE)
        handleCombat(posY);
}

Sea::Sea(std::tuple<int, std::string, int> dest, Ship &ship) : ship(ship), destination(std::move(dest))
{
    std::vector<std::string> menu =
            {
                    "1. Shoot",
                    "2. Flee",
                    "3. Surrender"
            };

    setOptions(menu);
}

void Sea::playRound()
{
    int encounter{RNG::generateRandomNumber(4)};
    if (encounter <= 4)
    {
        ship.setState(ShipStates::COMBAT);
        pirateShip = generatePirateShip();
        system("cls");
        menuHandler::resetCursor();
        std::cout << "A pirate ship has engaged you in combat!" << std::endl
        << "it looks like it has about " << pirateShip->getTotalCanons() << " canons" << std::endl
        <<  "What would you like to do?" << std::endl << std::endl;

        showOptions();
        minLine += 4;
        return;
    }

    int wind{RNG::generateRandomNumber(1, 20)};
    int movement{0};

    if (wind >= 1 && wind <= 2)
        movement = 0;
    else if (wind >= 3 && wind <= 4)
        movement = ship.getAbility() == "licht" ? 1 : 0;
    else if (wind >= 5 && wind <= 7)
        movement = ship.getAbility() == "log" ? 0 : 1;
    else if (wind >= 8 && wind <= 17)
        movement = 1;
    else if (wind >= 18 && wind <= 19)
        movement = 2;
    else if (wind == 20)
        movement = handleStorm();

    std::get<2>(destination) -= movement;
    std::cout << "The current wind causes you to move " << movement << " miles" << std::endl;
    system("pause");
}

int Sea::handleStorm()
{
    int rand{RNG::generateRandomNumber(1, 10)};
    int result{0};

    std::cout << "You got caught in a storm!" << std::endl;

    if (rand >= 1 && rand <= 4)
        result = -1;
    else if (rand >= 5 && rand <= 8)
        result = 0;
    else if (rand >= 8 && rand <= 10)
    {
        int healthLoss{RNG::generateRandomNumber(100)};
        int health{ship.getCurrentHealth()};
        int damage{(healthLoss / 100) * health};

        ship.takeDamage(damage);
        std::cout << "Your ship took " << damage << " damage from the storm!" << std::endl;
    }

    return result;
}

void Sea::handleCombat(int key)
{

}

std::optional<Ship> Sea::generatePirateShip()
{
    const auto randomShip{DB::selectData(
            "SELECT s.type, s.prijs, s.laadruimte, s.kanonnen, s.schadepunten, "
            "(SELECT bijzonderheid FROM bijzonderheden b WHERE b.id = sb.bijzonderheid_id) "
            "FROM schepen s INNER JOIN schepen_bijzonderheden sb on s.id = sb.schip_id ORDER BY random() LIMIT 1")[0]};
    auto tempShip = Ship{randomShip[0], std::stoi(randomShip[1]), std::stoi(randomShip[2]), std::stoi(randomShip[3]),
                         std::stoi(randomShip[4]), randomShip[5], 0};

    int totalCanons{2 + RNG::generateRandomNumber(0, (tempShip.getCanonSpace() - 2))};
    for (int i = 0; i < totalCanons; ++i)
    {
        int randType{RNG::generateRandomNumber(1, 3)};
        std::string type;

        switch (randType)
        {
            case 1:
                type = "small canon";
                break;
            case 2:
                type = "medium canon";
                break;
            case 3:
                type = "large canon";
                break;
        }

        if(tempShip.getAbility() == "licht" && type == "large canon")
            type = "medium canon";

        tempShip.boughtCanon(type, 1, 0);
    }

    return tempShip;
}
