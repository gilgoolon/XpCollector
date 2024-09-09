#pragma once
#include "ILogger.h"

class ConsoleLogger :
    public ILogger
{
public:
    virtual void log(const std::string& message) override;
};

