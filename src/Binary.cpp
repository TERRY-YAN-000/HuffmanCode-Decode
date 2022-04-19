#include "Binary.h" 
#include <iostream>
#include <fstream>
using namespace std;

Bits::Bits(string filepath)
{
	Pointer = 0; data = NULL;
	ifstream  afile;
	afile.open(filepath, ios_base::binary);
	if (!afile.is_open())
	{
		Length = 0;
		return;
	}

	afile.seekg(0, ios_base::end); // 把文件指针到尾部
	Length = afile.tellg(); // 获得文件字节数
	// 如果读取文件，需要把文件指针指向文件开始出
	afile.seekg(0, ios_base::beg);
	data = new unsigned char[Length];
	afile.read((char*)data, Length);
	Length *= 8;
	MaxLength = Length;
	afile.close();
}

Bits::Bits(Bits* bit)
{
	Length = bit->Length;
	MaxLength = bit->MaxLength;
	data = new unsigned char[MaxLength/8];
	int len = Length / 8 + (Length % 8 ? 1 : 0);
	for (int i = 0; i <len ; i++)
	{
		data[i] = bit->data[i];
	}
}

bool Bits::ReadBit()
{
	if (Pointer >= Length)
	{
		throw "异常";
	}
	unsigned char c = data[Pointer / 8];
	unsigned char t = ((unsigned char)1 << (7 - (Pointer % 8)));
	Pointer++;
	return (c &t )>0;
}


unsigned char Bits::ReadBitOfLength(int length)
{
	int count = 0;
	int first8 = Pointer / 8 * 8;
	int next8 = first8 + 8;
	unsigned char re;
	unsigned char c;
	count = next8 - Pointer;
	c = data[Pointer / 8];
	if (count >= length)
	{
		c = (c << (Pointer - first8)) >> (Pointer - first8);
		c = (c >> (count - length)) << (8 - length);
		Pointer += length;
		return c;
	}
	else
	{
		c = (c << (Pointer - first8)) >> (Pointer - first8);
		c = c << (8 - count);
		Pointer = next8;
		c += (ReadBitOfLength(length - count) >> (count));
		return c;
	}
	return false;
}

char Bits::ReadChar()
{
	if (Pointer % 8 != 0)
	{
		throw;//不能在中间读取或者写入信息
	}
	unsigned char c = data[Pointer / 8];
	Pointer += 8;
	//return c >= 128 ? (char)c -256:(char)c;
	return c;
}

int Bits::ReadInt32()
{
	int re = 0;
	if (Pointer % 8 != 0)
	{
		throw;
	}
	Pointer += 24;
	for (int i = 0; i < 4; i++)
	{
		re = re * 256 + data[Pointer / 8 - i];
	}
	Pointer += 8;
	return re;
}

void Bits::Write(char c)
{
	
	if (Pointer % 8 != 0)
	{
		throw;//不能在中间读取或者写入信息
	}
	if (Pointer + 8 > MaxLength)
	{
		EnLarge(2);
	}
	data[Pointer / 8] = c;
	Pointer += 8;
	Length = Length > Pointer ? Length : Pointer;
}

void Bits::Write(int v)
{
	
	if (Pointer % 8 != 0)
	{
		throw;//不能在中间读取或者写入信息
	}
	if (Pointer + 32 > MaxLength)
	{
		EnLarge(6);
	}
	for (int i = 0; i < 4; i++)
	{
		data[Pointer / 8] = v % 256;
		Pointer += 8;
		v /= 256;
	}
	Length = Length > Pointer ? Length : Pointer;
}

void Bits::Write(Bits* bits)
{
	//TODO高效
	if (Pointer + bits->MaxLength > MaxLength)
	{
		EnLarge((Pointer + bits->MaxLength - MaxLength )/8+5);
	}
	int pos = bits->Pointer;
	bits->MoveFirst();
	while (!bits->Eof())
	{
		bool b = bits->ReadBit();
		Write(b);
	}
	bits->Pointer = pos;
}

void Bits::Write(bool bit)
{
	if (Pointer +1> MaxLength)
	{
		EnLarge(2);
	}
	char c = (unsigned char)1 << (7 - Pointer % 8);
	bool h = data[Pointer / 8] & c;
	if (bit && !h)
	{
		data[Pointer / 8] += c;
	}
	else if (!bit && h)
	{
		data[Pointer / 8] -= c;
	}
	Pointer++;
	Length = Length > Pointer ? Length : Pointer;
}

bool Bits::ToFile(const char* filepath)
{
	ofstream afile;
	afile.open(filepath, ios_base::binary);
	if (!afile.is_open())
	{
		return false;
	}
	afile.write((const char*)data, Length/8+(Length%8 ? 1:0));
	afile.close();
	return true;
}
bool Bits::ToFile(string filepath)
{
	ofstream afile;
	afile.open(filepath, ios_base::binary);
	if (!afile.is_open())
	{
		return false;
	}
	afile.write((const char*)data, Length / 8 + (Length % 8 ? 1 : 0));
	afile.close();
	return true;
}

void Bits::Show()
{
	int p =Pointer;
	MoveFirst();
	while (!Eof())
	{
		std::cout << ReadBit() ? "1" : "0";
	}
	//std::cout << "\n";
	Pointer = p;
}

void Bits::EnLarge(int bytelen)
{
	unsigned char* nd = new unsigned char[bytelen + MaxLength/8];
	for (int i = 0; i < Length/8+(Length % 8 ? 1:0); i++)
	{
		nd[i] = data[i];
	}
	data = nd; 
	MaxLength += bytelen * 8;
}
