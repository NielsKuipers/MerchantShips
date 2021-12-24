//
// Created by Niels on 24/12/21.
//

#ifndef MERCHANTSHIPS_QUITGAME_H
#define MERCHANTSHIPS_QUITGAME_H


#include <exception>
#include <string>
#include <iostream>
#include <utility>

class QuitGame : public std::exception
{
    int errorCode;
    std::string m_message;
public:
    QuitGame(int errorCode, std::string message) noexcept;
    ~QuitGame() override = default;
    const char* what() const noexcept override;
};


#endif //MERCHANTSHIPS_QUITGAME_H
