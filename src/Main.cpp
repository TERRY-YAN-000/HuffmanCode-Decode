
 

#pragma region V2
#include"Huffman2.h"
#include"CountWord.h"
#include"File.h"
#include"Debug.h"
#include <string.h> 
using namespace V2Code;
char* CodeStr;
Bits* bits=NULL;

int MainMenu() {
	Log("\n");
	int t;
	Log("当前为主菜单\n0:退出\n1:通过输入字符串进行编码\n2:通过输入字符文件进行编码\n");
	//Log("3:从文件读取并解码\n4:从控制台输入并解码\n");
	Log("3:从文件读取并解码\n");
	t = -1;
	while (!(t == 0 || t == 1 || t == 2 || t == 3 || t == 4))
	{
		Log("请输入正确的编号\n");
		t = ReadInt();
	}
	if (t == 0) {
		return 0;
	}
	else if (t == 1) {
		return 2;
	}
	else if (t == 2)
	{
		return 3;
	}
	else if (t == 3) {
		return 6;
	}
	else if (t == 4)
	{
		return 7;
	}
	return 0;
}
int SaveOrBackMenu() {
	Log("\n");
	int t;
	Log("当前为保存文件菜单\n1:保存到文件\n2:返回主菜单\n");
	t = 0;
	while (!(t == 1 || t == 2))
	{
		Log("请输入正确的编号\n");
		t = ReadInt();
	}
	if (t == 1)
	{
		return 5;
	}
	else
	{
		return 1;
	}
}

int Input_EnCodeMenu() {
	Log("\n");
	string c;
	Log("请输入字符串:\n");
	c = ReadString();
	EnCode_ToBits(c.c_str(), bits);
	Log("原有数据: "); Log(c); Log("\n");
	Log("原本大小: "); 字节单位转换(c.length()+1); Log("\n");
	Log("现在大小(哈夫曼树和01数据总和): "); 字节单位转换(bits->Length / 8 + (bits->Length % 8 ? 1 : 0)); Log("  \n");
	if (bits->Length>1024*8)
	{
		Log("现有数据较长 输入1查看具体情况:(1)");
		if (ReadInt()==1)
		{
			Log("现有数据: \n"); bits->Show(); Log("\n");
		}
	}
	if (bits == NULL)
	{
		delete bits;
		bits = NULL;
	}
	return 4;
}
int ReadFile_EnCodeMenu() {
	Log("\n");
	string c;
	char* c2=NULL;
	Log("当前为输入需要编码的文件界面,请输入文件位置(输入错误地址返回上一级):\n");
	c = ReadString();
	if (ReadFile(c, c2))
	{
		return 1;
	}
	else
	{
		Log("读取到了:\n"); Log(c2); Log("\n");
		EnCode_ToBits(c2, bits);
		Log("原有数据: "); Log(c2); Log("\n");
		Log("原本大小: ");字节单位转换(int(strlen(c2)+1)); Log("\n");
		Log("现在大小(哈夫曼树和01数据总和): "); 字节单位转换(bits->Length / 8 + (bits->Length % 8 ? 1 : 0)); Log("  \n");
		if (bits->Length > 1024 * 8)
		{
			Log("现有数据较长 输入1查看具体情况:(1)");
			if (ReadInt() == 1)
			{
				Log("现有数据: \n"); bits->Show(); Log("\n");
			}
		}
		if (bits==NULL)
		{
			delete bits;
			bits = NULL;
		}
		delete[] c2;
		return 4;
	}
}
int SaveFile_Menu() {
	Log("\n");
	string c;
	Log("当前为输入编码后数据文件保存地址界面,请输入文件位置(输入错误地址返回上一级):\n");
	Log("请输入保存地址\n");
	c = ReadString();
	
	if (bits!=NULL)
	{
		if (!bits->ToFile(c))
		{
			Log("操作失败\n");
			return 1;
		}
		Log("操作成功\n");
		return 1;
	}
	else
	{
		if (WriteFile(c,CodeStr))
		{
			Log("操作失败\n");
			return 1;
		}
		Log("操作成功\n");
		return 1;
	}
	
	
	return 1;
}
int ReadFile_DeCodeMenu() {
	Log("\n");
	string c  ;
	char* c2 = NULL;
	Log("当前为从文件读取并解码界面,请输入文件位置(输入错误地址返回主菜单):\n请输入地址\n");
	c = ReadString();
	if (bits!=NULL)
	{
		delete bits; 
		bits = NULL;
	}
	bits = new Bits(c);
	if (bits->Length==0)
	{
		Log("没有当前文件 返回主菜单\n\n");
		return 1;
	}
		

	if (DeCode_FromBits(bits, c2))
	{
		delete bits; bits = NULL;
		Log("解码失败\n");
		return 1;
	}
	else
	{
		Log("结果是:\n"); Log(c2); Log("\n"); Log("\n"); Log("\n");
		CodeStr = c2;
		delete bits; bits = NULL;
		return 4;
	}
}
int Input_DeCodeMenu() {
	Log("弃用的 返回主菜单\n");
	return 1;
}


int MainCycle(int mem) {
	// 0 退出 1 主菜单 2 输入字符界面 3 输入需要编码的文件界面 4 是否需要保存菜单 5 保存到文件界面
	//6 从文件读取编码界面 7从字符串读取编码界面
	switch (mem)
	{
	case 0:
		return 0;
	case 1:
		return MainMenu();
	case 2:
		return Input_EnCodeMenu();
	case 3:
		return ReadFile_EnCodeMenu();
	case 4:
		return SaveOrBackMenu();
	case 5:
		return SaveFile_Menu();
	case 6:
		return ReadFile_DeCodeMenu();
	case 7:
		return Input_DeCodeMenu();
	default:
		break;
	}
	return 1;
}

int main() {
	int mem = 1;


	while (mem = MainCycle(mem))
	{
		
	}

	return 0;
}
#pragma endregion

