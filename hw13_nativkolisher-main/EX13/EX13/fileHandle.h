#include <string>
#include <iostream>
#include <iterator>
#include <queue>
#include <thread>
#include <fstream>

//make sure that 2 users will not write or read from file together
std::mutex sync;

/*
return the lines from file in one line
*/
std::string readAdminFile(std::string file_name)
{
    sync.lock();
    std::string line = "";
    std::string text = "";
    std::ifstream myfile(file_name);
    if (myfile.is_open())
    {
        while (getline(myfile, line))        //gets every line in file
        {
            text += line;        //add to text
        }
        myfile.close();
    }
    sync.unlock();
    return text;
}


/*
write string into file (also create file if not exist)
*/
void writeMessagesToUsersFile(std::string file_name, std::string message)
{
    sync.lock();
    std::ofstream outputfile;
    if (std::ifstream(file_name))       //check if file exists
    {
        outputfile.open(file_name, std::fstream::app);
        if (outputfile.is_open())
        {
            outputfile << message << "\n";
        }
        else std::cout << "Unable to open output file";
    }
    else
    {
        std::cout << "CREATE FILE.." << std::endl;
        std::ofstream outputfile2(file_name);       //create new file
        if (outputfile2.is_open())
        {
            outputfile2 << message << "\n";
        }
        else std::cout << "Unable to open output file";
    }
    sync.unlock();
}