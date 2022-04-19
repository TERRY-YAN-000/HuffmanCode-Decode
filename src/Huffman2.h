#ifndef __HUFFMAN__ 
#define __HUFFMAN__
#include"CountWord.h"
#include "Binary.h"
namespace V2Code {

#define HUFFMANERROR int
#define HUFFMANERROR_NON 0
#define HUFFMANERROR_TabError 1
#define HUFFMANERROR_NoThisCode 2
#define HUFFMANERROR_LengthError 3
#define HUFFMANERROR_CodeError 4

	typedef struct HTreeNode {
		HTreeNode* lCr;
		HTreeNode* rCr;

		char word;
		int weight;
	}HTreeNode;

	typedef struct {
		//原来的char
		char origin;
		Bits* code;
	}HuffmanTabItem;

	typedef struct {
		int count;
		int MaxCodeLen;
		HuffmanTabItem* Items;
		HTreeNode* Tree;
	}HuffmanTab;

	HuffmanTab CreatHuffmanTab(CounterResult cr);
	void ShowHuffmanTab(HuffmanTab tab);
	/// <summary>
	/// 显示编码后的字符串
	/// </summary>
	 HUFFMANERROR EnCode_ToBits(const char* str, Bits*& result);
	 HUFFMANERROR DeCode_FromBits(Bits* bits, char*& result);
	 void Destory(HuffmanTab tb);

}


#endif // !__HUFFMAN__