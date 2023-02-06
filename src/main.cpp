#include <iostream>
#include "headers/CalendarManager.h"
#include "headers/SerialCom.h"

int main(){
    //CalendarManager cm{"./../config/config.json"};
    //cm.getUpcommingEvents(3);
    ///dev/ttyACM0
    SerialCom serial("dev/ttyACM0");
    serial.setReadConfig(2,100);

    while (true){
        std::cout << serial.readData() << "\n\n";
    }
    serial.closeConnection();
    return 0;
}