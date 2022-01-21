//
// Created by Niels on 11/12/21.
//

#include "harbor/Harbor.h"

Harbor::Harbor(int id, const std::string &name, Ship &ship) : ship(ship)
{
    this->id = id;
    this->name = name;

    displayMenu();
    generateGoods();
    getLocations();
}

void Harbor::handleInput(int key)
{
    std::tuple<int, int> menuPos;
    int posY;

    menuPos = UIHandler::handleInput(options, key, minLine, options.size() + minLine);
    posY = std::get<1>(menuPos);

    if ((currentState == HarborStates::MENU) && key == KEY_SPACE)
    {
        setState(posY);
        UIHandler::resetCursor();
    }

    switch (currentState)
    {
        case HarborStates::GOODS:
            currentShop = currentState;
            displayShop();
            break;
        case HarborStates::CANONS:
            currentShop = currentState;
            displayShop();
            break;
        case HarborStates::SHIPS:
            currentShop = currentState;
            displayShips();
            break;
        case HarborStates::MAP:
            displayDestinations();
            break;
        case HarborStates::REPAIRING:
            ship.repair();
            displayMenu();
            setState(HarborStates::MENU);
            break;
        case HarborStates::QUIT:
            throw QuitGame(-EINVAL, "Quit the game successfully");
            break;
        case HarborStates::MENU:
            break;
        case HarborStates::TRADING_SHIP:
            if (key == KEY_SPACE)
            {
                buyShip(posY);
                displayShips();
            } else if (key == KEY_BACKSPACE)
                displayMenu();
            break;
        case HarborStates::TRADING:
            if (key == KEY_B)
            {
                buyItem(posY);
                displayShop();
            } else if (key == KEY_S)
            {
                sellItem(posY);
                displayShop();
            } else if (key == KEY_BACKSPACE)
                displayMenu();
            break;
        case HarborStates::LEAVING:
            if (key == KEY_SPACE)
                leaveHarbor(posY);
            else if (key == KEY_BACKSPACE)
                displayMenu();
            break;
    }
}

void Harbor::displayMenu()
{
    system("cls");
    minLine = ship.displayShipInfo();
    UIHandler::outputText("Welcome to the " + name + " harbor!;");
    ++minLine;

    menu =
            {
                    "1. Trade goods",
                    "2. Trade canons",
                    "3. Trade ships",
                    "4. Leave harbor",
                    "5. Repair ship",
                    "6. Quit game"
            };

    setOptions(menu);
    showOptions();
    UIHandler::resetCursor();
    setState(HarborStates::MENU);
}

void Harbor::generateGoods()
{
    const auto goodsData{DB::selectData(
            "SELECT g2.goed, g.min_goed, g.max_goed, g.min_prijs, g.max_prijs "
            "FROM havens_goederen g INNER JOIN goederen g2 on g.goed_id = g2.id "
            "WHERE g.haven_id = " + std::to_string(id))};

    const auto shipData{DB::selectData(
            "SELECT s.type, s.prijs, s.laadruimte, s.kanonnen, s.schadepunten, "
            "(SELECT bijzonderheid FROM bijzonderheden b WHERE b.id = sb.bijzonderheid_id) "
            "FROM schepen s "
            "INNER JOIN schepen_bijzonderheden sb on s.id = sb.schip_id "
            "ORDER BY random() LIMIT 3")};

    for (const auto &rawShip: shipData)
    {
        std::tuple<std::string, int, int, int, int, std::string> finalShip
                {
                        rawShip[0], std::stoi(rawShip[1]),
                        std::stoi(rawShip[2]), std::stoi(rawShip[3]),
                        std::stoi(rawShip[4]), rawShip[5]
                };

        ships.emplace_back(finalShip);
    }

    for (const auto &productData: goodsData)
    {
        const int amount{RNG::generateRandomNumber(std::stoi(productData[1]), std::stoi(productData[2]))};
        const int price{RNG::generateRandomNumber(std::stoi(productData[3]), std::stoi(productData[4]))};
        std::tuple<std::string, int, int> product{productData[0], amount, price};

        goods.emplace_back(product);
    }

    canons.emplace_back(std::make_tuple("small canon", RNG::generateRandomNumber(0, 5), 50));
    canons.emplace_back(std::make_tuple("medium canon", RNG::generateRandomNumber(0, 3), 200));

    //light ships can't buy heavy canons
    if (ship.getAbility() != "licht")
        canons.emplace_back(std::make_tuple("large canon", RNG::generateRandomNumber(0, 2), 1000));
}

