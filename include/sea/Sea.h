//
// Created by Niels on 24/12/21.
//

#ifndef MERCHANTSHIPS_SEA_H
#define MERCHANTSHIPS_SEA_H


#include "interface/Place.h"

class Sea : public Place
{
    void handleInput(int key) override;

public:
    Sea() = default;
};


#endif //MERCHANTSHIPS_SEA_H
