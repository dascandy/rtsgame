#ifndef SDL_ERROR_H
#define SDL_ERROR_H

#include <exception>

class sdl_error : public std::exception
{
private:
	std::string error;
public:
	sdl_error(std::string error) : error(error)
	{
	}
	virtual ~sdl_error() throw()
	{
	}
	friend std::ostream &operator<<(std::ostream &os, const sdl_error &err);
};

#endif

