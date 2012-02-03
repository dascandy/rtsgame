#ifndef ILISTENER_H
#define ILISTENER_H

template <class T>
class IListener
{
public:
	virtual void OnChange(T *button) = 0;
};

#endif

