//
// Created by Niels on 13/12/21.
//

#ifndef MERCHANTSHIPS_PLACE_H
#define MERCHANTSHIPS_PLACE_H

#include <utility>
#include <vector>
#include <string>
#include <iostream>

class Place
{
public:
    virtual void handleInput(int key) = 0;

    void showOptions()
    {
        for (const auto &option: options)
            std::cout << option << std::endl;
    };

    void setOptions(std::vector<std::string> menuOptions)
    { this->options = std::move(menuOptions); }

    const std::vector<std::string> &getOptions() const
    { return this->options; }

protected:
    std::vector<std::string> options{};
    int minLine{0};
};

#endif //MERCHANTSHIPS_PLACE_H
