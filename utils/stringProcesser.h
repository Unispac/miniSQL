#ifndef  stringProcesser_H
#define stringProcesser_H
#include<vector>
#include<string>
using namespace std;

class stringProcesser
{
public:
	static vector<string> split(const string& str, const string& delim) 
	{
		vector<string> res;
		if ("" == str) return res;
		//�Ƚ�Ҫ�и���ַ�����string����ת��Ϊchar*����  
		char * strs = new char[str.length() + 1]; //��Ҫ����  
		strcpy(strs, str.c_str());

		char * d = new char[delim.length() + 1];
		strcpy(d, delim.c_str());

		char *p = strtok(strs, d);
		while (p) {
			string s = p; //�ָ�õ����ַ���ת��Ϊstring����  
			res.push_back(s); //����������  
			p = strtok(NULL, d);
		}
		return res;
	}
};

#endif // ! stringProcesser_H
