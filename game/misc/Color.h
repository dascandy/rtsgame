#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
	float raw[4];
	float &r, &g, &b, &a;
	Color(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f) 
		: r(raw[0])
		, g(raw[1])
		, b(raw[2])
		, a(raw[3])
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	Color(const Color &rhs) 
		: r(raw[0])
		, g(raw[1])
		, b(raw[2])
		, a(raw[3])
	{
		raw[0] = rhs.raw[0];
		raw[1] = rhs.raw[1];
		raw[2] = rhs.raw[2];
		raw[3] = rhs.raw[3];
	}
	Color(int color)
		: r(raw[0])
		, g(raw[1])
		, b(raw[2])
		, a(raw[3])
	{
		r = ((color >> 0) & 0xFF) / 255.0f;
		g = ((color >> 8) & 0xFF) / 255.0f;
		b = ((color >> 16) & 0xFF) / 255.0f;
		a = ((color >> 24) & 0xFF) / 255.0f;
	}
	Color &operator=(const Color &rhs) 
	{
		raw[0] = rhs.raw[0];
		raw[1] = rhs.raw[1];
		raw[2] = rhs.raw[2];
		raw[3] = rhs.raw[3];
		return *this;
	}
	Color operator*(double value) 
	{
		return Color(r * (float)value, g * (float)value, b * (float)value, a * (float)value);
	}
};

Color operator+(const Color &a, const Color &b);

#endif

