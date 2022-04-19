#ifndef __CountWord__ 
#define __CountWord__
typedef struct {
	char word;
	int weight;
}WordWeight;

typedef struct {
	int Count;
	int MaxLenth;
	WordWeight* results;
}CounterResult;
CounterResult DoCount(const char* str);
void Destory (CounterResult  cr);
#endif // !__CountWord__


