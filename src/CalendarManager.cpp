#include "headers/CalendarManager.h"

CalendarManager::CalendarManager(const std::string &config_path){
    LoadConfig(config_path);
}

void CalendarManager::LoadConfig(const std::string &path){
    std::cout << "Loading config file... \n";

    std::ifstream configFile;
    configFile.open(path);

    if (configFile.is_open()){
        std::string buffer;
        std::string key;
        std::string value;

        while(configFile){
            configFile >> key >> value;
            config[key] = value;
        }

        configFile.close();
    }
}
