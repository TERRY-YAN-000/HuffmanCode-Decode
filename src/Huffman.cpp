#include "Huffman.h"
#include "Debug.h"

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
void Destory(HuffmanTab tb)
{
	delete[] tb.Items;

}
#pragma endregion

#pragma region EnCode_ToString
char* CodeToString(unsigned int code, int len) {
	char* re = new char[len + 1];
	re[len] = 0;
	int i = 0;
	while (code > 0)
	{
		re[len - 1 - i] = code % 2 + '0';
		code /= 2;
		i++;
	}
	for (; i < len; i++)
	{
		re[len - 1 - i] = '0';
	}
	return re;
}
void CreatTabByTree(HTreeNode* node, unsigned int code, int depth, HuffmanTabItem* item) {
	if (node->lCr == nullptr && node->rCr == nullptr)
	{
		if (item[node->word].codelen > 0)
		{
			Log("哈夫曼树异常:存在相同节点");
			while (true)
			{

			}
		}
		item[node->word].code = code;
		item[node->word].origin = node->word;
		item[node->word].codelen = depth;
		item[node->word].visible = CodeToString(code, depth);
	}
	else if (node->lCr != nullptr && node->rCr != nullptr)
	{
		CreatTabByTree(node->lCr, code * 2, depth + 1, item);
		CreatTabByTree(node->rCr, code * 2 + 1, depth + 1, item);
	}
	else
	{
		Log("\n哈夫曼树创建产生严重异常\n");
		while (true)
		{

		}
	}
}
HuffmanTab CreatHuffmanTab(CounterResult cr)
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

	re.count = cr.Count; re.MaxLen = cr.MaxLenth;
	item = new HuffmanTabItem[cr.MaxLenth];
	re.Tree = HTree;
	re.Items = item;
	for (int i = 0; i < cr.MaxLenth; i++)
	{
		item[i].codelen = 0;
	}
	CreatTabByTree(HTree, 0, 0, item);
	j = 0;//maxcodelen
	for (int i = 0; i < cr.MaxLenth; i++)
	{
		if (item[i].codelen > j)
		{
			j = item[i].codelen;
		}
	}
	re.MaxCodeLen = j;
	return re;
}
void __EnCode_ToString_Tab(HTreeNode* tree, char* c, int& pos) {
	if (tree->lCr != NULL)
	{
		c[pos++] = Node_Non;
		__EnCode_ToString_Tab(tree->lCr, c, pos);
		__EnCode_ToString_Tab(tree->rCr, c, pos);
	}
	else
	{
		c[pos++] = tree->word;
	}
	//c[pos] = tree->word;
}
HUFFMANERROR EnCode_ToString(const char* str, char*& result)
{
#pragma region CreateTab
	CounterResult cr = DoCount(str);
	HuffmanTab tb = CreatHuffmanTab(cr);
	ShowHuffmanTab(tb); Log("\n");
	Destory(cr);
#pragma endregion


	if (result != NULL)
	{
		delete[] result; result = NULL;
	}

	char* re = new char[tb.count * 2 + 1 + strlen(str) * tb.MaxCodeLen + 1];
	int pos = 0;
#pragma region Tab
	if (tb.Tree == 0)
	{
		str = new char{ '\0' };
		return 0;
	}
	__EnCode_ToString_Tab(tb.Tree, re, pos);
	re[pos++] = TabEnd;
	//re[pos++] = 0;
#pragma endregion

#pragma region Body
	while (*str != '\0')
	{

		if (tb.Items[*str].codelen <= 0)
		{
			Log("解析异常 该字符没有对应的编码");
			return HUFFMANERROR_NoThisCode;
		}

		/*strcpy_s(current, tb.Items[*str].codelen + 1, tb.Items[*str].visible);
		current += strlen(tb.Items[*str].visible);*/
		strcpy_s(re + pos, tb.Items[*str].codelen + 1, tb.Items[*str].visible);
		pos += strlen(tb.Items[*str].visible);
		str++;
	}
	re[pos] = 0;

#pragma endregion
	result = re;
	Destory(tb);
	return  0;
}
#pragma endregion

