#include "File.h" 
#include <fstream>
#include <iostream>
using namespace std;

int ReadFile(string path, char*& result)
{
    if (result!=NULL)
    {
        delete[] result;
    }
    ifstream  afile;
    afile.open(path,ios_base::binary);
    if (!afile.is_open())
    {
        return FileError_NoThisFile;
    }

    afile.seekg(0, ios_base::end); // 把文件指针到尾部
    int len = afile.tellg(); // 获得文件字节数
    // 如果读取文件，需要把文件指针指向文件开始出
    afile.seekg(0, ios_base::beg);
    result = new char[len+1];
    afile.read(result,len);
    afile.close();
    result[len] = 0;
    return 0;
}

int WriteFile(string filepath, const char* content)
{
    ofstream afile;
    afile.open(filepath, ios_base::binary);
    if (!afile.is_open())
    {
        return FileError_NoThisFile;
    }
    afile << content <<endl;
    afile.close();
    return 0;
}


