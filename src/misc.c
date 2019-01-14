#include "includes.h"

void printColor(int color, char *str)
{
	#ifdef WIN32
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		WORD saved_attributes;

		GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
		saved_attributes = consoleInfo.wAttributes;

		switch (color)
		{
			case RED:
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
				break;
			case GREEN:
				SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
				break;
			case BLUE:
				SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
				break;
			case YELLOW:
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
				break;
		}
		
		printf("%s", str);

		SetConsoleTextAttribute(hConsole, saved_attributes);
	#else
		switch (color)
		{
			case RED:
				printf("\033[31m");
				break;
			case GREEN:
				printf("\033[32m");
				break;
			case BLUE:
				printf("\033[34m");
				break;
			case YELLOW:
				printf("\033[33m");
				break;
		}
		
		printf("%s\033[0m", str);
		
	#endif
}

int isElementOf(char *element, char **array, int length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		if (strcmp(element, array[i]) == 0)
		{
			return 1;
		}
	}
	
	return 0;
}

unsigned int getFileSize(FILE *file)
{	
	fseek(file, 0, SEEK_END);
	unsigned int size = ftell(file);
	rewind(file);
	
	return size;
}

int createDirectory(char *path)
{
	#if (defined (WIN32) || defined(WIN64))
		if (CreateDirectoryA(path, NULL) != 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	#else
		return mkdir(path, 0755);
	#endif
}