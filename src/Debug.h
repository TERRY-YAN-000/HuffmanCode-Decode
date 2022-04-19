#ifndef __DEBUG__ 

#define __DEBUG__
#include <string>
using namespace std;
void Log(const char* c);
void Log(const string str);
void Log(const int i);
void Log(const char c);
void 字节单位转换(int len);


string ReadString();
int ReadInt();
#endif // !__DEBUG__

