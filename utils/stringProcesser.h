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
		//先将要切割的字符串从string类型转换为char*类型  
		char * strs = new char[str.length() + 1]; //不要忘了  
		strcpy(strs, str.c_str());

		char * d = new char[delim.length() + 1];
		strcpy(d, delim.c_str());

		char *p = strtok(strs, d);
		while (p) {
			string s = p; //分割得到的字符串转换为string类型  
			res.push_back(s); //存入结果数组  
			p = strtok(NULL, d);
		}
		return res;
	}
	static void trim(string &s)
	{
		if (s.empty())return;
		s.erase(0, s.find_first_not_of(" "));
		s.erase(s.find_last_not_of(" ") + 1);
		return;
	}
	static bool stringToInt(string x,int &y)
	{
		y = 0;
		int size = x.size();
		if (size <= 0)return false;
		for (int i = 0; i < size; i++)
		{
			if (x[i] > '9' || x[i] < '0')return false;
			y = y * 10 + x[i] - '0';
		}
		return true;
	}
	static bool getRidPar(string x)
	{
		trim(x);
		if (!x.empty())
		{
			if (x[0] == '('&&x[x.size() - 1] == ')')x = x.substr(1, x.size() - 2);
		}
		return true;
	}
};

#endif // ! stringProcesser_H
