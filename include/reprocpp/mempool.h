#ifndef DEFINE_MOL_PROMISE_MEMPOOL_DEF_GUARD_DEFINE_
#define DEFINE_MOL_PROMISE_MEMPOOL_DEF_GUARD_DEFINE_

#include "reprocpp/future.h"

/*
 * promisestate mempool
 * 
 */

namespace repro     {


template<size_t S>
class MemPool
{
public:
	static constexpr size_t size = S;

	struct Node
	{
		Node(): next(nullptr) {}

		Node* next;
	};

	MemPool()
	{}

	void* alloc()
	{
		if ( freestore_.next == nullptr)
		{
			void * n = std::malloc(size);
			return n; 
		}

		Node* p = freestore_.next;		
		freestore_.next = p->next;
		return p;
	}

	void free(void* v)
	{
		Node* oldhead = freestore_.next;
		Node* newhead = (Node*) v;
		newhead->next = oldhead;
		freestore_ .next = newhead;
	}

private:

  Node freestore_;
};


template<size_t S>
inline MemPool<S>& mempool()
{
	static MemPool<S> mem;
	return mem;
};


} // end namespace org


#endif


