#include "CountWord.h" 
#include <malloc.h>
#include<string>
#include "Debug.h"


void Show(CounterResult cr) {
	Log("  Cr.Count  "); 
	Log(cr.Count);
	Log("  Cr.MaxLenth  "); 
	Log(cr.MaxLenth);
	Log("\n");
	for (int i = 0; i < cr.MaxLenth; i++)
	{
		if (cr.results[i].weight!=0)
		{
			Log(cr.results[i].word);
			Log(":");
			Log(cr.results[i].weight);
			Log("   ");
		}
	}
	Log("\n");
}

CounterResult DoCount(const char* str)
{
	WordWeight* words;
	CounterResult re;
	char c;
	int len = strlen(str);re.MaxLenth = 128; re.Count = 0; re.results = NULL;

	//分配内存并初始化计数
	words = new WordWeight[128];
	if (words == NULL)
	{
		Log("异常:内存不足");
		while (true);
	}
	re.results = words;
	for (int i = 0; i < 128; i++)
	{
		re.results[i].word = i;
		re.results[i].weight = 0;
	}


	for (int i = 0; i < len; i++)
	{
		c = str[i];
		if (c > 0) {
			re.results[c].weight++;
			if (re.results[c].weight == 1)
			{
				re.Count++;
			}
		}
	}
	Show(re);


	return re;
}

void Destory(CounterResult cr)
{
	delete[] cr.results;
}
