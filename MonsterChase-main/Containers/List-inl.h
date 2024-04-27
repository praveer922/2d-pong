
#include <assert.h>

template<typename T>
int Count(struct _LinkNode<T> * i_pList)
{
	int count = 0;

	while (i_pList)
	{
		count++;
		i_pList = i_pList->pNext;
	}

	return count;
}

template<typename T>
struct _LinkNode<T> * GetAt(struct _LinkNode<T> * i_pList, int i_ItemNumber)
{
	while (i_ItemNumber--)
	{
		assert(i_pList);
		i_pList = i_pList->pNext;
	}

	return i_pList;
}

template<typename T>
void RemoveAt(struct _LinkNode<T> * & i_pList, int i_ItemNumber)
{
	struct _LinkNode<T> ** i_ppNode = &i_pList;

	while (i_ItemNumber--)
	{
		assert(*i_ppNode);
		i_ppNode = &(*i_ppNode)->pNext;
	}

	assert(*i_ppNode);

	struct _LinkNode<T> * pToRemove = *i_ppNode;
	*i_ppNode = (*i_ppNode)->pNext;

	delete pToRemove;
}
