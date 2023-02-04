#include <iostream>
#include <map>
#include "../include/chilkat/CkRest.h"
#include "../include/chilkat/CkOAuth2.h"
#include "../include/chilkat/CkStringBuilder.h"
#include "../include/chilkat/CkJsonObject.h"
#include "headers/CalendarManager.h"


std::string refreshToken(CkRest &rest){
    /*Get refresh token*/
    CkStringBuilder sbJson;
    //rest.AddQueryParam("grant_type","refresh_token");
    //rest.AddQueryParam("refresh_token",REFRESH_TOKEN);
    //rest.AddQueryParam("client_secret",CLIENT_SECRET);
    //rest.AddQueryParam("client_id",CLIENT_ID);

    bool success = rest.FullRequestNoBodySb("POST","/oauth2/v4/token",sbJson);
    
    if (success!=true){
        std::cout << rest.lastErrorText() << std::endl;
        exit(-1);
    }

    if (rest.get_ResponseStatusCode() != 200) {
        std::cout << "Received error response code: " << rest.get_ResponseStatusCode() << "\r\n";
        std::cout << "Response body:" << "\r\n";
        std::cout << sbJson.getAsString() << "\r\n";
        exit(-1);
    }

    std::cout << sbJson.getAsString() << "\r\n";

    CkJsonObject json;
    json.LoadSb(sbJson);

    //Parses the JSON response
    
    return "No token";
}

void getCalendars(CkRest &rest){
    CkStringBuilder sbJson;
    bool success = rest.FullRequestNoBodySb("GET","/calendar/v3/calendars/miguel.gro.padilla@gmail.com/events",sbJson);
    if (success!=true){
        std::cout << rest.lastErrorText() << std::endl;
        exit(-1);
    }

    if (rest.get_ResponseStatusCode() != 200) {
        std::cout << "Received error response code: " << rest.get_ResponseStatusCode() << "\r\n";
        std::cout << "Response body:" << "\r\n";
        std::cout << sbJson.getAsString() << "\r\n";
        exit(-1);
    }
    
    std::cout << sbJson.getAsString() << "\r\n";
    std::cout << "End"<<std::endl;
}

void connect(){
    CkRest rest; 
    bool success;

    //Provide OAuth2 acces token
    CkOAuth2 oauth2;
    //oauth2.put_ClientId(CLIENT_ID);
    //oauth2.put_ClientSecret(CLIENT_SECRET);
    //oauth2.put_AccessToken(ACCESS_TOKEN);
    //oauth2.put_RefreshToken(ACCESS_TOKEN);
    
    rest.SetAuthOAuth2(oauth2);
    
    success = rest.Connect("www.googleapis.com",443,true,true);
    if  (success!=true){
        std::cout << rest.lastErrorText() << std::endl;
        return exit(-1);
    }

    refreshToken(rest);
}

int main(){

    CalendarManager cm = CalendarManager("./config/config.json");
    //CalendarManager cm = CalendarManager("./../config/config.json");

    return 0;
}