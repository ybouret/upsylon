
#include "y/geometry/iso2d/levels.hpp"
#include "y/exception.hpp"
#include "y/type/bzset.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso2D {

            static const char Fn[] = "Iso2D::LevelSet: ";

            LevelSet:: LevelSet() throw()
            {
            }

            LevelSet:: ~LevelSet() throw()
            {
            }

            void LevelSet:: create(const size_t n)
            {
                std::cerr << "create level_set" << std::endl;
                free();
                ensure(n);
                for(size_t k=1;k<=n;++k)
                {
                    const Level l = new Level_(k);
                    if( !insert(l) )
                    {
                        throw exception("%sinsert#%u failure", Fn,unsigned(k) );
                    }
                }
                assert(size()==n);
            }

            void LevelSet::  check() const
            {
                for( const_iterator it=begin();it!=end();++it)
                {
                    (**it).check();
                }
            }

            void LevelSet:: buildLines(const bool keep)
            {
                for( iterator it=begin();it!=end();++it)
                {
                    (**it).buildLines(keep);
                }
            }
            
        }

    }

}
