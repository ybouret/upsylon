
#include "y/spade/vtk/formats.hpp"
#include "y/associative/be-key.hpp"
#include "y/type/spec.hpp"
#include "y/exception.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace Spade {

        namespace VTK {

            Format:: Format(const unsigned n) throw() : components(n),
            isScalar(1==components),
            isVector(!isScalar)
            {
                assert(components>0);
            }

            Format:: ~Format() throw()
            {
                _bzset(components);
            }

            

            PrimaryFormat:: PrimaryFormat(const char  *fmt) :
            Format(1),
            format(fmt)
            {

            }

            PrimaryFormat:: ~PrimaryFormat() throw()
            {
            }

            namespace {

                template <typename T,typename V>
                class _Format : public PrimaryFormat
                {
                public:
                    explicit _Format(const char *fmt) :
                    PrimaryFormat(fmt) {}

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
if(!pdb.search_by(k)) {                     \
const PrimaryFormat::Handle p = new _Format<TYPE,CAST>(FMT);\
if(!aliasing::_(pdb).insert_by(k,p)) throw exception("%sunexpected failure for <%s>",fn,*t.name());\
}\
} while(false)

#define Y_SPADE_VTK_TUPLE(TUPLE,TYPE) do { \
const type_spec &t = type_spec_of<TYPE>(); \
const be_key     k = t;\
const PrimaryFormat::Handle *ppF = pdb.search_by(k);\
if(!ppF) throw exception("%sno primary format for <%s>",fn,*t.name());\
const Format::Handle F = new TupleFormat<TUPLE,TYPE>(**ppF);          \
const type_spec  &tt   = type_spec_of< TUPLE<TYPE> >();               \
const be_key      tk   = tt;\
if(!aliasing::_(fdb).insert_by(tk,F)) throw exception("%sunexpected failure for <%s>",fn,*tt.name());\
} while(false)

            static const char fn[] = "VTK::Formats: ";

            Formats:: Formats() : fdb(), pdb()
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

            Formats:: ~Formats() throw()
            {
            }

            string & Formats:: formatString(const std::type_info &tid) const
            {
                const type_spec &t = type_spec_for(tid);
                const be_key     k = t;
                const PrimaryFormat::Handle *ppF = pdb.search_by(k);
                if(!ppF) throw exception("%sno primary <%s>", fn, *t.name());
                return aliasing::_((**ppF).format);
            }

            const Format & Formats:: get(const std::type_info &tid) const
            {
                const type_spec &t = type_spec_for(tid);
                const be_key     k = t;
                const PrimaryFormat::Handle *ppF = pdb.search_by(k);
                if(ppF)
                {
                    return **ppF;
                }
                else
                {
                    const Format::Handle *ppF_ = fdb.search_by(k);
                    if(!ppF_) throw exception("%sno format for <%s>",fn,*t.name());
                    return **ppF_;
                }
            }




        }

    }

}
