
#include "y/yap/prime/iterator.hpp"

namespace upsylon
{
    namespace yap
    {
        prime_iterator:: prime_iterator( library & _) throw() :
        ptr( & *_.p2 ),
        lib(_)
        {
        }

        prime_iterator:: ~prime_iterator() throw()
        {
            ptr = 0;
        }

        prime_iterator:: prime_iterator(const prime_iterator &other) throw() :
        ptr(other.ptr),
        lib(other.lib)
        {
            assert(ptr);
        }

        prime_iterator & prime_iterator:: operator=(const prime_iterator &other) throw()
        {
            ptr = other.ptr;
            return *this;
        }

        static inline const prime *update_library(library &lib)
        {
            Y_LOCK(lib.access);
            return & lib.prefetch();
        }

        static inline const prime *next_from(const prime *nxt, library &lib)
        {
            if(nxt)
            {
                return nxt;
            }
            else
            {
                return update_library(lib);
            }
        }


        void prime_iterator:: next()
        {
            assert(ptr);
            static const prime *p2 = & * lib.p2;
            static const prime *p3 = & * lib.p3;

            if(ptr==p2)
            {
                ptr=p3;
            }
            else
            {
                if(ptr==p3)
                {
                    ptr = next_from(lib.primes.head,lib);
                }
                else
                {
                    assert(ptr);
                    ptr = next_from(ptr->next,lib);
                }
            }
        }

        const prime & prime_iterator:: operator*()  const throw()
        {
            assert(ptr);
            return *ptr;
        }

        const prime * prime_iterator:: operator->()  const throw()
        {
            assert(ptr);
            return ptr;
        }

        prime_iterator & prime_iterator:: operator++()
        {
            next();
            return *this;
        }


        prime_iterator  prime_iterator:: operator++(int)
        {
            prime_iterator ans = *this;
            next();
            return ans;
        }

        void  prime_iterator:: next(size_t n)
        {
            while(n-- > 0 )
            {
                next();
            }
        }

        prime_iterator operator+(const size_t n,const prime_iterator &p)
        {
            prime_iterator it(p);
            it.next(n);
            return it;
        }

        prime_iterator operator+(const prime_iterator &p, const size_t n)
        {
            prime_iterator it(p);
            it.next(n);
            return it;
        }
    }

}
