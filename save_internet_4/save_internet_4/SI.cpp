#include "SI.h"

int wmain(int argc,const wchar_t *argv[])
{
	if (argc == 3)
	{
		if (!wcscmp(argv[1], TEXT(L"-c")))
			c(argv[2]);
		else if (!wcscmp(argv[2], TEXT(L"-c")))
			c(argv[1]);
		else
		{
			cout << "-c <*.bmp>\n";
		}
	}
	else if (argc == 4)
	{
		if (!wcscmp(argv[1], TEXT(L"-e")))
			e(argv[2], argv[3]);
		else if (!wcscmp(argv[1], TEXT(L"-g")))
			g(argv[2], argv[3]);
		else
		{
			cout << "-e <*.bmp> <*.txt>\n";
			cout << "-g <*.bmp> <*.txt>\n";
		}
	}
	else
	{
		cout << "-c <*.bmp>\n";
		cout << "-e <*.bmp> <*.txt>\n";
		cout << "-g <*.bmp> <*.txt>\n";
	}
	return 0;
}