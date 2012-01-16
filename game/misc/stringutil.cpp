#include "stringutil.h"

std::vector<std::string> split(std::string line, char c)
{
	if (line.find_first_of(";#") != std::string::npos)
		line = line.substr(0, line.find_first_of(";#"));

	std::vector<std::string> items;
	size_t index = 0;
	do 
	{
		index = line.find_first_not_of(c, index);
		if (index == std::string::npos)
			break;

		size_t start = index;
		index = line.find_first_of(c, start);
		items.push_back(line.substr(start, index - start));
	}
	while (index != std::string::npos);

	return items;
}

