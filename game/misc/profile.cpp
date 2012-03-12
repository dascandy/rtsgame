#include "profile.h"
#include <string.h>
#include <stdlib.h>

const char *profile_name[256];
unsigned long long profile_ents[256*PROFILE_COUNT];
unsigned long long *profile_entsp = profile_ents;
int profile_count[256*PROFILE_COUNT];
int *profile_countp = profile_count;
bool profile_enabled = false;
int profile_current = 0;
FILE *out;
FILE *csv;

#ifdef _MSC_VER
#define _64B_ "%I64u"
#else
#define _64B_ "%llu"
#endif

void profile_init(const char *fname) 
{
	char debn[1024];
	char csvn[1024];
	sprintf(debn, "%s.deb", fname);
	sprintf(csvn, "%s.csv", fname);
	for (int i = 0; i < 256; i++) 
	{
		profile_name[i] = 0;
		profile_ents[i] = 0;
	}
	out = fopen(debn, "w");
	csv = fopen(csvn, "w");
	profile_enter(0);
}

void profile_fini() 
{
	fclose(out);
	for (int i = 0; i < 256; i++) 
	{
		if (i != 0) fprintf(csv, ",");
		fprintf(csv, "%s,%s count", profile_name[i]? profile_name[i] : "unknown", profile_name[i]? profile_name[i] : "unknown");
	}
	fprintf(csv, "\n");
	fclose(csv);
}

void profile_next() {
	profile_current++;
	if (profile_current == PROFILE_COUNT) profile_current = 0;
	profile_entsp = profile_ents + PROFILE_COUNT * profile_current;
	profile_countp = profile_count + PROFILE_COUNT * profile_current;

	for (int i = 0; i < 256; i++) {
		profile_entsp[i] = 0;
		profile_countp[i] = 0;
	}
}

void profile_print_overview_line(int gen, int line, char *buffer) {
	sprintf(buffer, "<tr><td>%d</td><td>%s</td><td><a href=\"/profile/graph?id=%d\">%d cycles</a></td>"
		"<td><a href=\"/profile/graph?id=%d&relative=true\">%2.4f%%</a></td></tr>", 
		line, profile_name[line], line, profile_count[gen * PROFILE_COUNT + line], line, 
		100.0 * profile_count[gen * PROFILE_COUNT + line] / profile_count[gen * PROFILE_COUNT + line]);
}

void profile_print_overview(char *&buffer, int &size) {
	buffer = (char *)malloc(1048576);
	strcpy(buffer, "<html><head><title>Profiling overview</title></head><body>");
	char *pb = buffer;
	for (int i = 0; i < 256; i++) {
		pb += strlen(pb);
		profile_print_overview_line((profile_current + 99) % 100, i, pb);
	}
	strcat(buffer, "</body></html>");
}

void profile_print_graph(void *&buffer, int *&bytecount, int id, bool relative) {
	
}

