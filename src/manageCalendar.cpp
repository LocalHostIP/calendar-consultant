#include <map>
#include <iostream>
#include <fstream>

class CalendarManager{
    std::map <std::string,std::string> config;

    void LoadConfig(const std::string &path){
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

public:
    CalendarManager(const std::string &config_path){
        LoadConfig(config_path);
        std::cout << config["CLIENT_ID"] << std::endl;
    }
};