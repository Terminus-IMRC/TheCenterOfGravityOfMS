#include <stdio.h>
#include <stdlib.h>
#include "libms.h"
#include "error.h"

#ifndef MY_X
#error Define MY_X in compile
#endif

#if MY_X == 4
#define MSFILE "ms4.txt"
#elif MY_X == 5
#define MSFILE "ms5saf.txt"
#else
#error Unsupported MY_X value
#endif

int main()
{
	int *ms;
	char buf[0xffff];
	ms_state_t st;
	FILE *fp;

	ms_init(MY_X, MS_ORIGIN_ONE, &st);
	ms = ms_alloc(&st);

	fp = fopen(MSFILE, "r");
	if (fp == NULL) {
		error("cannot open MSFILE\n");
		exit(EXIT_FAILURE);
	}

	while (fgets(buf, 0xffff, fp) != NULL) {
		int i;
		float tcogom_x = 0.0, tcogom_y = 0.0;

		str_to_ms(ms, buf, &st);

		printf("ms: ");
		output_ms(ms, stdout, &st);

		for (i = 0; i < ms_Ceilings(&st); i++) {
			tcogom_x += ms[i] * (i % ms_X(&st));
			tcogom_y += ms[i] * (int)(i / ms_X(&st));
		}
		tcogom_x /= ms_Ceilings(&st)*(1+ms_Ceilings(&st))/2.0;
		tcogom_y /= ms_Ceilings(&st)*(1.0+ms_Ceilings(&st))/2.0;

		printf("x=%f y=%f\n", tcogom_x, tcogom_y);
	}

	fclose(fp);

	ms_free(ms, &st);
	ms_finalize(&st);

	return 0;
}
