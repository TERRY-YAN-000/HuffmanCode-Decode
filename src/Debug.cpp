#include "Debug.h"
#include <iostream> 
void Log(const char* c)
{
	std::cout << c;
}

void Log(const string str)
{
	std::cout << str;
}


void Log(const int i)
{
	std::cout << i;
}

void Log(const char c)
{
	cout << c;
}

void 字节单位转换(int len)
{
	int GB = len>> 30;
	int MB = (len >> 20)%1024;
	int KB = (len >> 10) % 1024;
	int B = len%1024;
	if (GB > 0)
	{
		cout <<GB <<"GB";
	}
	if (MB > 0)
	{
		cout << MB << "MB";
	}
	if (KB > 0)
	{
		cout << KB << "KB";
	}
	if (B > 0)
	{
		cout << B << "B";
	}
}

string ReadString()
{
	string s;
	cin >> s;
	return s;
}
int ReadInt()
{
	int re;
	cin >> re;
	return re;
}
