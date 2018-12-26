#include "includes.h"

int isElementOf(char *element, char **array, int length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		if (strcmp(element, array[i]) == 0)
			return 1;
	}
	
	return 0;
}