//! \file
#ifndef Y_COMPARISON_INCLUDED
#define Y_COMPARISON_INCLUDED 1

#include "y/os/platform.hpp"
#include <iostream>
namespace upsylon
{

    //! handle some comparison function
    class comparison
    {
    public:
        //! lexicographic comparison
        template <typename T> static inline
        int lexicographic(const T *sa, const size_t na, const T *sb, const size_t nb) throw()
        {
            return ( (na<=nb) ? __lexicographic<T>(sa,na,sb,nb) : - __lexicographic<T>(sb,nb,sa,na) );
        }

        //! convert signed value
        static int normalize( const int ans ) throw()
        {
            return (ans < 0) ? -1 : ( (0<ans) ? 1 : 0 );
        }

        //! check same comparison signed
        static int are_same_results( const int a, const int b ) throw()
        {
            switch( normalize(a) )
            {
                case -1: return (-1==normalize(b));
                case  1: return ( 1==normalize(b));
                default:
                    break;
            }
            assert(0==normalize(a));
            return (0==normalize(b));
        }

        //! compare by increasing value
        template <typename T> static inline
        int increasing( const T &lhs, const T &rhs ) throw()
        {
            if(lhs<rhs)
            {
                return -1;
            }
            else if(rhs<lhs)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }

        //! C-style
        template <typename T> static inline
        int __increasing( const void *L, const void *R ) throw()
        {
            assert(L);assert(R);
            const T &lhs = *(const T *)L;
            const T &rhs = *(const T *)R;
            if(lhs<rhs)
            {
                return -1;
            }
            else if(rhs<lhs)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }


        //! compare by decreasing value
        template <typename T> static inline
        int decreasing( const T &lhs, const T &rhs ) throw()
        {
            if(lhs<rhs)
            {
                return 1;
            }
            else if(rhs<lhs)
            {
                return -1;
            }
            else
            {
                return 0;
            }
        }

    private:
        template <typename T> static inline
        int __lexicographic(const T     *small_data,
                            const size_t small_size,
                            const T     *large_data,
                            const size_t large_size) throw()
        {
            assert(small_size<=large_size);
            for(size_t i=0; i<small_size;++i)
            {
                const T s = small_data[i];
                const T l = large_data[i];
                if(s<l)
                {
                    return -1;
                }
                else if(l<s)
                {
                    return 1;
                }
                // else continue
            }
            return (small_size<large_size) ? -1 : 0;
        }
    };

}


#endif


