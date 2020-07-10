
#include "y/memory/grooves.hpp"
#include "y/type/aliasing.hpp"
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


        void grooves:: free() throw()
        {
            size_t i=num;
            while(i-- > 0 )
            {
                grv[i].free();
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
                free();
                assert(num);
                object:: operator delete(grv,num*sizeof(groove));
                itm=0;
                grv=0;
                num=0;
            }
        }

        void grooves:: acquire(const size_t n)
        {
            if(n!=num)
            {
                release();
                grv = static_cast<groove *>(object:: operator new(n*sizeof(groove)));
                num = n;
                itm = grv-1;
            }
            else
            {
                free();
            }
            
        }

    }
}

