#pragma once

#include <iostream>

#define DEBUG ej::Debug::instance()
#define LOG(MSGS)   do { DEBUG << DEBUG.getLogPrefix() << MSGS << "\n"; } while(0)
#define WARN(MSGS)  do { DEBUG << DEBUG.getWarnPrefix() << MSGS << "\n"; } while (0)
#define ERROR(MSGS) do { DEBUG << DEBUG.getErrorPrefix() << MSGS << "\n"; exit(0); } while(0)

namespace ej {

class Debug {
  public:
    Debug(const Debug&) = delete;
    Debug& operator=(const Debug &) = delete;
    Debug(Debug &&) = delete;
    Debug& operator=(Debug &&) = delete;
    static auto& instance(){
        static Debug debug;
        return debug;
    }

    template<class T>
    Debug &operator<<(const T &msg) {
      std::cout << msg;
      return *this;
    }

    std::string getLogPrefix();
    std::string getWarnPrefix();
    std::string getErrorPrefix();

  private:
    Debug() {};
};

}

