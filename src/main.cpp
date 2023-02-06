#include <iostream>
#include "headers/CalendarManager.h"

int main(){
    CalendarManager cm{"./../config/config.json"};
    cm.getUpcommingEvents(3);
    return 0;
}