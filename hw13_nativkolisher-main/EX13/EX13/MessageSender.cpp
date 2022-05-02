#include "MessageSender.h"
#include <algorithm>

MessageSender::MessageSender()
{
    this->_names;
}

MessageSender::~MessageSender()
{
    this->_names.clear();
    delete this;
}

void MessageSender::SignIn(std::string name)
{
    if (!(this->_names.find(name) == this->_names.end()))
        std::cout << "Name already exist" << std::endl;

    this->_names.insert(name);
}

void MessageSender::SignOut(std::string name)
{
    if (!(this->_names.find(name) == this->_names.end()))
        this->_names.erase(name);
    else
        std::cout << "Name don't exist" << std::endl;
}

void MessageSender::ConnectedUsers()
{
    std::unordered_set<std::string> ::iterator itr;
    for (itr = this->_names.begin(); itr != this->_names.end(); itr++)
        std::cout << (*itr) << std::endl;
}

void MessageSender::ShowMenu()
{
    std::cout << "1. Sign In" << std::endl;
    std::cout << "2. Sign Out" << std::endl;
    std::cout << "3. Connected Users" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << std::endl;
}

std::string MessageSender::getAllUsers()
{
    std::string name = "";
    std::vector<std::string> vector_sort;
    std::copy(this->_names.begin(), this->_names.end(), std::back_inserter(vector_sort));

    std::sort(vector_sort.begin(), vector_sort.end());		//sort alphabetically
    for (auto it = std::begin(vector_sort); it != std::end(vector_sort); ++it) {
        name += *(it)+"&";
    }

    name.pop_back();        //remove last &
    return name;
}

std::unordered_set<std::string> MessageSender::getNames()
{
    return this->_names;
}
