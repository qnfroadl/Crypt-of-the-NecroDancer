#pragma once

#include "GameObject.h"
#include <queue>

/*
	GameObject들을 Pool에서 관리하여 
	최초 로딩 이후에 New, Delete를 최소화 시킨다.
*/

template <typename T>
class ObjectPool
{

private:
	size_t size;
	vector<T*> pool;

public:
	ObjectPool(size_t size);

	T* GetPoolObject();					// 풀에 있거나, 없으면 생성하여 오브젝트를 리턴
	void ReleaseObject(T*);			// 사용완료한 오브젝트를 다시 풀에 보관.
	void AddObject(size_t size);

};


template<typename T>
inline ObjectPool<T>::ObjectPool(size_t _size)
	:size(0)
{
	AddObject(_size);
}

template<typename T>
inline T* ObjectPool<T>::GetPoolObject()
{
	T* obj = nullptr;
	if (pool.empty())
	{
		AddObject(size);
	}
	
	obj = pool.back();
	pool.pop_back();
	
	return obj;
}

template<typename T>
inline void ObjectPool<T>::ReleaseObject(T* obj)
{
	if (nullptr != obj)
	{
		pool.push_back(obj);
	}
}

template<typename T>
inline void ObjectPool<T>::AddObject(size_t _size)
{
	this->size += _size;
	pool.reserve(this->size);
	for (size_t i = 0; i < _size; i++)
	{
		pool.push_back(new T());
	}
}