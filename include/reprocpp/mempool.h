#ifndef MOL_DEF_GUARD_DEFINE_REPRO_CPP_MEMPOOL_DEF_GUARD_
#define MOL_DEF_GUARD_DEFINE_REPRO_CPP_MEMPOOL_DEF_GUARD_

#include <memory>
#include <type_traits>

namespace repro {
namespace impl {

    template<size_t S>
    class MemPool
    {
    public:
        static constexpr size_t size = S;

        struct Node
        {
            Node() : next(nullptr) {}

            Node* next;
        };

        MemPool()
        {}

        void* alloc()
        {
            if (freestore_.next == nullptr)
            {
                void* n = std::malloc(size);
                return n;
            }

            Node* p = freestore_.next;
            freestore_.next = p->next;
            return p;
        }

        void free(void* v)
        {
            Node* oldhead = freestore_.next;
            Node* newhead = (Node*)v;
            newhead->next = oldhead;
            freestore_.next = newhead;
        }

    private:

        Node freestore_;
    };


    template<size_t S>
    MemPool<S>& mempool()
    {
        static MemPool<S> mem;
        return mem;
    };


    template <class T>
    class mempool_allocator
    {
    public:
        using value_type = T;

        std::allocator<T> fallback;

        mempool_allocator() noexcept
        {}

        template <class U> mempool_allocator(mempool_allocator<U> const&) noexcept
        {}

        template<typename U>
        struct rebind
        {
            typedef mempool_allocator<U> other;
        };

        value_type* allocate(std::size_t n)
        {
            if (n > 1)
            {
                return fallback.allocate(n);
            }
            return static_cast<value_type*>(mempool<sizeof(value_type)>().alloc());
        }

        void deallocate(value_type* p, std::size_t n) noexcept
        {
            if (n > 1)
            {
                return fallback.deallocate(p, n);
            }
            mempool<sizeof(value_type)>().free(p);
        }
    };

    template <class T, class U>
    bool operator==(mempool_allocator<T> const&, mempool_allocator<U> const&) noexcept
    {
        return true;
    }

    template <class T, class U>
    bool operator!=(mempool_allocator<T> const& x, mempool_allocator<U> const& y) noexcept
    {
        return !(x == y);
    }

    template<class T>
    impl::mempool_allocator<T>& mempool()
    {
        static impl::mempool_allocator<T> mp;
        return mp;
    }

}} // end namespaces

#endif

