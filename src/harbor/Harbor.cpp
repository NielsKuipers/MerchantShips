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
}

void Harbor::handleInput(int key)
{
    std::tuple<int, int> menuPos;
    int posY;

    menuPos = menuHandler::handleInput(options, key, minLine, options.size() + minLine);
    posY = std::get<1>(menuPos);

    if ((currentState != HarborStates::TRADING && currentState != HarborStates::TRADING_SHIP) && key == KEY_SPACE)
    {
        setState(posY);
        menuHandler::resetCursor();
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
        case HarborStates::LEAVING:
            break;
        case HarborStates::REPAIRING:
            break;
        case HarborStates::QUIT:
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
    }
}

void Harbor::displayMenu()
{
    system("cls");
    minLine = ship.displayShipInfo();
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
    menuHandler::resetCursor();
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
    menuHandler::resetCursor();
    const auto &items{(currentShop == HarborStates::GOODS) ? goods : canons};

    //display information and set the minline to 5 so it skips these in selection
    std::cout << "Welcome to the " << name << " market" << std::endl
              << "You can buy items by pressing B, and sell them by pressing S" << std::endl << std::endl
              << "You currently have " << ship.getGold() << " gold" << " and room for " << ship.getCargoSpace()
              << " goods" << std::endl
              << std::endl;
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
    std::cout << "Ah, you want to buy " + std::get<0>(item) << "?" << std::endl
              << "This will cost you " + std::to_string(std::get<2>(item)) << " gold each" << std::endl << std::endl
              << "How many do you wish to buy?" << std::endl
              << "Amount to buy: ";
    amount = menuHandler::getNumberInput();

    int totalCost = amount * std::get<2>(item);
    while (totalCost > gold || amount > space || amount > std::get<1>(item))
    {
        if (amount > space)
            std::cout << std::endl << "Your ship does not have enough room for this purchase, try again." << std::endl;
        else if (amount > std::get<1>(item))
            std::cout << "We only have " << std::get<1>(item) << " of that item, please try again." << std::endl;
        else
            std::cout << std::endl << "You do not have enough gold for this purchase, try again." << std::endl;

        std::cout << "Amount to buy: ";
        amount = menuHandler::getNumberInput();
        totalCost = amount * std::get<2>(item);
    }

    if (amount != 0)
    {
        currentShop == HarborStates::GOODS ? ship.boughtItem(std::get<0>(item), amount, totalCost)
                                           : ship.boughtCanon(std::get<0>(item), amount, totalCost);
        std::get<1>(item) -= amount;
        std::cout << std::endl << "You bought " << amount << " " << std::get<0>(item)
                  << " for a total of " << totalCost << " gold" << std::endl << std::endl;
        system("pause");
    }
}

void Harbor::sellItem(int y)
{
    auto &item{currentShop == HarborStates::GOODS ? goods[y] : canons[y]};
    int has{ship.getItemAmount(std::get<0>(item))};
    int amount;

    system("cls");
    std::cout << "Yeah, we buy " + std::get<0>(item) << std::endl << "How many do you wish to sell?" << std::endl
              << std::endl << "Amount to sell: ";
    amount = menuHandler::getNumberInput();

    int totalEarned = amount * std::get<2>(item);
    while (amount > has)
    {
        std::cout << std::endl << "Are you trying to cheat me? you only have " << has << " of this item" << std::endl;
        std::cout << "Amount to sell: ";
        amount = menuHandler::getNumberInput();
        totalEarned = amount * std::get<2>(item);
    }

    if (amount != 0)
    {
        currentShop == HarborStates::GOODS ? ship.soldItem(std::get<0>(item), amount, totalEarned)
                                           : ship.soldCanon(std::get<0>(item), amount, totalEarned);

        std::get<1>(item) += 1;
        std::cout << std::endl << "You sold " << amount << " " << std::get<0>(item)
                  << " for a total of " << totalEarned << " gold" << std::endl << std::endl;
        system("pause");
    }
}

void Harbor::displayShips()
{
    system("cls");
    options.clear();
    menuHandler::resetCursor();

    std::cout << "Want to buy a new ship, eh?" << std::endl
              << "Here is our selection" << std::endl
              << std::endl << "Buy a ship with spacebar."
              << std::endl << "Buying a ship will automatically sell you current one for half the original price"
              << std::endl << "Selling your current ship will earn you " << ship.getPrice() / 2 << " gold."
              << std::endl << std::endl;
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
        std::cout << "You do not have enough gold for this ship." << std::endl << std::endl;
        system("pause");
        return;
    }

    std::cout << "Ah, the " << std::get<0>(item) << "? A fine choice." << std::endl
              << "This will cost you " << std::get<1>(item) << " gold" << std::endl
              << "This will exchange your current ship for the new one, are you sure?" << std::endl
              << std::endl;

    if (ship.getTotalCanons() > std::get<3>(item))
        std::cout
                << "You have more canons than you have room for on the new ship, you will lose some of them"
                << std::endl << std::endl;
    if (ship.getTotalCargo() > std::get<2>(item))
        std::cout << "You have more goods than you have room for on the new ship, you will lose some of them"
                  << std::endl << std::endl;

    auto result{menuHandler::getConfirmInput()};

    if (result == "y")
    {
        ship.changeShip(item);
        std::cout << std::endl << "You have successfully bought the " << std::get<0>(item) << "!" << std::endl << std::endl;
        system("pause");
    } else
        return;
}