#pragma region DeCode_FromString
void __Load_HuffmanTab_FromString(HTreeNode*& node, const char*& str) {
	if (*str == 0)
	{
		return;
	}
	else if (*str == Node_Non)
	{
		node = new HTreeNode();
		str++;
		__Load_HuffmanTab_FromString(node->lCr, str);
		__Load_HuffmanTab_FromString(node->rCr, str);
	}
	else
	{
		node = new HTreeNode();
		node->word = *str++;
	}
}
HuffmanTab Load_HuffmanTab_FromString(const char* str) {
	HuffmanTab re; int j;
	re.MaxLen = 128;
	re.Items = new HuffmanTabItem[128];
	re.count = (strlen(str) + 1) / 2;
	__Load_HuffmanTab_FromString(re.Tree, str);
	if (*str=='\0')
	{
		Destory(re.Tree);
		re.Tree = NULL;
		return re;
	}
	CreatTabByTree(re.Tree, 0, 0, re.Items);
	j = 0;//maxcodelen
	for (int i = 0; i < 128; i++)
	{
		if (re.Items[i].codelen > j)
		{
			j = re.Items[i].codelen;
		}
	}
	re.MaxCodeLen = j;
	return re;
}
HUFFMANERROR DeCode_FromString(char* strbyte, char*& result)
{
	if (result != NULL)
	{
		delete[] result;
	}
	HuffmanTab tb = Load_HuffmanTab_FromString(strbyte);
	if (tb.Tree==NULL)
	{
		return HUFFMANERROR_TabError;
	}
	while (*strbyte != '\0' && *strbyte++ != TabEnd) {}
	if (*strbyte == '\0')
	{
		Log("DeCode_FromString:长度异常");
		return HUFFMANERROR_LengthError;
	}

	HTreeNode* node;
	char* re = new char[strlen(strbyte) + 1];
	char* str = re;
	while (*strbyte != 0 && *strbyte != '\n')
	{
		node = tb.Tree;
		while (node->lCr != NULL)
		{
			if (*strbyte == '0')
			{
				strbyte++;
				node = node->lCr;
			}
			else  if (*strbyte == '1')
			{
				strbyte++;
				node = node->rCr;
			}
			else {
				Log("编码含有非法字符");
				return HUFFMANERROR_CodeError;
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

//
//unsigned char* CodeToByte(unsigned int code, int len) {
//	int blen = len / 8 + (len % 8 != 0);
//	unsigned char* re = new unsigned char[blen];
//	for (int i = blen-1; i>=0; i--)
//	{
//		re[i] = code % 256;
//		code /= 256;
//	}
//	return re;
//}
//void __EnCode_ToString_Tab(HTreeNode* tree,unsigned char* c, int& pos) {
//	if (tree->lCr != NULL)
//	{
//		c[pos++] = Node_Non;
//		__EnCode_ToString_Tab(tree->lCr, c, pos);
//		__EnCode_ToString_Tab(tree->rCr, c, pos);
//	}
//	else
//	{
//		c[pos++] = tree->word;
//	}
//	c[pos] = tree->word;
//}
//HUFFMANERROR EnCode_ToString(const char* str, unsigned char*& result,int &len)
//{
//#pragma region CreateTab
//	CounterResult cr = DoCount(str);
//	HuffmanTab tb = CreatHuffmanTab(cr);
//	ShowHuffmanTab(tb); Log("\n");
//	Destory(cr);
//#pragma endregion
//
//
//	if (result != NULL)
//	{
//		delete[] result; result = NULL;
//	}
//
//	unsigned char* re = new unsigned char[tb.count * 2 + 1+4 + strlen(str) * tb.MaxByteCodeLen/8+1];
//	int pos = 0;
//#pragma region Tab
//	if (tb.Tree == 0)
//	{
//		str = new char{ '\0' };
//		return 0;
//	}
//	__EnCode_ToString_Tab(tb.Tree, re, pos);
//	re[pos++] = TabEnd;
//#pragma endregion
//	re[pos++]
//
//
//#pragma region Body
//	while (*str != '\0')
//	{
//
//		if (tb.Items[*str].codelen <= 0)
//		{
//			Log("解析异常 该字符没有对应的编码");
//			return HUFFMANERROR_NoThisCode;
//		}
//
//		/*strcpy_s(current, tb.Items[*str].codelen + 1, tb.Items[*str].visible);
//		current += strlen(tb.Items[*str].visible);*/
//		//strcpy_s(re + pos, tb.Items[*str].codelen + 1, tb.Items[*str].visible);
//		pos += strlen(tb.Items[*str].visible);
//		str++;
//	}
//	re[pos] = 0;
//
//#pragma endregion
//	result = re;
//	Destory(tb);
//	return  0;
//}
//



void ShowHuffmanTab(HuffmanTab tab)
{
	Log("编码最大长度"); Log(tab.MaxCodeLen); Log("\n");
	for (int i = 0; i < tab.MaxLen; i++)
	{
		if (tab.Items[i].codelen > 0)
		{
			Log("原有的值:  ");
			Log(tab.Items[i].origin);
			Log("  编码后的值:  ");
			Log(tab.Items[i].visible);
			Log("  编码后的值:  ");
			Log(int(tab.Items[i].code));
			Log("  编码长度:  ");
			Log(tab.Items[i].codelen);
			Log("\n");
		}
	}
}
HUFFMANERROR ShowHuffmanStr(const char* str)
{
	while (*str != '\0' && *str++ != TabEnd) {}
	if (*str == '\0')
	{
		Log("ShowHuffmanStr:长度异常");
		return HUFFMANERROR_LengthError;
	}
	Log(str);
	return 0;
}







