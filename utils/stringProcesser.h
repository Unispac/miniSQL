#ifndef  stringProcesser_H
#define stringProcesser_H
#include<vector>
#include<string>
#include<interpreter\syntaxError.h>
#include<logic\Logic.h>
#include<sstream>
using namespace std;

class stringProcesser
{
public:
	static vector<string> split(const string& str, const string& delim) 
	{
		vector<string> res;
		if ("" == str||str==delim) return res;
		string x = str;
		trim(x);
		int t = x.find(delim);
		int L = delim.length();
		while (t != -1)
		{
			res.push_back(x.substr(0, t));
			x = x.substr(t+L);
			trim(x);
			t = x.find(delim);
		}
		if (x.length() > 0)res.push_back(x);
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
	static bool getRidPar(string &x)
	{
		trim(x);
		if (!x.empty())
		{
			if (x[0] == '('&&x[x.size() - 1] == ')')x = x.substr(1, x.size() - 2);
		}
		return true;
	}
	static bool getRidQuo(string &x)
	{
		trim(x);
		if (!x.empty())
		{
			if (isChar(x))x = x.substr(1, x.size() - 2);
		}
		return true;
	}
	static bool isFloat(string x)
	{
		trim(x);
		int len = x.size();
		int cnt = 0;
		for (int i = 0; i < len; i++)
		{
			if ((x[i]<'0' || x[i]>'9') && x[i] != '.')return false;
			if (x[i] == '.')cnt++;
			if (cnt > 1)return false;
		}
		return true;
	}
	static bool isChar(string x)
	{
		trim(x);
		if (x[0] != '\''&&x[0] != '\"')return false;
		int len = x.size() - 1;
		if (x[len] != '\''&&x[len] != '\"')return false;
		return true;
	}
	static bool isInt(string x)
	{
		int len = x.size();
		for (int i = 0; i < len; i++)
		{
			if (x[i]<'0' || x[i]>'9')return false;
		}
		return true;
	}
	static int getConditionType(string x)
	{
		//int cnt[6], i;
		if (x.find("<>") != -1)return NOT_EQUAL;
		if (x.find("=") != -1)return EQUAL;
		if (x.find(">=") != -1)return GREATER_EQUAL;
		if (x.find("<=") != -1)return LESS_EQUAL;
		if (x.find(">") != -1)return GREATER_THAN;
		if (x.find("<") != -1)return LESS_THAN;
		return -1;
	}
	static Logic* getLogic(string x, int cmpType, dbDataType * dbType)
	{
		int dataType = dbType->dbType;
		int loc;
		string left, right;
		switch (cmpType)
		{
		case EQUAL:
			loc = x.find("=");
			if (loc == -1) { syntaxError::Error(); return NULL; }
			left = x.substr(0, loc);
			right = x.substr(loc + 1);
			break;
		case NOT_EQUAL:
			loc = x.find("<>");
			if (loc == -1) { syntaxError::Error(); return NULL; }
			left = x.substr(0, loc);
			right = x.substr(loc + 2);
			break;
		case GREATER_THAN:
			loc = x.find(">");
			if (loc == -1) { syntaxError::Error(); return NULL; }
			left = x.substr(0, loc);
			right = x.substr(loc + 1);
			break;
		case LESS_THAN:
			loc = x.find("<");
			if (loc == -1) { syntaxError::Error(); return NULL; }
			left = x.substr(0, loc);
			right = x.substr(loc + 1);
			break;
		case GREATER_EQUAL:
			loc = x.find(">=");
			if (loc == -1) { syntaxError::Error(); return NULL; }
			left = x.substr(0, loc);
			right = x.substr(loc + 2);
			break;
		case LESS_EQUAL:
			loc = x.find("<=");
			if (loc == -1) { syntaxError::Error(); return NULL; }
			left = x.substr(0, loc);
			right = x.substr(loc + 2);
			break;
		}
		stringProcesser::trim(left);
		stringProcesser::trim(right);
		tableValue temp;

		Logic * condition;
		stringstream ss;
		switch (dataType)
		{
		case DB_INT:
			if (stringProcesser::isInt(right) == false)
			{
				syntaxError::Error(); return NULL;
			}
			ss << right;
			ss >> temp.INT;	
			break;
		case DB_FLOAT:
			if (stringProcesser::isFloat(right) == false)
			{
				syntaxError::Error(); return NULL;
			}
			ss << right;
			ss >> temp.FLOAT;
			break;
		case DB_CHAR:
			if (stringProcesser::isChar(right) == false)
			{
				syntaxError::Error(); return NULL;
			}
			stringProcesser::getRidQuo(right);
			temp.CHAR = new char[dbType->n];
			memcpy(temp.CHAR, right.c_str(), right.size() + 1);
			break;
		}
		//
		condition = new Logic(left, cmpType, temp);
		return condition;
	}
};

#endif // ! stringProcesser_H
