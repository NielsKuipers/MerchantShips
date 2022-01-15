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
                  << "What would you like to do?" << std::endl << std::endl;

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
    menuHandler::setCursorForText(0, minLine + options.size() + 1);
    menuHandler::deleteLines(5, 100);

    //surrender
    if (key == 2)
    {
        int amountStolen{0};
        std::string result{"The pirateship stole "};

        if((ship.getTotalCargo() - pirateShip->getCargoSpace()) > 0)
        {
            amountStolen += pirateShip->getCargoSpace();
            result += (std::to_string(amountStolen) + " goods and threw the rest of your cargo overboard!");
        } else
        {
            amountStolen += ship.getTotalCargo();
            result += (std::to_string(amountStolen) + " goods!");
        }

        std::cout << result << std::endl << "You get away safely" << std::endl;
        ship.removeCargo();

        endCombat();
        return;
    }

    const int dmgDone{getDamage(ship.getCanons())};
    const int dmgTaken{getDamage(pirateShip->getCanons())};

    //attacking ship
    if (key == 0)
    {
        std::cout << "You shoot at the enemy ship and hit it for " << dmgDone << " damage!" << std::endl << std::endl;
        pirateShip->takeDamage(dmgDone);
        if (pirateShip->getCurrentHealth() <= 0)
        {
            std::cout << "the enemy ship is riddled with holes... and sinks.";
            endCombat();
            return;
        }
    }

    //pirate ship attacks you
    std::cout << "The enemy ship fires its canons at you and hits you for " << dmgTaken << " damage!" << std::endl
              << std::endl;
    ship.takeDamage(dmgTaken);
    if (ship.getCurrentHealth() <= 0)
    {
        ship.setState(ShipStates::DEAD);
        std::cout
                << "The pirateship proved too strong for you and your ship slowly sinks to the bottom of the ocean..."
                << std::endl;
        endCombat();
        return;
    }

    //escape attempt
    if (key == 1)
    {
        if (handleEscape())
        {
            std::cout << "You escaped the pirateship" << std::endl;
            endCombat();
        } else
            std::cout << "You couldn't escape the vessel" << std::endl;
    }
}

int Sea::getDamage(const std::map<CanonType, int> &canons)
{
    int dmg{0};
    for (const auto&[type, value]: canons)
    {
        if (type == CanonType::SMALL)
            dmg += RNG::generateRandomNumber(0, 2);
        else if (type == CanonType::MEDIUM)
            dmg += RNG::generateRandomNumber(0, 4);
        else if (type == CanonType::LARGE)
            dmg += RNG::generateRandomNumber(0, 6);
    }

    return dmg;
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
        CanonType type;

        switch (randType)
        {
            case 1:
                type = CanonType::SMALL;
                break;
            case 2:
                type = CanonType::MEDIUM;
                break;
            case 3:
                type = CanonType::LARGE;
                break;
        }

        if (tempShip.getAbility() == "licht" && type == CanonType::LARGE)
            type = CanonType::MEDIUM;

        tempShip.boughtCanon(type, 1, 0);
    }

    return tempShip;
}

bool Sea::handleEscape()
{
    int escapeChance{0};

    if (ship.getAbility() == "klein")
        escapeChance += 50;
    else if (ship.getAbility() == "licht")
        escapeChance += 30;
    else if (ship.getAbility() == "log")
        escapeChance += 5;

    if (pirateShip->getAbility() == "licht")
        escapeChance += 10;
    else if (pirateShip->getAbility() == "log")
        escapeChance += 25;

    if (escapeChance >= RNG::generateRandomNumber(100))
        return true;
    else
        return false;
}

void Sea::endCombat()
{
    ship.setState(ShipStates::SAILING);
    pirateShip = std::nullopt;
    menuHandler::resetCursor();
    minLine -= 4;
    system("pause");
    system("cls");
}
