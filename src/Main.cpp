
 

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
	Log("��ǰΪ���˵�\n0:�˳�\n1:ͨ�������ַ������б���\n2:ͨ�������ַ��ļ����б���\n");
	//Log("3:���ļ���ȡ������\n4:�ӿ���̨���벢����\n");
	Log("3:���ļ���ȡ������\n");
	t = -1;
	while (!(t == 0 || t == 1 || t == 2 || t == 3 || t == 4))
	{
		Log("��������ȷ�ı��\n");
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
	Log("��ǰΪ�����ļ��˵�\n1:���浽�ļ�\n2:�������˵�\n");
	t = 0;
	while (!(t == 1 || t == 2))
	{
		Log("��������ȷ�ı��\n");
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
	Log("�������ַ���:\n");
	c = ReadString();
	EnCode_ToBits(c.c_str(), bits);
	Log("ԭ������: "); Log(c); Log("\n");
	Log("ԭ����С: "); �ֽڵ�λת��(c.length()+1); Log("\n");
	Log("���ڴ�С(����������01�����ܺ�): "); �ֽڵ�λת��(bits->Length / 8 + (bits->Length % 8 ? 1 : 0)); Log("  \n");
	if (bits->Length>1024*8)
	{
		Log("�������ݽϳ� ����1�鿴�������:(1)");
		if (ReadInt()==1)
		{
			Log("��������: \n"); bits->Show(); Log("\n");
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
	Log("��ǰΪ������Ҫ������ļ�����,�������ļ�λ��(��������ַ������һ��):\n");
	c = ReadString();
	if (ReadFile(c, c2))
	{
		return 1;
	}
	else
	{
		Log("��ȡ����:\n"); Log(c2); Log("\n");
		EnCode_ToBits(c2, bits);
		Log("ԭ������: "); Log(c2); Log("\n");
		Log("ԭ����С: ");�ֽڵ�λת��(int(strlen(c2)+1)); Log("\n");
		Log("���ڴ�С(����������01�����ܺ�): "); �ֽڵ�λת��(bits->Length / 8 + (bits->Length % 8 ? 1 : 0)); Log("  \n");
		if (bits->Length > 1024 * 8)
		{
			Log("�������ݽϳ� ����1�鿴�������:(1)");
			if (ReadInt() == 1)
			{
				Log("��������: \n"); bits->Show(); Log("\n");
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
	Log("��ǰΪ�������������ļ������ַ����,�������ļ�λ��(��������ַ������һ��):\n");
	Log("�����뱣���ַ\n");
	c = ReadString();
	
	if (bits!=NULL)
	{
		if (!bits->ToFile(c))
		{
			Log("����ʧ��\n");
			return 1;
		}
		Log("�����ɹ�\n");
		return 1;
	}
	else
	{
		if (WriteFile(c,CodeStr))
		{
			Log("����ʧ��\n");
			return 1;
		}
		Log("�����ɹ�\n");
		return 1;
	}
	
	
	return 1;
}
int ReadFile_DeCodeMenu() {
	Log("\n");
	string c  ;
	char* c2 = NULL;
	Log("��ǰΪ���ļ���ȡ���������,�������ļ�λ��(��������ַ�������˵�):\n�������ַ\n");
	c = ReadString();
	if (bits!=NULL)
	{
		delete bits; 
		bits = NULL;
	}
	bits = new Bits(c);
	if (bits->Length==0)
	{
		Log("û�е�ǰ�ļ� �������˵�\n\n");
		return 1;
	}
		

	if (DeCode_FromBits(bits, c2))
	{
		delete bits; bits = NULL;
		Log("����ʧ��\n");
		return 1;
	}
	else
	{
		Log("�����:\n"); Log(c2); Log("\n"); Log("\n"); Log("\n");
		CodeStr = c2;
		delete bits; bits = NULL;
		return 4;
	}
}
int Input_DeCodeMenu() {
	Log("���õ� �������˵�\n");
	return 1;
}


int MainCycle(int mem) {
	// 0 �˳� 1 ���˵� 2 �����ַ����� 3 ������Ҫ������ļ����� 4 �Ƿ���Ҫ����˵� 5 ���浽�ļ�����
	//6 ���ļ���ȡ������� 7���ַ�����ȡ�������
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

