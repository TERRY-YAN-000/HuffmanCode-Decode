#include<iostream> 
using namespace std;
#ifndef __MYFILE__
#define __MYFILE__
#define FileError_Non 0
#define FileError_NoThisFile 1
int ReadFile(string filepath, char*& result);
int WriteFile(string filepath, const char* content);

#endif // !__MYFILE__


