#include "Debug.h"
#include "Huffman2.h" 

using namespace V2Code;

#define Node_Non -1
#define TabEnd 1

#pragma region 堆
void Swap(HTreeNode& t1, HTreeNode& t2) {
	HTreeNode t;
	t = t1;
	t1 = t2;
	t2 = t;
}

void Heap_build(HTreeNode* tree, int root, int length)
{
	int lchild = root * 2 + 1;
	if (lchild < length)
	{
		int flag = lchild;
		int rchild = lchild + 1;
		if (rchild < length)
		{
			if (tree[rchild].weight > tree[flag].weight)//找出左右子结点中的最大值
			{
				flag = rchild;
			}
		}
		if (tree[root].weight < tree[flag].weight)
		{
			Swap(tree[root], tree[flag]);
			Heap_build(tree, flag, length);
		}
	}
}
void Heap_Init(HTreeNode* tree, int len)
{
	for (int i = len / 2; i >= 0; --i)
	{
		Heap_build(tree, i, len);
	}

	for (int j = len - 1; j > 0; --j)
	{
		Swap(tree[0], tree[j]);
		Heap_build(tree, 0, j);
	}

}
#pragma endregion

#pragma region Destory 回收内存
void Destory(HTreeNode* node)
{
	if (node->lCr == NULL && node->rCr == NULL)
	{
		delete node;
		return;
	}

	if (node->lCr)
	{
		Destory(node->lCr);
	}
	if (node->rCr)
	{
		Destory(node->rCr);
	}
}
void V2Code::Destory(HuffmanTab tb)
{
	delete[] tb.Items;

}
#pragma endregion

#pragma region EnCode_ToBits
void CreatTabByTree(HTreeNode* node, Bits* bits, HuffmanTabItem* item) {
	if (node->lCr == nullptr && node->rCr == nullptr)
	{
		if (item[node->word].code != 0)
		{
			Log("哈夫曼树异常:存在相同节点");
			while (true)
			{

			}
		}
		item[node->word].code = new Bits(bits);
		item[node->word].code->Length = bits->Pointer;
		item[node->word].origin = node->word;
	}
	else if (node->lCr != nullptr && node->rCr != nullptr)
	{
		int pos = bits->Pointer;
		bits->Write(false);
		CreatTabByTree(node->lCr, bits, item);
		bits->Pointer = pos;
		bits->Write(true);
		CreatTabByTree(node->rCr, bits, item);
	}
	else
	{
		Log("\n哈夫曼树创建产生严重异常\n");
		while (true)
		{

		}
	}
}
HuffmanTab V2Code::CreatHuffmanTab(CounterResult cr)
{
	HTreeNode* HTree; HTreeNode* l, * r;
	HuffmanTab re; HuffmanTabItem* item;
	int j;
	HTree = new HTreeNode[cr.Count];
	for (int i = 0, j = 0; i < cr.Count; i++)
	{
		while (cr.results[j].weight == 0)
		{
			j++;
		}
		HTree[i].weight = cr.results[j].weight;
		HTree[i].word = cr.results[j].word;
		HTree[i].lCr = nullptr;
		HTree[i].rCr = nullptr;
		j++;
	}


	Heap_Init(HTree, cr.Count);
	for (int i = 0; i < cr.Count - 1; i++)
	{
		l = new HTreeNode();
		l->lCr = HTree[0].lCr;
		l->rCr = HTree[0].rCr;
		l->weight = HTree[0].weight;
		l->word = HTree[0].word;
		HTree[0] = HTree[cr.Count - i - 1];
		Heap_Init(HTree, cr.Count - i - 1);
		r = new HTreeNode();
		r->lCr = HTree[0].lCr;
		r->rCr = HTree[0].rCr;
		r->weight = HTree[0].weight;
		r->word = HTree[0].word;
		HTree[0].lCr = l; HTree[0].rCr = r;
		HTree[0].weight = l->weight + r->weight;
		HTree[0].word = 0;
		Heap_Init(HTree, cr.Count - i - 1);
	}

	re.count = cr.MaxLenth;
	item = new HuffmanTabItem[cr.MaxLenth];
	re.Tree = HTree;
	re.Items = item;
	for (int i = 0; i < cr.MaxLenth; i++)
	{
		item[i].code = NULL;
	}
	Bits* b = new Bits(2);
	CreatTabByTree(HTree, b, item);
	delete b;
	j = 0;//maxcodelen
	for (int i = 0; i < cr.MaxLenth; i++)
	{
		if (item[i].code != NULL && item[i].code->Length > j)
		{
			j = item[i].code->Length;
		}
	}
	re.MaxCodeLen = j;
	return re;
}
void __EnCode_ToBits_Tab(HTreeNode* tree, Bits* result) {
	if (tree->lCr != NULL)
	{
		result->Write((char)Node_Non);
		__EnCode_ToBits_Tab(tree->lCr, result);
		__EnCode_ToBits_Tab(tree->rCr, result);
	}
	else
	{
		result->Write((char)tree->word);
	}
}
HUFFMANERROR V2Code::EnCode_ToBits(const char* str, Bits*& result)
{
#pragma region CreateTab
	CounterResult cr = DoCount(str);
	HuffmanTab tb = CreatHuffmanTab(cr);
	ShowHuffmanTab(tb); Log("\n");
	Destory(cr);
#pragma endregion


	result = new Bits(tb.count * 2 + 1 + strlen(str) * tb.MaxCodeLen + 1);
#pragma region Tab
	if (tb.Tree == 0)
	{
		str = new char{ '\0' };
		return 0;
	}
	__EnCode_ToBits_Tab(tb.Tree, result);
	result->Write((char)TabEnd);
#pragma endregion
	int pos_num = result->Pointer; int count = 0;
	result->Pointer += 32;

#pragma region Body
	while (*str != '\0')
	{
		if (tb.Items[*str].code == NULL)
		{
			Log("解析异常 该字符没有对应的编码");
			return HUFFMANERROR_NoThisCode;
		}
		result->Write(tb.Items[*str].code);
		count += tb.Items[*str].code->Length;
		str++;
	}
#pragma endregion
	result->Pointer = pos_num;
	result->Write(count);
	result->MoveFirst();
	Destory(tb);
	return  0;
}
#pragma endregion

