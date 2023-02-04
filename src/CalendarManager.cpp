#include "headers/CalendarManager.h"

CalendarManager::CalendarManager(const std::string &config_path){
    this->config_path = config_path;
    bool success = loadConfig();
    if (success!=true){
        std::cout << "Failed to load "<< config_path << "\r\n";
        exit(-1);
    }
    //Setting tokens
    oauth2.put_TokenEndpoint("https://www.googleapis.com/oauth2/v4/token");

    //Connect to API
    rest.SetAuthOAuth2(oauth2);
    
    success = rest.Connect("www.googleapis.com",443,true,true);
    if  (success!=true){
        std::cout << rest.lastErrorText() << std::endl;
        exit(-1);
    }

    //Get Calendars
    getCalendars();
}

bool CalendarManager::loadConfig(){
    bool success = jsConfig.LoadFile(config_path.c_str());

    if (success != true) {
        return false;
    }

    oauth2.put_AccessToken(jsConfig.stringOf("ACCESS_TOKEN"));
    oauth2.put_ClientId(jsConfig.stringOf("CLIENT_ID"));
    oauth2.put_ClientSecret(jsConfig.stringOf("CLIENT_SECRET"));
    oauth2.put_RefreshToken(jsConfig.stringOf("REFRESH_TOKEN"));

    return true;
}

bool CalendarManager::refreshToken(){

    //Getting refresh token
    bool success = oauth2.RefreshAccessToken();
        
    if (success!=true){
        std::cout << oauth2.lastErrorText() << std::endl;
        exit(-1);
    }

    std::cout << "New token " << oauth2.accessToken() << std::endl;

    jsConfig.UpdateString("ACCESS_TOKEN",oauth2.accessToken());

    // Save the new JSON access token response to a file.
    CkStringBuilder sbJson;
    jsConfig.put_EmitCompact(false);
    jsConfig.EmitSb(sbJson);
    sbJson.WriteFile(config_path.c_str(),"utf-8",false);

    return success;
}

void CalendarManager::getCalendars(){
    std::cout << "Asking for Calendars.. \n";
    CkStringBuilder sbJson;
    bool success = rest.FullRequestNoBodySb("GET","/calendar/v3/calendars/miguel.gro.padilla@gmail.com/events",sbJson);

    if (success!=true){
        std::cout << rest.lastErrorText() << std::endl;
        exit(-1);
    }

    std::cout << rest.get_ResponseStatusCode() << std::endl;

    if (rest.get_ResponseStatusCode() == 401){
        std::cout << "Getting refresh token \n";
        success = refreshToken();
        if (success!=true){
            exit(-1);
        }

        getCalendars();
        return;
    }

    else if (rest.get_ResponseStatusCode() != 200) {
        std::cout << "Received error response code: " << rest.get_ResponseStatusCode() << "\r\n";
        std::cout << "Response body:" << "\r\n";
        std::cout << sbJson.getAsString() << "\r\n";
        exit(-1);
    }

    std::cout << sbJson.getAsString() << std::endl;
}