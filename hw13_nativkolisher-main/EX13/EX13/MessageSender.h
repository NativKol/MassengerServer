#pragma once

#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>
#include <iterator>

enum options {
    zero,
    sign_in,
    sign_out,
    account_print,
    EXIT
};

class MessageSender 
{
public:
    MessageSender();
    ~MessageSender();
    void SignIn(std::string name);
    void SignOut(std::string name);
    void ConnectedUsers();
    void ShowMenu();

    std::string getAllUsers();

    std::unordered_set <std::string> getNames();

private:
    std::unordered_set <std::string> _names;
};