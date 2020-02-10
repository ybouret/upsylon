//!\file
#ifndef Y_STRING_ESMA_INCLUDED
#define Y_STRING_ESMA_INCLUDED 1

#include "y/os/platform.hpp"
 namespace upsylon {

    namespace core {

        struct esma
        {


            //! build offset table
            /**
             k[m+1], x[m], m>=0
             */
            template <typename T> static inline
            void build( unit_t *k, const T x[], const unit_t m) throw()
            {
                assert(0!=k);
                assert(0<=m);
                assert(!(0==x&&m>0));
                k[0]=-1;
                for(unit_t i=0,j=-1;i<m;++i,++j,k[i]=j)
                {
                    while( (j>=0) && (x[i]!=x[j]) ) j = k[j];
                }
            }

            //! find all offset of pattern x[m] in string y[n]
            /**
             use the precomputed table k[m] for x[n]
             bool proc(index)
             */
            template <typename T, typename FUNC> static inline
            size_t find(const T         y[], const unit_t n,
                        const T         x[], const unit_t m,
                        const unit_t    k[],
                        FUNC           &proc,
                        const size_t    countMax=0)
            {
                const bool   check = (countMax>0);
                size_t       count = 0;
                unit_t       i     = 0;
                unit_t       j     = 0;

                while (j<n)
                {
                    //while( (i > -1) && (x[i] != y[j]) ) i = k[i];
                    while( (i >= 0) && (x[i] != y[j]) ) i = k[i];
                    ++i; ++j;
                    if(i>=m)
                    {
                        const size_t res = (j-i);
                        if( !proc(res) ) return count;                    //-- early return
                        ++count;
                         if( check && (count >= countMax) ) return count; //-- global return
                        i = k[i];
                    }
                }
                return count;
            }


            
            //! build offset table
            /**
             k[m+1], x[m], m>=0
             */
            template <typename T> static inline
            void check_build( unit_t *k, const T x[], const unit_t m) throw()
            {
                assert(0!=k);
                assert(0<=m);
                assert(!(0==x&&m>0));
                k[0]=-1;
                for(unit_t i=0,j=-1;i<m;++i,++j,k[i]=j)
                {
                    while( (j>=0) && (x[ check_i(i,m) ]!=x[ check_j(j,m) ]) ) j = k[j];
                }
            }

        private:
            static unit_t check_i(const unit_t i, const unit_t m);
            static unit_t check_j(const unit_t j, const unit_t m);
            
        };

        
    }

}

#endif

