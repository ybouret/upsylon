#include "y/geometry/contour2d.hpp"
#include "y/exception.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace geometry {

        static const char Fn[] = "contour2d::level_set: ";
        
        contour2d:: level_set:: level_set() throw()
        {
        }

        contour2d:: level_set:: ~level_set() throw()
        {
        }

        void contour2d:: level_set:: create(const size_t n)
        {
            std::cerr << "create level_set" << std::endl;
            free();
            ensure(n);
            for(size_t k=1;k<=n;++k)
            {
                const level l = new level_(k);
                if( !insert(l) )
                {
                    throw exception("%sinsert#%u failure", Fn,unsigned(k) );
                }
            }
            assert(size()==n);
        }

        void contour2d:: level_set::  check() const
        {
            for( const_iterator it=begin();it!=end();++it)
            {
                (**it).check();
            }
        }

        void contour2d:: level_set:: build_isolines(const bool keep)
        {
            for( iterator it=begin();it!=end();++it)
            {
                (**it).build_isolines(keep);
            }
        }



    }
}
