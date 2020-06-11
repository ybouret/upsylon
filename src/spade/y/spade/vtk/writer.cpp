
#include "y/spade/vtk.hpp"
#include "y/type/spec.hpp"
#include "y/associative/be-key.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Spade {

        vtk::Writer:: Writer(const unsigned dim) throw() :
        components(dim)
        {
            assert(components>0);
        }

        vtk::Writer:: ~Writer() throw()
        {
            _bzset(components);
        }

#define Y_VTK_TUPLES(TUPLE,TYPE) do {                             \
const IWriter    w = new Tuples<TUPLE,TYPE>( getNative<TYPE>() ); \
const type_spec &t = type_spec_of< TUPLE<TYPE> >();               \
const be_key     k =  t;                                          \
if(!writers.insert_by(k,w))                                       \
throw exception("%sfailure to insert <%s>", fn, *t.name());       \
} while(false)

        static const char fn[] = "vtk::Writers: ";

        void vtk:: registerWriters()
        {
            Y_VTK_TUPLES(complex,float);
            Y_VTK_TUPLES(complex,double);

            Y_VTK_TUPLES(point2d,float);
            Y_VTK_TUPLES(point3d,float);

            Y_VTK_TUPLES(point2d,double);
            Y_VTK_TUPLES(point3d,double);

        }


        const vtk::Writer & vtk::getWriter(const std::type_info &tid) const
        {
            const type_spec &t = type_spec_for(tid);
            const be_key     k = t;
            const INative   *n = natives.search_by(k);
            if(n)
            {
                return **n;
            }
            else
            {
                const IWriter *w = writers.search_by(k);
                if(!w)
                {
                    throw exception("%s no <%s>",fn,*t.name());
                }
                return **w;
            }
        }
    }

}
