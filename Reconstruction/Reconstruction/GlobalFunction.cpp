#include "GlobalFunction.h"

int ErrorHandling(string message)
{
	cout << "An Error Occurs:" << message << endl;
	cout << "Input response:";
	int a;
	cin >> a;
	return 0;
}

int CreateDir(string dir_path)
{
	string tmp_path;
	for (int i = 1; i < dir_path.length(); i++)
	{
		if (dir_path[i] == '/')
		{
			tmp_path = dir_path.substr(0, i);

			int status;
			status = MY_ACCESS(tmp_path.c_str(), 0);
			if (status != 0)
			{
				status = MY_MKDIR(tmp_path.c_str());
				if (status != 0)
				{
					ErrorHandling("Error in CreateDir(): " + tmp_path);
					return -1;
				}
			}
		}
	}

	return 0;
}