//
// Created by Niels on 24/12/21.
//

#include "sea/Sea.h"
#include "game/DBManager.h"
#include "../../exceptions/QuitGame.h"

void Sea::handleInput(int key)
{
    while (ship.getState() == ShipStates::SAILING)
        playRound();

    std::tuple<int, int> menuPos{UIHandler::handleInput(options, key, minLine, options.size() + minLine)};
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
    if (encounter == 0)
    {
        ship.setState(ShipStates::COMBAT);
        pirateShip = generatePirateShip();
        system("cls");
        UIHandler::resetCursor();
        UIHandler::outputText("A pirate ship has engaged you in combat!;it looks like it has about " +
                              std::to_string(pirateShip->getTotalCanons()) + " canons;What would you like to do?;;");

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
    UIHandler::outputText("The current wind causes you to move " + std::to_string(movement) + " miles;You are now " +
                          std::to_string(std::get<2>(destination)) + " miles from your destination;;");

    if (std::get<2>(destination) <= 0)
    {
        UIHandler::outputText(
                "You can see the harbor approaching in the distance;You have arrived at your location!;");
        ship.setState(ShipStates::ARRIVING);
    }

    system("pause");
}

int Sea::handleStorm()
{
    int rand{RNG::generateRandomNumber(1, 10)};
    int result{0};

    UIHandler::outputText("You got caught in a strom!;");

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
        UIHandler::outputText("Your ship took " + std::to_string(damage) + " damage from the storm!;");
    }

    return result;
}

void Sea::handleCombat(int key)
{
    UIHandler::setCursorForText(0, minLine + options.size() + 1);
    UIHandler::deleteLines(7, 100);

    //surrender
    if (key == 2)
    {
        int amountStolen{0};
        std::string result{"The pirateship stole "};

        if ((ship.getTotalCargo() - pirateShip->getCargoSpace()) > 0)
        {
            amountStolen += pirateShip->getCargoSpace();
            result += (std::to_string(amountStolen) + " goods and threw the rest of your cargo overboard!");
        } else
        {
            amountStolen += ship.getTotalCargo();
            result += (std::to_string(amountStolen) + " goods!");
        }

        UIHandler::outputText(result + ";You get away safely;");
        ship.removeCargo();

        endCombat();
        return;
    }

    const int dmgDone{getDamage(ship.getCanons())};
    const int dmgTaken{getDamage(pirateShip->getCanons())};

    //attacking ship
    if (key == 0)
    {
        UIHandler::outputText(
                "You shoot at the enemy ship and hit it for " + std::to_string(dmgDone) + " damage!;It has " +
                std::to_string(pirateShip->getCurrentHealth()) + " health left;;");
        pirateShip->takeDamage(dmgDone);
        if (pirateShip->getCurrentHealth() <= 0)
        {
            UIHandler::outputText("the enemy ship is riddled with holes... and sinks.");
            endCombat();
            return;
        }
    }

    //pirate ship attacks you
    UIHandler::outputText("The enemy ship fires its canons ar you and hits you for " + std::to_string(dmgTaken) +
                          " damage!;You have " + std::to_string(ship.getCurrentHealth()) + " health left;;");
    ship.takeDamage(dmgTaken);
    if (ship.getCurrentHealth() <= 0)
    {
        ship.setState(ShipStates::DEAD);
        UIHandler::outputText(
                "The pirateship proved too strong for you and your ship slowly sinks to the bottom of the ocean...;Game over...;");
        endCombat();
        throw QuitGame(-EINVAL, "Quit the game successfully");
        return;
    }

    //escape attempt
    if (key == 1)
    {
        if (handleEscape())
        {
            UIHandler::outputText("You escaped the pirateship!;");
            endCombat();
        } else
            UIHandler::outputText("You couldn't escape the vessel;");
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
    UIHandler::resetCursor();
    minLine -= 4;
    system("pause");
    system("cls");
}
