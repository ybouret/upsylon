
//! \file
#ifndef Y_EUCLIDEAN_ARC_STANDARD_INCLUDED
#define Y_EUCLIDEAN_ARC_STANDARD_INCLUDED 1

#include "y/math/euclidean/arc.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            template <typename T, template <class> class VTX>
            class StandardArc : public Arc<T,VTX>
            {
            public:
                Y_EUCLIDEAN_ARC_TYPES();
                inline explicit StandardArc() throw() {}
                inline virtual ~StandardArc() throw() {}

                inline virtual void ensure(size_t n)
                {
                    aliasing::_(this->nodes).ensure(n);
                    if(n>1)
                    {
                        aliasing::_(this->segments).ensure(n-1);
                    }
                }
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(StandardArc);

                inline virtual void add(const SharedPoint &p )
                {
                    this->pushBack(p);
                    if(this->nodes.size()>1)
                    {
                        try { this->pushGrowing(); }
                        catch(...){
                            this->popBack();
                            throw;
                        }
                    }
                    assert(this->segments.size()==this->nodes.size()-1);
                }



            };

        }

    }

}

#endif
