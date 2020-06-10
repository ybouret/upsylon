
#include "y/spade/vtk/format.hpp"
#include "y/associative/be-key.hpp"
#include "y/type/spec.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Spade {

        namespace VTK {

#if 0
            Format:: Format() : db()
            {
            }

            Format:: ~Format() throw()
            {
            }

            void Format:: use(const std::type_info &tid, const string &fmt)
            {
                static const char fn[] = "VTK::Format::use: ";

                const be_key     k = tid;
                const string    *p = db.search_by(k);
                const type_spec &t = type_spec_for(tid);
                if( p )
                {
                    if( *p != fmt ) throw exception("%sdifferent formats for <%s>",fn,*t.name());
                }
                else
                {
                    if( !db.insert_by(k,fmt) ) throw exception("%sunexpected failure for <%s>",fn,*t.name());
                }
            }

            void Format:: use(const std::type_info &tid, const char *fmt)
            {
                const string _(fmt);
                use(tid,_);
            }

#endif
            
        }

    }

}
