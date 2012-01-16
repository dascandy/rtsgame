#include "profile.h"

const char *profile_name[256];
unsigned long long profile_ents[256];
int profile_count[256];
bool profile_enabled = false;
FILE *out;
FILE *csv;

void profile_init() 
{
	for (int i = 0; i < 256; i++) 
	{
		profile_name[i] = 0;
		profile_ents[i] = 0;
	}
	out = fopen("C:\\crusader.deb", "w");
	csv = fopen("C:\\crusader.csv", "w");
	profile_enter(0);
}

void profile_fini() 
{
	profile_print();
	fclose(out);
	for (int i = 0; i < 20; i++) 
	{
		if (i != 0) fprintf(csv, ",");
		fprintf(csv, "%s,%s count", profile_name[i]? profile_name[i] : "unknown", profile_name[i]? profile_name[i] : "unknown");
	}
	fprintf(csv, "\n");
	fclose(csv);
}

void profile_print()
{
	profile_exit(0);
	fprintf(out, "----------------------------------------------");
	for (int i = 0; i < 20; i++) 
	{
		if (i != 0) fprintf(csv, ",");
		fprintf(csv, "%I64u", profile_ents[i]);
		fprintf(csv, ",%u", profile_count[i]);
		if ((1000000.0 * profile_ents[i]) / profile_ents[0]  > 1.0)
		{
			fprintf(out, "%s (%d) = %2.5f (%I64u)\n", profile_name[i] ? profile_name[i] : "unknown", i, (100.0 * profile_ents[i]) / profile_ents[0], profile_ents[i]);
		}
		profile_ents[i] = 0;
		profile_count[i] = 0;
	}
	fprintf(csv, "\n");
	profile_enter(0);
}