void Harbor::displayShop()
{
    system("cls");
    options.clear();
    UIHandler::resetCursor();
    const auto &items{(currentShop == HarborStates::GOODS) ? goods : canons};

    //display information and set the minline to 5 so it skips these in selection
    UIHandler::outputText("Wecome to the " + name + " market;" +
                          "You can buy items by pressing B, and sell them by pressing S;;" +
                          "You currently have " + std::to_string(ship.getGold()) + " gold and room for "
                          + std::to_string(ship.getCargoSpace()) + " goods;;");
    minLine = 5;

    for (const auto &product: items)
    {
        const std::string line{
                "Product: " + std::get<0>(product) +
                " amount available: " + std::to_string(std::get<1>(product)) +
                " price: " + std::to_string(std::get<2>(product)) +
                " You have: " + std::to_string(ship.getItemAmount(std::get<0>(product)))
        };

        options.emplace_back(line);
    }

    this->showOptions();
    setState(HarborStates::TRADING);
}

template<typename T>
void Harbor::setState(T state)
{
    currentState = HarborStates(state);
}

void Harbor::buyItem(int y)
{
    auto &item{currentShop == HarborStates::GOODS ? goods[y] : canons[y]};
    int gold{ship.getGold()};
    int space{currentShop == HarborStates::GOODS ? ship.getCargoSpace() : ship.getCanonSpace()};
    int amount;

    system("cls");
    UIHandler::outputText("Ah, you want to buy " + std::get<0>(item) + "?;"
                                                                       "This will cost you " +
                          std::to_string(std::get<2>(item)) + " gold each;;"
                                                              "How many do you wish to buy?;Amount to buy:");
    amount = UIHandler::getNumberInput();

    int totalCost = amount * std::get<2>(item);
    while (totalCost > gold || amount > space || amount > std::get<1>(item))
    {
        if (amount > space)
            UIHandler::outputText(";Your ship does not have enough room for this purchase, try again.;");
        else if (amount > std::get<1>(item))
            UIHandler::outputText(
                    "We only have " + std::to_string(std::get<1>(item)) + " of that item, please try again.;");
        else
            UIHandler::outputText(";You do not have enough gold for this purchase, try again.;");

        UIHandler::outputText("Amount to buy: ");
        amount = UIHandler::getNumberInput();
        totalCost = amount * std::get<2>(item);
    }

    if (amount != 0)
    {
        currentShop == HarborStates::GOODS ? ship.boughtItem(std::get<0>(item), amount, totalCost)
                                           : ship.boughtCanon(convertToCanon(std::get<0>(item)), amount, totalCost);
        std::get<1>(item) -= amount;
        UIHandler::outputText(";You bought " + std::to_string(amount) + " " + std::get<0>(item)
                              + " for a total of " + std::to_string(totalCost) + " gold;;");
        system("pause");
    }
}

void Harbor::sellItem(int y)
{
    auto &item{currentShop == HarborStates::GOODS ? goods[y] : canons[y]};
    int has{ship.getItemAmount(std::get<0>(item))};
    int amount;

    system("cls");
    UIHandler::outputText("Yeah, we buy " + std::get<0>(item) + ";How many do you wish to sell?;;Amount to sell: ");
    amount = UIHandler::getNumberInput();

    int totalEarned = amount * std::get<2>(item);
    while (amount > has)
    {
        UIHandler::outputText(";Are you trying to cheat me? you only have " + std::to_string(has) + " of this item;");
        UIHandler::outputText("Amount to sell: ");
        amount = UIHandler::getNumberInput();
        totalEarned = amount * std::get<2>(item);
    }

    if (amount != 0)
    {
        currentShop == HarborStates::GOODS ? ship.soldItem(std::get<0>(item), amount, totalEarned)
                                           : ship.soldCanon(convertToCanon(std::get<0>(item)), amount, totalEarned);

        std::get<1>(item) += 1;
        UIHandler::outputText(";You sold " + std::to_string(amount) + " " + std::get<0>(item)
                              + " for a total of " + std::to_string(totalEarned) + " gold;;");

        if (ship.getGold() > 1000000)
            ship.setState(ShipStates::VICTORY);

        system("pause");
    }
}

