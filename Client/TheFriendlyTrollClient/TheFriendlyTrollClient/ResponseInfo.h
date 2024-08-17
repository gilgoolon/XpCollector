#pragma once
#include "JSON.h"

class ResponseInfo
{
public:
	ResponseInfo(unsigned int status, std::string body);

	unsigned int get_status() const;

	json get_body() const;

private:
	unsigned int m_status; // I.E. 200
	std::string m_body;
};

