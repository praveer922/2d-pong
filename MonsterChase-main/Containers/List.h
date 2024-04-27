#pragma once

template<typename T>
struct _LinkNode
{
	T 					Item;
	struct _LinkNode *	pNext;

	_LinkNode(const T & i_Item, struct _LinkNode * i_Next = nullptr) :
		Item(i_Item),
		pNext(i_Next)
	{
	}

	_LinkNode(const _LinkNode & i_other) = delete;
};

template<typename T>
int Count(struct _LinkNode<T> * i_pList);

template<typename T>
struct _LinkNode<T> * GetAt(struct _LinkNode<T> * i_pList, int i_ItemNumber);

template<typename T>
void RemoveAt(struct _LinkNode<T> * & i_pList, int i_ItemNumber);

#include "List-inl.h"