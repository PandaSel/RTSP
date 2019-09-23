#pragma once

#include <regex>
#include <exception>
#include <vector>
#include <string>

std::vector<std::string> regexsplit(const std::string& input, const char* c)
{
	std::regex re(c);

	std::sregex_token_iterator p(input.begin(), input.end(), re, -1);
	std::sregex_token_iterator end;
	std::vector<std::string> vec;
	while (p != end)
		vec.emplace_back(*p++);

	return vec;
}

template <class Type>
Type string2N(const std::string& str)
{
	std::istringstream iss(str);
	Type N;
	iss >> N;
	return N;
}