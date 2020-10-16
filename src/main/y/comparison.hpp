//! \file
#ifndef Y_COMPARISON_INCLUDED
#define Y_COMPARISON_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    //__________________________________________________________________________
    //
    //
    //! handle some comparison function
    //
    //__________________________________________________________________________
    class comparison
    {
    public:

        //______________________________________________________________________
        //
        //! named result
        //______________________________________________________________________
        enum result
        {
            eq, //!< ==
            lt, //!< <
            gt  //!< '>'
        };
        //______________________________________________________________________
        //
        //! increasing lexicographic comparison any block sizes
        //______________________________________________________________________
        template <typename T> static inline
        int lexicographic(const T *sa, const size_t na,
                          const T *sb, const size_t nb) throw()
        {
            return ( (na<=nb) ? __lexicographic<T>(sa,na,sb,nb) : - __lexicographic<T>(sb,nb,sa,na) );
        }

        //______________________________________________________________________
        //
        //! increasing lexicographic comparison of same block sizes
        //______________________________________________________________________
        template <typename T> static inline
        int increasing_lexicographic(const T *a, const T *b, size_t n) throw()
        {
            assert( !( (0==a) && (n>0) ) );
            assert( !( (0==b) && (n>0) ) );
            while(n-->0)
            {
                const T &lhs = *a;
                const T &rhs = *b;
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
                    ++a;
                    ++b;
                }
            }
            return 0; // the same!
        }

        //______________________________________________________________________
        //
        //! decreasing lexicographic comparison of same block sizes
        //______________________________________________________________________
        template <typename T> static inline
        int decreasing_lexicographic(const T *a, const T *b, size_t n) throw()
        {
            return increasing_lexicographic(b,a,n);
        }

        //______________________________________________________________________
        //
        //! convert signed value
        //______________________________________________________________________
        static int normalize(const int ans) throw()
        {
            return (ans < 0) ? -1 : ( (0<ans) ? 1 : 0 );
        }

        //______________________________________________________________________
        //
        //! check same comparison signed
        //______________________________________________________________________
        static int are_same_results(const int a, const int b) throw()
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

        //______________________________________________________________________
        //
        //! compare by increasing value
        //______________________________________________________________________
        template <typename T> static inline
        int increasing(const T &lhs, const T &rhs) throw()
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

        //______________________________________________________________________
        //
        //! C-style
        //______________________________________________________________________
        template <typename T> static inline
        int __increasing(const void *L, const void *R) throw()
        {
            assert(L);assert(R);
            const T &lhs = *(const T *)L;
            const T &rhs = *(const T *)R;
            return increasing(lhs,rhs);
        }


        //______________________________________________________________________
        //
        //! compare by decreasing value
        //______________________________________________________________________
        template <typename T> static inline
        int decreasing(const T &lhs, const T &rhs) throw()
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

        //______________________________________________________________________
        //
        //! comparison by decreasing absolute value
        //______________________________________________________________________
        template <typename T> static inline
        int decreasing_abs(const T &lhs, const T &rhs)
        {
            const T al = (lhs<0?-lhs:lhs);
            const T ar = (rhs<0?-rhs:rhs);
            return decreasing<T>(al,ar);
        }
       
        //______________________________________________________________________
        //
        //! comparison of increasing addresses
        //______________________________________________________________________
        static int increasing_addresses(const void *lhs, const void *rhs) throw();
        
        //______________________________________________________________________
        //
        //! comparison of decreasing addresses
        //______________________________________________________________________
        static int decreasing_addresses(const void *lhs, const void *rhs) throw();
        
        
        
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


