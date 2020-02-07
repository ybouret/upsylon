//!\file
#ifndef Y_STRING_ESMA_INCLUDED
#define Y_STRING_ESMA_INCLUDED 1

#include "y/string/basic.hpp"
#include "y/code/round.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace core {

        template <typename T>
        class esma
        {
        public:
            inline explicit esma(const T      motif[],
                                 const size_t count) :
            m(0), x(0), k(0), l(0)
            {
                prepare(motif,count);
            }

            inline explicit esma( const T motif[] ) :
            m(0), x(0), k(0), l(0)
            {
                prepare(motif, length_of(motif) );
            }

            inline explicit esma( const string<T> &s ) :
            m(0), x(0), k(0), l(0)
            {
                prepare(*s,s.size());
            }

            inline virtual ~esma() throw()
            {
                static memory::allocator &mgr = core::string_allocator_location();
                void *p = (void*)x;
                mgr.release(p,aliasing::_(l));
                aliasing::_(m) = 0;
                x=0;
                k=0;
            }

            inline size_t find( const T y[], const size_t n, const size_t countMax=0)
            {
                size_t          count = 0;
                const ptrdiff_t n_    = n;
                ptrdiff_t       i     = 0;
                ptrdiff_t       j     = 0;

                while (j < n_) {
                    while( (i > -1) && (x[i] != y[j]) ) i = k[i];
                    ++i;
                    ++j;
                    if ( i >= m)
                    {
                        const size_t res = (j-i);
                        std::cerr << "@" << res << std::endl;
                        // if( ! on_find( res, args ) )return;
                        if( countMax>0 && count >= countMax ) return count; //-- global return
                        i = k[i];
                    }
                }
                return count;
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(esma);
            const ptrdiff_t  m;
            const T         *x;
            const ptrdiff_t *k;
            const size_t     l;

            void prepare(const T     *motif,
                         const size_t count)
            {
                
                static memory::allocator &mgr = core::string_allocator_instance();
                const size_t char_length = count * sizeof(T);
                const size_t indx_offset = Y_MEMALIGN(char_length);
                const size_t indx_length = count * sizeof(ptrdiff_t);
                void *p = mgr.acquire(  (aliasing::_(l) = indx_offset + indx_length) );
                x       = (T*)p;
                k       = (ptrdiff_t *)( static_cast<uint8_t*>(p) + indx_offset );
                aliasing::_(m) = count;
                memcpy(p,motif,char_length);


                ptrdiff_t *k_ = (ptrdiff_t *)k;
                ptrdiff_t  i = 0;
                ptrdiff_t  j = k_[0] = -1;
                while (i < m)
                {
                    while(j > -1 && x[i] != x[j])
                        j = k_[j];
                    ++i;
                    ++j;
                    if (x[i] == x[j])
                        k_[i] = k_[j];
                    else
                        k_[i] = j;
                }
                for(size_t i=0;i<count;++i)
                {
                    std::cerr << "x[" << i << "]='" << x[i] << "', k=" << k[i] << std::endl;
                }
            }

        };
    }

}

#endif

