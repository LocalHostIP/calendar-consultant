#ifndef SerialCom_H_
#define SerialCom_H_

// C library headers
#include <string.h>
#include <iostream>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#define SIZE_BUFFER 255

class SerialCom{
    int serial_port;
    struct termios tty;
    void setIniConfigPort();
    char read_buffer[SIZE_BUFFER];
    void updateConfig();
public:
    SerialCom(const std::string port);
    std::string readData();
    void setReadConfig(const int maxBytes,const int TimeOut);
    void closeConnection();
};

#endif