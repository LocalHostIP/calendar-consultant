#include <iostream>
#include <vector>
#include <thread>
#include <exception>
#include <chrono>
#include <filesystem>

#include "headers/CalendarManager.h"
#include "headers/CalendarEvent.h"
#include "headers/SerialCom.h"

using namespace std;

#define MAX_EVENTS 5
#define MAX_LENGTH_BUFFER_SEND 300
#define UPDATE_TIME 10
#define READ_LENGTH_BUFFER 2
#define GET_EVENTS 'E'
#define DELETE_EVENT 'D'

void makeSendBuffer(const std::vector<CalendarEvent> &events, std::string &send_buffer){
    using namespace std;

    send_buffer = "";
    for (auto e:events){
        send_buffer += e.getSummary()+"\t"+ e.getTimeStartDate() + " " + e.getTimeStartHour()+"\t";
    }
    if (send_buffer.length()>MAX_LENGTH_BUFFER_SEND){
        send_buffer=send_buffer.substr(0,MAX_LENGTH_BUFFER_SEND-1);
    } 
    cout << "Data to send: " << send_buffer << "\n";
}

std::string SelectPort(){
    namespace fs = std::filesystem;
    using namespace std;
    
    vector <string> ports;
    string path = "/dev";
    
    int index=0;

    cout << "Listing valid ports...\n";
    for (const auto & entry : fs::directory_iterator(path)){
        //Select all files that start with /dev/ttyA (STM32 Development board)
        if(entry.path().string().substr(0,9)=="/dev/ttyA"){
            ports.push_back(entry.path().string());
            cout << "("<<index<<") "<< entry.path() << std::endl;
        }
    }

    uint32_t selected_port=0;

    if (ports.size()==0){
        cout << "No valid ports found \n";
        throw std::runtime_error("No valid ports found");
    }

    cout << "Select a port by index: ";
    cin >> selected_port;

    if (selected_port>=ports.size()){
        cout << "No valid index selectd \n";
        throw std::runtime_error("No valid index port selected");
    }

    return ports.at(selected_port);
}

int main(){

    std::string port = SelectPort();

    //Check UPcomming events from Calendar
    CalendarManager cm{"./../config/config.json"};
    //Get events
    vector<CalendarEvent> events = cm.getUpcommingEvents(MAX_EVENTS);
    vector<CalendarEvent> last_events_sent;

    //Make buffer
    string send_buffer;
    makeSendBuffer(events,send_buffer);

    auto time_lastUpdate = chrono::steady_clock::now();

    //Send Events to Microcontroller
    ///dev/ttyACM0
    int const timeout=1;
    int const maxChar=2;
    auto ctime = chrono::steady_clock::now();
    int passed_time;
    const int next_time = UPDATE_TIME;
    int index_delete;

    try{
        SerialCom serial("dev/ttyACM1");

        //Set Config to Wait until receibe one byte
        serial.setReadConfig(maxChar,timeout);

        while(true){
            //Wait until recibe data from Micro
            cout << "Waiting for data from STM32... \n";
            

            /* ------------ Receive petition ---------------*/
            string r_data = serial.readData();

            if (r_data.length()>1){
                cout << "Recibed data: " << r_data << "\n";

                if (r_data[0]==GET_EVENTS){
                    //Send Data
                    cout << "Sending events \n";
                    serial.writeData(send_buffer);
                    last_events_sent = events;
                }

                if (r_data[0]==DELETE_EVENT){
                    //Delete indicated event index
                    cout << "deleting event (" << r_data[1] << ")... \n";
                    index_delete=r_data[1]-'0';
                    cm.deleteEvent(last_events_sent[index_delete]);
                    //Update Events
                    cout << "\nGetting new upcoming events\n";
                    events = cm.getUpcommingEvents(MAX_EVENTS);
                    makeSendBuffer(events,send_buffer);
                    time_lastUpdate = chrono::steady_clock::now();
                }

            }

            /*-------------- Update Events timmer --------------*/

            ctime = std::chrono::steady_clock::now();
            passed_time = chrono::duration_cast<chrono::seconds>(ctime - time_lastUpdate).count();
            //If enough time has passed then update events
            if (passed_time >= next_time){
                cout << "\nGetting new upcoming events\n";
                events = cm.getUpcommingEvents(MAX_EVENTS);
                makeSendBuffer(events,send_buffer);
                time_lastUpdate = chrono::steady_clock::now();
            }
        }
        
        serial.closeConnection();

    }catch(const std::runtime_error &e){
        std::cout << e.what() << std::endl;
    }
    return 0;
} 
