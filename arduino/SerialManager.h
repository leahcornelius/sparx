#ifndef SERIAL_MANAGER
#define SERIAL_MANAGER

#include <Arduino.h>

namespace SM {
class SerialManager {
  public:
    SerialManager();

    void start();
    bool onReceive();
    void reset();
    void store();

    String command = "";
    String params[10];
    int paramCount = 0;
    String raw = "";
  private:
    int _position;
    char _char;
    char _flag      = '\n'; // you must set the serial monitor to include a newline with each command
    char _delimiter = ',';
    char _cmd_delimiter = ';';
    String _buffer  = "";

    String _cmd_buff = "";
    String _params_buff[10];
    int _paramCountBuff = 0;
};
}

#endif
