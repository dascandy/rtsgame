#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>
#include <map>
#include <vector>
#include "QueuedWork.h"
#include "Blob.h"
#include "debug.h"

class Resource {
public:
	virtual ~Resource() {}
	virtual bool isLoaded() = 0;
	static const char *getDirName();
};

template <typename T>
class Res : public Resource {
	struct Handle {
		Handle() : obj(0), usageCount(1), loadQueued(false) {}
		~Handle() { delete obj; }
		T *obj;
		int usageCount;
		bool loadQueued;
	};
	friend class ResourceManager;
	Res() : h(new Handle()) { }
	Handle *h;
	void dec() {
		h->usageCount--; 
		if (h->usageCount == 0) 
			delete h; 
	}
	void inc() {
		h->usageCount++;
	}
public:
	bool operator==(const Resource &r) {
		const Res<T> *t = dynamic_cast<const Res<T> *>(&r);
		return (t != null && t->h == h);
	}
	Res(const Res<T> &obj) : h(h) { 
		inc();
	}
	Res<T> &operator=(const Res<T> &o) {
		dec();
		h = o.h;
		inc();
	}
	~Res() { 
		dec();
	}
	bool isLoaded() { return !!h->obj; }
};

class ResourceQueue {
public:
	std::vector<Resource *> resources;
	template <typename T>
	void addResource(const Res<T> &resource) {
		resources.push_back(new Res<T>(resource));
	}
	bool allLoaded() {
		for (std::vector<Resource *>::iterator it = resources.begin(); it != resources.end(); ++it) {
			if (!(*it)->isLoaded()) return false;
		}
		return true;
	}
};

class BaseTypeHandler { 
public:
	virtual const char *getExt() = 0;
};

template <typename T>
class ResourceTypeHandler : public BaseTypeHandler {
public:
	virtual T *load(Blob &) = 0;
};

template <typename T>
class ResourceStorer : public BaseTypeHandler {
public:
	virtual Blob save(T *) = 0;
};

template <typename T>
class QueuedLoad : public Queued {
	std::string name;
	Res<T> res;
	QueuedLoad(std::string name, Res<T> res)
	: name(name)
	, res(res)
	{}
	void run() {
		const char *dirName = T::getDirName();
		std::vector<BaseTypeHandler *> &rths = ResourceManager::Instance().rths[typeid(T).name()];
		for (std::vector<BaseTypeHandler *>::iterator it = rths.begin(); it != rths.end(); ++it) {
			ResourceTypeHandler<T> *rth = (ResourceTypeHandler<T> *)*it;
			blob b = ResourceManager::Instance().loadblob(dirName + "/" + name + "." + rth->getExt());
			if (b.size) {
				QueuedWork::ResourceLoading.push_back(new QueuedOpen(name, b, res, *rth));
				return;
			}
		}
		Log("Cannot find file to load for resource %s of type %s", name.c_str(), typeid(T).name());
	}
};

template <typename T>
class QueuedOpen : public Queued {
	std::string name;
	Blob data;
	Res<T> res;
	ResourceTypeHandler<T> &rth;
	QueuedOpen(std::string name, Blob data, Res<T> res, ResourceTypeHandler<T> &rth)
	: name(name)
	, data(data)
	, res(res)
	, rth(rth)
	{}
	void run() {
		T *obj = rth.load(data);
		if (obj)
			res.h->replaceWith(obj);
		else
			Log("Couldn't load new resource for %s of type %s, input is corrupt", name, typeid(T).name());
	}
};

class ResourceManager {
	struct ResourceStorageBase {
		virtual void cleanup() = 0;
	};
	template <typename T>
	struct ResourceStorage : public ResourceStorageBase {
		static ResourceStorage<T> &Instance() { static ResourceStorage<T> inst; return inst; }
		std::map<std::string, Res<T> > resources;
		ResourceStorage() {
			ResourceManager::instance().storages.push_back(this);
		}
		void cleanup() {
			TODO_W("Implement cleanup in resource storage");
		}
	};
	std::map<std::string, std::vector<BaseTypeHandler *> > rths;
	std::map<std::string, BaseTypeHandler *> rss;
	std::string rootDir;
	std::vector<ResourceStorageBase *> storages;
public:
	static ResourceManager &Instance() { static ResourceManager rm; return rm; }
	void setRootDir(std::string rootDir) { 
		this->rootDir = rootDir; 
		TODO_W("Add inotify stuff here");
	}
	template <typename T>
	Res<T> getResource(const std::string &name) {
		Res<T> hdl = ResourceStorage<T>::Instance().resources[name];
		if (!hdl.h->loadQueued) {
			QueuedWork::Background.push_back(new QueuedLoad(name, hdl));
			hdl.h->loadQueued = true;
		}
		return hdl;
	}
	template <typename T>
	Blob storeResource(Res<T> *resource) {
		ResourceStorer *rs = (ResourceStorer *)rss[typeid(T).name()];
		if (!rs) {
			Log("Cannot find handler to store resource %s of type %s", name.c_str(), typeid(T).name());
		} else {
			return rs->save(resource->h->obj);
		}
	}
	template <typename T>
	void RegisterResourceTypeHandler(ResourceTypeHandler<T> &rth) {
		rths[typeid(T).name()].push_back((void *)&rth);
	}
	template <typename T>
	void RegisterResourceStorer(ResourceStorer<T> &rs) {
		rss[typeid(T).name()] = (void *)&rs;
	}
	Blob loadblob(const std::string &name);
	void saveblob(const std::string &name, Blob &b);
};

#endif


