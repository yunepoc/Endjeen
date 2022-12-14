#pragma once

#include <string>
#include <vector>

namespace ej {

class SystemMsg {
  public:
    SystemMsg(std::string system, std::string msg) : system(system), msg(msg) {};
    void addInt(int i);
    void addString(std::string s);
    int getInt(unsigned index);
    std::string getString(unsigned index);
    std::string getMsg();
    std::string getSystem();
    unsigned numInt();
    unsigned numString();
  private:
    std::string system;
    std::string msg;
    std::vector<int> ints;
    std::vector<std::string> strings;
};

class System {
  public:
    System() {
      systems.push_back(this);
    }
    virtual void load() = 0;
    virtual void shutdown() = 0;
    static void send(SystemMsg& msg) {
      for (auto system: systems)
        system->receive(msg);
    }

  protected:
    static std::vector<System*> systems;
    virtual void receive(SystemMsg& msg) {};
};

}
