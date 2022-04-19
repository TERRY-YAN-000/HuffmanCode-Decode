#ifndef __HUFFMAN__
#define __HUFFMAN__

#define HUFFMANERROR int
#define HUFFMANERROR_NON 0
#define HUFFMANERROR_TabError 1
#define HUFFMANERROR_NoThisCode 2
#define HUFFMANERROR_LengthError 3
#define HUFFMANERROR_CodeError 4

#include"WordCounter.h"
typedef struct HTreeNode {
	//HTreeNode* father;
	HTreeNode* lCr;
	HTreeNode* rCr;
	char word;
	int weight;
}HTreeNode;

typedef struct {
	//原来的char
	char origin;
	char* visible;
	//编码
	unsigned int code;
	int codelen;
}HuffmanTabItem;

typedef struct {
	int MaxLen;
	int count;
	int MaxCodeLen;
	int MaxByteCodeLen;
	HuffmanTabItem* Items;
	HTreeNode* Tree;
}HuffmanTab;

HuffmanTab CreatHuffmanTab(CounterResult cr);
void ShowHuffmanTab(HuffmanTab tab);
/// <summary>
/// 显示编码后的字符串
/// </summary>
HUFFMANERROR ShowHuffmanStr(const char* str);
HUFFMANERROR EnCode_ToString(const char* str,char*& result);
HUFFMANERROR DeCode_FromString( char* strbyte, char*& result);
void Destory(HuffmanTab tb);

#endif // !__HUFFMAN__


