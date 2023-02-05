#include <iostream>
#include "headers/CalendarManager.h"

int main(){

    //CalendarManager cm = CalendarManager("./config/config.json");
    CalendarManager cm = CalendarManager("./../config/config.json");
    cm.getUpcommingEvents(3);
    return 0;
}