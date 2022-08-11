#pragma once

namespace ej {

class System {
  public:
    virtual void load() = 0;
    virtual void shutdown() = 0;
};

}
