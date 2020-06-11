
#include "y/spade/vtk/writers.hpp"
#include "y/associative/be-key.hpp"
#include "y/type/spec.hpp"
#include "y/exception.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace Spade {

        namespace VTK {

            Writer:: Writer(const unsigned n) throw() : components(n),
            isScalar(1==components),
            isVector(!isScalar)
            {
                assert(components>0);
            }

            Writer:: ~Writer() throw()
            {
                _bzset(components);
            }

            

            NativeWriter:: NativeWriter(const char  *fmt) :
            Writer(1),
            format(fmt)
            {

            }

            NativeWriter:: ~NativeWriter() throw()
            {
            }

            namespace {

                template <typename T,typename V>
                class _Format : public NativeWriter
                {
                public:
                    explicit _Format(const char *fmt) :
                    NativeWriter(fmt) {}

                    virtual ~_Format() throw()
                    {
                    }

                    virtual ios::ostream & write( ios::ostream &fp, const void *addr) const
                    {
                        assert(addr);
                        const T &input = *static_cast<const T *>(addr);
                        const V  value = static_cast<V>(input);
                        fp(*format,value);
                        return fp;
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(_Format);
                };

            }

#define Y_SPADE_VTK(TYPE,CAST,FMT) do {     \
const type_spec & t = type_spec_of<TYPE>(); \
const be_key      k = t;                    \
if(!ndb.search_by(k)) {                     \
const NativeWriter::Handle p = new _Format<TYPE,CAST>(FMT);\
if(!aliasing::_(ndb).insert_by(k,p)) throw exception("%sunexpected failure for <%s>",fn,*t.name());\
}\
} while(false)

#define Y_SPADE_VTK_TUPLE(TUPLE,TYPE) do { \
const type_spec &t = type_spec_of<TYPE>(); \
const be_key     k = t;\
const NativeWriter::Handle *ppF = ndb.search_by(k);\
if(!ppF) throw exception("%sno primary format for <%s>",fn,*t.name());\
const Writer::Handle F  = new TupleWriter<TUPLE,TYPE>(**ppF);          \
const type_spec     &tt = type_spec_of< TUPLE<TYPE> >();               \
const be_key         tk = tt;\
if(!aliasing::_(wdb).insert_by(tk,F)) throw exception("%sunexpected failure for <%s>",fn,*tt.name());\
} while(false)

            static const char fn[] = "VTK::Writers: ";

            Writers:: Writers() : wdb(), ndb()
            {

                Y_SPADE_VTK(short, long, "%ld");
                Y_SPADE_VTK(int,   long, "%ld");
                Y_SPADE_VTK(long,  long, "%ld");

                Y_SPADE_VTK(unsigned short, unsigned long, "%lu");
                Y_SPADE_VTK(unsigned int,   unsigned long, "%lu");
                Y_SPADE_VTK(unsigned long,  unsigned long, "%lu");

                Y_SPADE_VTK(int16_t, long, "%ld");
                Y_SPADE_VTK(int32_t, long, "%ld");

                Y_SPADE_VTK(uint16_t, unsigned long, "%lu");
                Y_SPADE_VTK(uint32_t, unsigned long, "%lu");
                
                Y_SPADE_VTK(size_t, unsigned long, "%lu");
                Y_SPADE_VTK(unit_t, long,          "%ld");


                Y_SPADE_VTK(float,float,"%.15g");
                Y_SPADE_VTK(double,double,"%.15g");

                Y_SPADE_VTK_TUPLE(complex,float);
                Y_SPADE_VTK_TUPLE(complex,double);

                Y_SPADE_VTK_TUPLE(point2d,float);
                Y_SPADE_VTK_TUPLE(point3d,float);

                Y_SPADE_VTK_TUPLE(point2d,double);
                Y_SPADE_VTK_TUPLE(point3d,double);

            }

            Writers:: ~Writers() throw()
            {
            }

            string & Writers:: formatString(const std::type_info &tid) const
            {
                const type_spec &t = type_spec_for(tid);
                const be_key     k = t;
                const NativeWriter::Handle *ppF = ndb.search_by(k);
                if(!ppF) throw exception("%sno primary <%s>", fn, *t.name());
                return aliasing::_((**ppF).format);
            }

            const Writer & Writers:: get(const std::type_info &tid) const
            {
                const type_spec &t = type_spec_for(tid);
                const be_key     k = t;
                const NativeWriter::Handle *ppF = ndb.search_by(k);
                if(ppF)
                {
                    return **ppF;
                }
                else
                {
                    const Writer::Handle *ppF_ = wdb.search_by(k);
                    if(!ppF_) throw exception("%sno format for <%s>",fn,*t.name());
                    return **ppF_;
                }
            }




        }

    }

}
