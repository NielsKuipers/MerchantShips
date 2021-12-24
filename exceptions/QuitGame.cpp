//
// Created by Niels on 24/12/21.
//

#include "QuitGame.h"

QuitGame::QuitGame(int errorCode, std::string message) noexcept: errorCode(errorCode), m_message(std::move(message))
{
    system("cls");
    std::cout << this->m_message << std::endl;
}

const char *QuitGame::what() const noexcept
{
    return this->m_message.c_str();
}