void Harbor::displayShips()
{
    system("cls");
    options.clear();
    UIHandler::resetCursor();

    UIHandler::outputText("Want to buy a new ship, eh?;"
                          "Here is our selection;; Buy a ship with spacebar."
                          ";Buying a ship will automatically sell you current one for half the original price"
                          ";Selling your current ship will earn you " + std::to_string(ship.getPrice() / 2) +
                          " gold.;;");
    minLine = 7;

    for (const auto &shipData: ships)
    {
        const std::string line{
                "Type: " + std::get<0>(shipData) +
                " Price: " + std::to_string(std::get<1>(shipData)) +
                " Cargospace: " + std::to_string(std::get<2>(shipData)) +
                " Canonspace: " + std::to_string(std::get<3>(shipData)) +
                " Health: " + std::to_string(std::get<4>(shipData)) +
                " Ability: " + std::to_string(std::get<3>(shipData))
        };

        options.emplace_back(line);
    }

    this->showOptions();
    setState(HarborStates::TRADING_SHIP);
}

void Harbor::buyShip(int y)
{
    auto &item{ships[y]};
    int gold{ship.getGold()};

    system("cls");

    if (gold + (ship.getPrice() / 2) < std::get<1>(item))
    {
        UIHandler::outputText("You do not have enough gold for this ship.;;");
        system("pause");
        return;
    }

    UIHandler::outputText(
            "Ah, the " + std::get<0>(item) + "? A fine choice.; This will cost you " +
            std::to_string(std::get<1>(item)) +
            " gold; This will exchange your current ship for the new one, are you sure?;;");

    if (ship.getTotalCanons() > std::get<3>(item))
        UIHandler::outputText(
                "You have more canons than you have room for on the new ship, you will lose some of them;;");
    if (ship.getTotalCargo() > std::get<2>(item))
        UIHandler::outputText(
                "You have more goods than you have room for on the new ship, you will lose some of them;;");

    auto result{UIHandler::getConfirmInput()};

    if (result == "y")
    {
        ship.changeShip(item);
        UIHandler::outputText(";You have successfully bought the " + std::get<0>(item) + "!;;");
        system("pause");
    } else
        return;
}

void Harbor::getLocations()
{
    const auto locationData{DB::selectData(
            "SELECT (SELECT haven FROM havens WHERE haven1_id=havens.id), a.haven1_id, h.haven, a.haven2_id, a.afstand "
            "FROM afstanden a INNER JOIN havens h on h.id = a.haven2_id "
            "WHERE haven1_id in (" + std::to_string(id) + ") OR haven2_id in (" + std::to_string(id) +
            ") AND haven1_id < haven2_id")};

    for (const auto &location: locationData)
    {
        std::string destination;
        int locationId;

        if (location[0] == name)
        {
            destination = location[2];
            locationId = std::stoi(location[3]);
        } else
        {
            destination = location[0];
            locationId = std::stoi(location[1]);
        }

        locations.emplace_back(std::make_tuple(locationId, destination, std::stoi(location[4])));
    }
}

void Harbor::displayDestinations()
{
    system("cls");
    options.clear();
    UIHandler::resetCursor();

    UIHandler::outputText("You take a look at the map and find some suitable locations to sail to;;");
    minLine = 2;

    for (const auto &locationData: locations)
    {
        const std::string line{
                "Location: " + std::get<1>(locationData) +
                " Distance: " + std::to_string(std::get<2>(locationData))
        };

        options.emplace_back(line);
    }

    this->showOptions();
    setState(HarborStates::LEAVING);
}

void Harbor::leaveHarbor(int y)
{
    ship.setDestination(locations[y]);
    ship.setState(ShipStates::LEAVING);
    system("cls");
}

CanonType Harbor::convertToCanon(const std::string &string)
{
    if (string == "small canon")
        return CanonType::SMALL;
    else if (string == "medium canon")
        return CanonType::MEDIUM;
    else if (string == "large canon")
        return CanonType::LARGE;
    else
        throw std::invalid_argument("no sting matches a canon");
}
