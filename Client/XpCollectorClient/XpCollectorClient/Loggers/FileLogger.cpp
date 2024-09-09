#include <iostream>

#include "FileLogger.h"

FileLogger::FileLogger(const std::string& log_folder)
	: m_cout_buff(log_folder + "\\output.txt")
	, m_cerr_buff(log_folder + "\\error.txt")
{
	m_cout_old_buff = std::cout.rdbuf(); //save old buf
	std::cout.rdbuf(m_cout_buff.rdbuf()); //redirect std::cout to out.txt!

	m_cerr_old_buff = std::cerr.rdbuf(); //save old buf
	std::cerr.rdbuf(m_cerr_buff.rdbuf()); //redirect std::cout to out.txt!
}

FileLogger::~FileLogger()
{
	std::cout.rdbuf(m_cout_old_buff);
	std::cerr.rdbuf(m_cerr_old_buff);
}

void FileLogger::log(const std::string& message)
{
	std::cout << message << std::endl;
}
