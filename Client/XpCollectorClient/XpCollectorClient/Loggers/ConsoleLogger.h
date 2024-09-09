#pragma once
#include "ILogger.h"

namespace xp_collector {

class ConsoleLogger :
    public ILogger
{
public:
    virtual void log(const std::string& message) override;
};

}
