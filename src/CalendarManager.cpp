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
    getUpcommingEvents(2);
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
    user_calendar = jsConfig.stringOf("USER_CALENDAR");

    std::cout << user_calendar << std::endl;

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

void CalendarManager::getUpcommingEvents(int maxEvents){   
    //Cleaning rest
    rest.ClearAllQueryParams();
    //Setting rest
    std::cout << "Asking for Calendars.. \n";
    CkStringBuilder sbJson;
    const std::string url = "/calendar/v3/calendars/"+user_calendar+"/events";

    //Get current time for setting a minDate for Events
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream current_date;
    current_date << std::put_time(&tm,"%Y-%m-%dT%H:%M:%SZ");
    std::cout << "Date: " << current_date.str() << "\n";

    rest.AddQueryParam("maxResults",std::to_string(maxEvents).c_str());
    rest.AddQueryParam("timeMin",current_date.str().c_str());
    rest.AddQueryParam("orderBy","startTime");
    rest.AddQueryParam("singleEvents","True");

    //Perform query
    bool success = rest.FullRequestNoBodySb("GET",url.c_str(),sbJson);

    if (success!=true){
        std::cout << rest.lastErrorText() << std::endl;
        exit(-1);
    }

    if (rest.get_ResponseStatusCode() == 401){
        std::cout << "Getting refresh token \n";
        success = refreshToken();
        if (success!=true){
            exit(-1);
        }

        getUpcommingEvents(maxEvents);
        return;
    }

    else if (rest.get_ResponseStatusCode() != 200) {
        std::cout << "Received error response code: " << rest.get_ResponseStatusCode() << "\r\n";
        std::cout << "Response body:" << "\r\n";
        std::cout << sbJson.getAsString() << "\r\n";
        exit(-1);
    }

    //Get json params
    CkJsonObject json;
    json.LoadSb(sbJson);

    //Parses the json response
    int i;
    int count_i;
    const char* id;
    const char* startDateTime;
    const char* endDateTime;
    const char* summary;
    const char* created;
    count_i = json.SizeOfArray("items");
    std::cout << "Printing events.. \n";
    while (i<count_i){
        std::cout << "------------------\n";
        json.put_I(i);
        id = json.stringOf("items[i].id");
        summary = json.stringOf("items[i].summary");
        startDateTime = json.stringOf("items[i].start.dateTime");
        endDateTime = json.stringOf("items[i].end.dateTime");
        created = json.stringOf("items[i].created");

        if (summary != 0){
            std::cout << summary << "\n";
        }
        if (startDateTime!=0){
            std::cout << "Start date: " << startDateTime << "\n"; 
        }
        if (endDateTime != 0){
            std::cout << "End date: " << endDateTime << "\n";
        }
        std::cout << "(" << created << ")\n";
        
        i++;
    }
}