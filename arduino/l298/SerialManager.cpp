#include <Arduino.h>
#include "SerialManager.h"

namespace SM {
SerialManager::SerialManager() {}

void SerialManager::start() {
  Serial.begin(115200);
}

bool SerialManager::onReceive() {
  if (Serial.available()) {
    _char = char(Serial.read());
    
    if (_char == _flag) {
      if (_cmd_buff == "" and _buffer == "") {
        reset();
        return false;
      } else if (_cmd_buff == "") {
        _cmd_buff == _buffer;
      } 
      store();
      reset();
      return true;
    } else if (_char == _cmd_delimiter) {
      _cmd_buff = _buffer;
      _buffer = "";
      return false;
    } else if (_char == _delimiter) {
      if (_paramCountBuff == 10) {
        Serial.println("Too many params (Max: 10)");
        reset();
        return false;
      } else {
        _params_buff[_paramCountBuff] = _buffer;
        _buffer = "";
        _paramCountBuff += 1;
        return false;
      }
    } else {
      _buffer += _char;
      return false;
    }
  } else {
    return false;
  }
}

void SerialManager::store() {
  command = _cmd_buff;
  for (int i = 0; i < _paramCountBuff; i++) {
    params[i] = _params_buff[i];
  }
  paramCount = _paramCountBuff;
}

void SerialManager::reset() {
  _cmd_buff = "";
  for (int i = 0; i < 10; i++) {
    _params_buff[i] = "";
  }
  _paramCountBuff = 0;
  _char = 0x00;
  _buffer = "";
}
}
