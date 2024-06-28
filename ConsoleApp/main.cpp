#include <iostream>

#include "../SNWK_Core/include/project_utils.h"
#include "../SNWK_Core/include/snwk_lib.h"
#include "../SNWK_Core/include/app_elements.h"

int main ()
{
    String savepath = "/Users/antoandimitrov/Documents/Code/UniCoursework/Coursework-OOP-23-24/InstallationDir";  
    Network::SetGlobalSavePath(savepath);
    
    Network& currentNetwork = Network::GetNetwork();

    currentNetwork.LoadNetwork("boyan");
    
    currentNetwork.GetUserByID(2)->printinfo(std::cout);
    std::cout << '\n';
    /*
    std::cout << "Users: " << currentNetwork.GetCurrentTrueUserCount() << '\n';
    User boyan("skrypstera", "123456", currentNetwork.GetCurrentTrueUserCount() + 1, 0);
    currentNetwork.RegisterUser(boyan);
    */

    std::cout << "great success\n";

    return 0;
}