//
// Created by Niels on 11/12/21.
//

#ifndef MERCHANTSHIPS_HARBOR_H
#define MERCHANTSHIPS_HARBOR_H


#include <string>

class Harbor
{
    int id;
    std::string name;
public:
    Harbor(int id, const std::string& name);
};


#endif //MERCHANTSHIPS_HARBOR_H
