#include <iostream> 
using namespace std;
class Bits
{
public:

	int Pointer;
	int Length;
	int MaxLength;
	Bits(int bytelength) { data = new unsigned char[bytelength]; MaxLength = bytelength * 8; Length = 0; Pointer = 0; };
	Bits(string  filepath);
	Bits(Bits* bit);
	~Bits() {delete[] data;}

	unsigned int ToInt32() {
		unsigned int re = 0;
		for (int i =3; i>=0; i--)
		{
			re = re * 256 + data[i];
		}
	}
	bool ReadBit();
	unsigned char ReadBitOfLength(int length);//读取长度在8以下的bit
	char ReadChar(); 
	int ReadInt32();
	void Write(char c);
	void Write(int v);
	void Write(Bits* bits);
	void Write(bool bit);
	void MoveFirst() { Pointer = 0; };
	bool Eof() { return Pointer >= Length; };
	bool ToFile(const char* filepath);
	bool ToFile(string filepath);
	void Show(); 
	void SetData(unsigned char* nd) { if (data != nullptr)delete[] data; data = nd; }
	void EnLarge(int bytelen);
private :
	unsigned char* data;
};


//class BitWriter
//{	
//public:
//	BitWriter();
//	~BitWriter();
//
//	int Write(int v);
//	int Write(Bits bits);
//private:
//	unsigned char *buff;
//	int buffLength;
//	int P;
//};
//
//class BitReader
//{
//public:
//	BitReader();
//	~BitReader();
//
//	int	ReadInt32();
//	int ReadBit();
//private:
//
//};
