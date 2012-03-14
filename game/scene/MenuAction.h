#ifndef MENUACTION_H
#define MENUACTION_H

class MenuAction {
public:
	virtual void operator()() = 0;
};

class SetMenuAction : public MenuAction {
	Menu *menu;
	MenuPage *page;
public:
	SetMenuAction(Menu *menu, MenuPage *page)
		: menu(menu)
		, page(page)
	{}
	void operator()() {
		menu->setCurrentPage(page);
	}
};

class ArbitraryAction : public MenuAction {
	class _wrapper { public: virtual void operator()() = 0; };
	class f : public _wrapper { 
		void (*func)();
	public:
		f(void (*func)()) : func(func) {}
		virtual void operator()() { func(); }
	};
	template <typename T, typename U>
	class wrapper : public _wrapper {
		T t;
		U u;
	public:
		wrapper(T t, U u) : t(t), u(u) {}
		void operator()() { (t->*u)(); }
	};
	template <typename T, typename U, typename V>
	class wrapper2 : public _wrapper {
		T t;
		U u;
		V v;
	public:
		wrapper2(T t, U u, V v) : t(t), u(u), v(v) {}
		void operator()() { (t->*u)(v); }
	};
	_wrapper *w;
public:
	ArbitraryAction(void (*func)())
		: w(new f(func))
	{}
	template <typename T, typename U>
	ArbitraryAction(T t, U u) 
		: w(new wrapper<T, U>(t, u))
	{}
	template <typename T, typename U, typename V>
	ArbitraryAction(T t, U u, V v)
		: w(new wrapper2<T, U, V>(t, u, v))
	{}
	void operator()() {
		(*w)();
	}
};

#endif

