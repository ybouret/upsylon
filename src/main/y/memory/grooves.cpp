
#include "y/memory/grooves.hpp"
#include "y/type/aliasing.hpp"
#include "y/object.hpp"
#include <cstring>

namespace upsylon {

    namespace memory {

        grooves:: ~grooves() throw()
        {
            if(num>0)
            {
                size_t i=num;
                while(i-- > 0)
                {
                    self_destruct(grv[i]);
                }
                object:: operator delete(grv, num*sizeof(groove) );
                grv=0;
                num=0;
            }
            else
            {
                assert(0==grv);
                assert(0==itm);
            }
        }

        grooves:: grooves() throw() :
        itm(0),
        grv(0),
        num(0)
        {
        }

        void grooves:: dismiss() throw()
        {
            size_t i=num;
            while(i-- > 0 )
            {
                grv[i].dismiss();
            }
        }

        size_t grooves:: size() const throw()
        {
            return num;
        }

        void grooves:: release() throw()
        {
            if(grv)
            {
                const size_t len = num * sizeof(groove);
                while(num>0)
                {
                    self_destruct( grv[--num] );
                }
                assert(num<=0);
                object:: operator delete(grv,len);
                grv=0;
                itm=0;
            }
        }

        void grooves:: acquire(const size_t n)
        {
            if(n!=num)
            {
                release();
                grv = static_cast<groove *>(object:: operator new(n*sizeof(groove)));
                do {
                    size_t i=0;
                    while(i<num) {
                        new (grv+i) groove();
                        ++i;
                    }
                }
                while(false);
                num = n;
                itm = grv-1;
            }
            else
            {
                dismiss();
            }
            
        }

        groove       & grooves:: operator[](const size_t indx)       throw()
        {
            assert(indx>0);
            assert(indx<=size());
            return itm[indx];
        }

        const groove & grooves:: operator[](const size_t indx) const throw()
        {
            assert(indx>0);
            assert(indx<=size());
            return itm[indx];
        }

    }
}

