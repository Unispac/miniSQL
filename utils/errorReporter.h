#ifndef errorReporter_H
#define errorReporter_H

#include<iostream>
#include<string>
#include<map>
using namespace std;

class errorReporter
{
public:
	map<int, string>errorTemplate;
	errorReporter()
	{
		errorTemplate[0] = " * error [0] : Unexpected buffer explosion, no resource in buffer can be released.";
		errorTemplate[1] = " * error [1] : Error when reading file.. Ensure the filepath is correct.";
		errorTemplate[2] = " * error [2] : Error when writing file.. Ensure the filepath is correct.";

	}
	void reportErrorCode(int x)
	{
		if (errorTemplate.count(x) != 0)
		{
			cout << errorTemplate[x];
			system("pause");
			exit(0);
		}
	}
};


#endif // !errorReporter_H