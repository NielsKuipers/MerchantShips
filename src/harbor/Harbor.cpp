//
// Created by Niels on 11/12/21.
//

#include "harbor/Harbor.h"

Harbor::Harbor(int id, const std::string &name)
{
    this->id = id;
    this->name = name;

    menu =
            {
                    "1. Trade goods",
                    "2. Trade canons",
                    "3. Trade ships",
                    "4. Leave harbor",
                    "5. Repair ship",
                    "6. Quit game"
            };

    this->setOptions(menu);
    generateGoods();
}

void Harbor::handleInput(int key)
{
    int menuPos = menuHandler::handleInput(key, options.size());

    if (key == KEY_SPACE)
        setState(menuPos);
    else
        menuHandler::setCursor(options[menuPos]);

    switch (currentState)
    {
        case HarborStates::TRADING:
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
    }

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

    for (const auto &product: goods)
    {
        const std::string line{
                std::get<0>(product) + " amount available: " +
                std::to_string(std::get<1>(product)) + " price: " +
                std::to_string(std::get<2>(product))
        };

        options.emplace_back(line);
    }

    this->showOptions();
    setState(6);
}

void Harbor::setState(int number)
{
    currentState = HarborStates(number);
}
