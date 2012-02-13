#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

class Resource {
public:
	virtual bool isLoaded() = 0;
};

template <typename T>
class Res {
	struct Handle {
		Handle(T *obj) : obj(obj), usageCount(1) {}
		~Handle() { delete obj; }
		T *obj;
		int usageCount;
	};
	friend class ResourceManager;
	Res(T *obj) : h(new Handle(obj)) { }
	Handle *h;
public:
	Res(const Res<T> &obj) : h(h) { h->usageCount++; }
	~Res() { h->usageCount--; if (h->usageCount == 0) delete h; }
	bool isLoaded() { return h->obj->isLoaded(); }
};

class blob {
public:
	blob(int size, void *buffer = 0) : size(size), buffer(buffer) {
		if (!buffer) {
			this->buffer = new char[size];
		}
	}
	int size;
	void *buffer;
};

template <typename T>
class ResourceTypeHandler {
public:
	virtual T *load(blob &) = 0;
};

template <typename T>
class ResourceStorer {
public:
	virtual void save(blob &, T *) = 0;
};

class ResourceManager {
	struct ResourceStorageBase {
		virtual void cleanup() = 0;
	};
	template <typename T>
	struct ResourceStorage : public ResourceStorageBase {
		static ResourceStorage<T> &Instance() { static ResourceStorage<T> inst; return inst; }
		std::vector<Res<T> > resources;
		ResourceStorage() {
			ResourceManager::instance().storages.push_back(this);
		}
	};
	std::map<std::string, std::vector<void *> > rths;
	std::map<std::string, void *> rss;
	std::string rootDir;
	std::vector<ResourceStorageBase *> storages;
public:
	static ResourceManager &instance() { static ResourceManager rm; return rm; }
	void setRootDir(std::string rootDir) { this->rootDir = rootDir; }
	template <typename T>
	Res<T> getResource(const std::string &name);
	template <typename T>
	void RegisterResourceTypeHandler(ResourceTypeHandler<T> &rth) {
		rths[typeid(T).name()].push_back((void *)&rth);
	}
	template <typename T>
	void RegisterResourceStorer(ResourceStorer<T> &rs) {
		rss[typeid(T).name()] = (void *)&rs;
	}
};

#endif


