#pragma once
#include <fstream>

#include "ILogger.h"

namespace xp_collector {

class FileLogger :
    public ILogger
{
public:
    FileLogger(const std::string& log_folder);

    ~FileLogger();

    virtual void log(const std::string& message) override;

private:
    std::ofstream m_cout_buff;
    std::streambuf* m_cout_old_buff;
    std::ofstream m_cerr_buff;
    std::streambuf* m_cerr_old_buff;
};

}
