#ifndef VAR_H
#define VAR_H

#include <math.h>

static const double PI = 3.141592653589793;

template <typename T>
class Var {
public:
	virtual T operator*() = 0;
	virtual Var<T> *simplify() { return this; }
};

template <typename T>
class VarNum : public Var<T> {
private:
	T val;
public:
	VarNum(T val) : val(val) {}
	T operator*() { return val; }
	VarNum<T> &operator=(T newVal) { val = newVal; return *this; }
};

template <typename T>
class VarCosInterp : public Var<T> {
private:
	Var<T> *a, *b;
	Var<int> *time;
	int interval;
public:
	VarCosInterp(Var<T> *a, Var<T> *b, Var<int> *time, int interval) 
	: a(a)
	, b(b)
	, time(time)
	, interval(interval)
	{
	}
	~VarCosInterp() {
		delete a;
		delete b;
	}
	T operator*() { 
		int timeoffs = (**time) % interval;
		T valA = **a, valB = **b;
		double interpval = 0.5f + 0.5f * cos(2 * PI * timeoffs / interval);
		return (T)(valA * interpval + valB * (1.0f - interpval));
	}
};

template <typename T>
class VarFade : public Var<T> {
private:
	Var<T> *a, *b;
	Var<int> *time;
	int duration;
	int startTime;
public:
	VarFade(Var<T> *a, Var<T> *b, Var<int> *time, int duration, int startTime) 
	: a(a)
	, b(b)
	, time(time)
	, duration(duration)
	, startTime(startTime) 
	{
	}
	~VarFade() {
		delete a;
		delete b;
	}
	T operator*() {
		int curtime = **time;
		if (curtime <= startTime) return **a;
		if (curtime >= startTime + duration) return **b;
		float interpval = 0.5f + 0.5f * cosf((float)PI * (curtime - startTime) / duration);
		return (T)(**a * interpval + **b * (1.0f - interpval));
	}
	Var<T> *simplify() {
		if (**time >= startTime + duration) {
			Var<T> *nb = b;
			b = 0;
			delete this;
			return nb;
		}
		return this; 
	}
};

template <typename T>
class VarFadeSharp : public Var<T> {
private:
	Var<T> *a, *b;
	Var<int> *time;
	int halftime;
	int startTime;
public:
	VarFadeSharp(Var<T> *a, Var<T> *b, Var<int> *time, int halftime, int startTime) 
	: a(a)
	, b(b)
	, time(time)
	, halftime(halftime)
	, startTime(startTime) 
	{
	}
	~VarFadeSharp() {
		delete a;
		delete b;
	}
	T operator*() {
		int curtime = **time;
		if (curtime <= startTime) return **a;
		if (curtime >= startTime + halftime * 10) return **b;
		float halflives = (curtime - startTime) / (float)halftime;
		float interpval = pow(0.5f, halflives);
		return (T)(**a * interpval + **b * (1.0f - interpval));
	}
	Var<T> *simplify() {
		if (**time >= startTime + halftime * 10) {
			Var<T> *nb = b;
			b = 0;
			delete this;
			return nb;
		}
		return this; 
	}
};

template <typename T>
Var<T> *fade(Var<T> *a, Var<T> *b, Var<int> *time, int duration, int startTime) {
	return new VarFade<T>(a, b, time, duration, startTime);
}

template <typename T>
Var<T> *fadesharp(Var<T> *a, Var<T> *b, Var<int> *time, int duration, int startTime) {
	return new VarFadeSharp<T>(a, b, time, duration, startTime);
}

template <typename T>
Var<T> *cosinterp(Var<T> *a, Var<T> *b, Var<int> *time, int interval) {
	return new VarCosInterp<T>(a, b, time, interval);
}

#endif


