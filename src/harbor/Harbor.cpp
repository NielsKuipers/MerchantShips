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

    if (key == KEY_SPACE && currentState != HarborStates::TRADING)
    {
        setState(posY);
        menuHandler::resetCursor();
    }

    switch (currentState)
    {
        case HarborStates::GOODS:
            displayShop();
            break;
        case HarborStates::CANONS:
            break;
        case HarborStates::SHIPS:
            break;
        case HarborStates::LEAVING:
            break;
        case HarborStates::REPAIRING:
            break;
        case HarborStates::QUIT:
            break;
        case HarborStates::MENU:
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
            {
                displayMenu();
                setState(HarborStates::MENU);
            }
            break;
    }
}

void Harbor::displayMenu()
{
    system("cls");
    minLine = 0;
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
    menuHandler::resetCursor();
    showOptions();
}

void Harbor::generateGoods()
{
    const auto goodsData{DB::selectData(
            "SELECT g2.goed, g.min_goed, g.max_goed, g.min_prijs, g.max_prijs "
            "FROM havens_goederen g INNER JOIN goederen g2 on g.goed_id = g2.id "
            "WHERE g.haven_id = " + std::to_string(id))};

    for (const auto &productData: goodsData)
    {
        const int amount{RNG::generateRandomNumber(std::stoi(productData[1]), std::stoi(productData[2]))};
        const int price{RNG::generateRandomNumber(std::stoi(productData[3]), std::stoi(productData[4]))};
        std::tuple<std::string, int, int> product{productData[0], amount, price};

        goods.emplace_back(product);
    }
}

void Harbor::displayShop()
{
    system("cls");
    options.clear();
    menuHandler::resetCursor();

    //display information and set the minline to 5 so it skips these in selection
    std::cout << "Welcome to the " << name << " market" << std::endl
              << "You can buy items by pressing B, and sell them by pressing S" << std::endl << std::endl
              << "You currently have " << ship.getGold() << " gold" << " and room for " << ship.getCargoSpace()
              << " goods" << std::endl
              << std::endl;
    minLine = 5;


    for (const auto &product: goods)
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
    auto &item = goods[y];
    int gold{ship.getGold()};
    int space{ship.getCargoSpace()};
    int amount;

    system("cls");
    std::cout << "Ah, you want to buy " + std::get<0>(item) << "?" << std::endl
              << "This will cost you " + std::to_string(std::get<2>(item)) << " gold each" << std::endl << std::endl
              << "How many do you wish to buy?" << std::endl
              << "Amount to buy: ";
    amount = menuHandler::getInput();

    int totalCost = amount * std::get<2>(item);
    while (totalCost > gold || amount > space)
    {
        if (amount > space)
            std::cout << "Your ship does not have enough room for this purchase, try again.";
        else
            std::cout << "You do not have enough gold for this purchase, try again.";

        amount = menuHandler::getInput();
        totalCost = amount * std::get<2>(item);
    }

    if (amount != 0)
    {
        ship.boughtItem(std::get<0>(item), amount, totalCost);
        std::get<1>(item) -= amount;
        std::cout << std::endl << "You bought " << amount << " " << std::get<0>(item)
                  << " for a total of " << totalCost << " gold" << std::endl << std::endl;
        system("pause");
    }
}

void Harbor::sellItem(int y)
{
    auto &item = goods[y];
    int has{ship.getItemAmount(std::get<0>(item))};
    int amount;

    system("cls");
    std::cout << "Yeah, we buy " + std::get<0>(item) << std::endl << "How many do you wish to sell?" << std::endl
              << std::endl << "Amount to sell: ";
    amount = menuHandler::getInput();

    int totalEarned = amount * std::get<2>(item);
    while (amount > has)
    {
        std::cout << "Are you trying to cheat me? you only have " << has << "of this item" << std::endl;
        amount = menuHandler::getInput();
        totalEarned = amount * std::get<2>(item);
    }

    if (amount != 0)
    {
        ship.soldItem(std::get<0>(item), amount, totalEarned);

        std::get<1>(item) += 1;
        std::cout << std::endl << "You sold " << amount << " " << std::get<0>(item)
                  << " for a total of " << totalEarned << " gold" << std::endl << std::endl;
        system("pause");
    }
}