#pragma region DeCode_FromBits
void __Load_HuffmanTab_FromBits(HTreeNode*& node, Bits* bits) {
	if (bits->Length == 0)
	{
		return;
	}
	else
	{
		char c = bits->ReadChar();
		if (c == Node_Non)
		{
			node = new HTreeNode();
			__Load_HuffmanTab_FromBits(node->lCr, bits);
			__Load_HuffmanTab_FromBits(node->rCr, bits);
		}
		else
		{
			node = new HTreeNode();
			node->word = c;
		}
	}
}
HuffmanTab Load_HuffmanTab_FromBits(Bits* bits) {
	HuffmanTab re; int j;
	re.count = 128;
	re.Items = new HuffmanTabItem[re.count];
	for (int i = 0; i < 128; i++)
	{
		re.Items[i].code = NULL;
	}
	//re.count = (strlen(str) + 1) / 2;
	__Load_HuffmanTab_FromBits(re.Tree, bits);
	
	if (bits->Eof())
	{
		Destory(re.Tree);
		re.Tree = NULL;
		return re;
	}
	Bits* temp = new Bits(2);
	CreatTabByTree(re.Tree, temp, re.Items);
	delete temp;
	j = 0;//maxcodelen
	for (int i = 0; i < 128; i++)
	{
		if (re.Items[i].code&&re.Items[i].code->Length > j)
		{
			j = re.Items[i].code->Length;
		}
	}
	re.MaxCodeLen = j;
	return re;
}
HUFFMANERROR V2Code::DeCode_FromBits(Bits* bits, char*& result)
{
	if (result != NULL)
	{
		delete[] result;
	}
	HuffmanTab tb = Load_HuffmanTab_FromBits(bits);
	if (tb.Tree == NULL || bits->ReadChar() != TabEnd)
	{
		return HUFFMANERROR_TabError;
	}

	int length = bits->ReadInt32();
	HTreeNode* node;
	char* re = new char[length + 1];//TODO
	char* str = re;
	bits->Length = bits->Pointer + length;
	while (!bits->Eof())
	{
		node = tb.Tree;
		while (node->lCr != NULL && !bits->Eof())
		{
			bool b = bits->ReadBit();
			if (!b)
			{
				node = node->lCr;
			}
			else
			{
				node = node->rCr;
			}
		}
		*str = node->word;
		str++;
	}
	*str = 0;
	result = re;
	return 0;
}
#pragma endregion


void V2Code::ShowHuffmanTab(HuffmanTab tab)
{
	Log("编码最大长度 "); Log(tab.MaxCodeLen); Log("\n");
	for (int i = 0; i < tab.count; i++)
	{
		if (tab.Items[i].code > 0)
		{
			Log("原有的值:  ");
			Log(tab.Items[i].origin);
			Log("  编码后的值:");
			tab.Items[i].code->Show();
			Log("  编码长度:  ");
			Log(tab.Items[i].code->Length);
			Log("\n");
		}
	}
}





