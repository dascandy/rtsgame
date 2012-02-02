#ifndef PROFILE_H
#define PROFILE_H

#include <stdio.h>

extern const char *profile_name[256];
extern unsigned long long profile_ents[256];
extern int profile_count[256];
extern bool profile_enabled;

#ifdef _MSC_VER
inline unsigned long long rdtsc() {
   __asm {
      RDTSC
   }
}
#else
inline unsigned long long rdtsc() {
	// TODO
	return 0;
}
#endif

inline void profile_reg(int id, const char *name) { profile_name[id] = name; }
inline void profile_enter(int id) { if (!profile_enabled) return; profile_ents[id] -= rdtsc(); profile_count[id]++; }
inline void profile_exit(int id) { if (!profile_enabled) return;profile_ents[id] += rdtsc(); }

#define profile(id, name) for (profile_region __reg(id, name); __reg; )
class profile_region {
public:
	profile_region(int id, const char *name) : id(id), name(name), first(false) { profile_reg(id, name); profile_enter(id); }
	~profile_region() { profile_exit(id); }
	operator bool() { return first =!first; }
private:
	int id;
	const char *name;
	bool first;
};

void profile_init();
void profile_print();
void profile_fini();

#endif

