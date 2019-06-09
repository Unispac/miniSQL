#include<config.h>
#include<bufferManager/bufferManager.h>
using namespace std;

errorReporter * errorHandler;
void configInit()
{
	errorHandler = new errorReporter();
}
int main()
{
	configInit();
	bufferManager * test = new bufferManager();
	cout << "initial" << endl;
	char * data = new char[4096];
	for (int i = 0; i < blockSize; i++)
	{
		if(i==0)*(data+i) = 'A';
		else if (i == 1)*(data+i) = 'B';
		else *(data+i) = ' ';
	}
	cout<<test->getBlock("test", 0)->data;
	int x;
	system("pause");
	return 0;
}