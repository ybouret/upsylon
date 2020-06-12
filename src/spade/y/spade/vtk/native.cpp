

#include "y/spade/vtk.hpp"
#include "y/type/spec.hpp"
#include "y/associative/be-key.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Spade {

        vtk::Native::  Native(const char *fmt) : Writer(), format(fmt) { }
        vtk::Native:: ~Native() throw() {}

        unsigned vtk::Native:: components() const throw()
        {
            return 1;
        }

        namespace {

            template <typename TYPE,typename CAST>
            class _Native : public vtk::Native
            {
            public:

                inline explicit _Native(const char *fmt) : vtk::Native(fmt) {}
                inline virtual ~_Native() throw() {}


            private:
                Y_DISABLE_COPY_AND_ASSIGN(_Native);
                inline virtual ios::ostream & write(ios::ostream &fp, const void *addr) const
                {
                    assert(addr);
                    const CAST value = static_cast<CAST>( *static_cast<const TYPE *>(addr) );
                    fp(*format,value);
                    return fp;
                }

                inline static const char *DataType()   throw()
                {
                    static const int IsFloat  = tl::index_of<tl::std_reals_list,float>::value;
                    static const int IsDouble = tl::index_of<tl::std_reals_list,double>::value;
                    static const int myIndex  = tl::index_of<tl::std_reals_list,TYPE>::value;
                    
                    switch( myIndex )
                    {
                        case IsFloat:  return vtk::TypeFloat;
                        case IsDouble: return vtk::TypeDouble;
                        default: break;
                    }
                    return vtk::TypeInt;
                }

                inline virtual const char *dataType() const throw()
                {
                    static const char * _ = DataType();
                    return _;
                }

            };

#define Y_VTK_NATIVE(TYPE,CAST,FMT) do {       \
const type_spec &t = type_spec_of<TYPE>();     \
const be_key     k = t;                        \
if(!natives.search_by(k)) {                    \
const INative w = new _Native<TYPE,CAST>(FMT); \
if(!natives.insert_by(k,w)) \
throw exception("%sunexpected failure for <%s>",fn, *t.name());\
}\
} while(false)
            
#define Y_VTK_NATIVE_AUTO(TYPE,FMT) Y_VTK_NATIVE(TYPE,TYPE,FMT)
            
        }

        static const char fn[] = "vtk::Natives: ";
        void vtk:: registerNatives()
        {
            Y_VTK_NATIVE(unsigned char,unsigned long, "%lu");
            Y_VTK_NATIVE(uint8_t,unsigned long, "%lu");

            Y_VTK_NATIVE(short,long, "%ld" );
            Y_VTK_NATIVE(int,  long, "%ld" );
            Y_VTK_NATIVE_AUTO(long, "%ld" );

            Y_VTK_NATIVE(unsigned short,unsigned long, "%lu" );
            Y_VTK_NATIVE(unsigned int,  unsigned long, "%lu" );
            Y_VTK_NATIVE_AUTO(unsigned long, "%lu" );

            Y_VTK_NATIVE(int16_t,long, "%ld" );
            Y_VTK_NATIVE(int32_t,long, "%ld" );

            Y_VTK_NATIVE(uint16_t,unsigned long, "%lu" );
            Y_VTK_NATIVE(uint32_t,unsigned long, "%lu" );

            Y_VTK_NATIVE(unit_t,long, "%ld" );
            Y_VTK_NATIVE(size_t,unsigned long, "%lu" );


            Y_VTK_NATIVE_AUTO(float,"%.15g");
            Y_VTK_NATIVE_AUTO(double,"%.15g");


        }

        const vtk::Native & vtk:: getNative(const std::type_info &tid) const
        {
            const type_spec &t = type_spec_for(tid);
            const be_key     k = t;
            const INative   *h = natives.search_by(k);
            if(!h) throw exception("%sno <%s>",fn, *t.name());
            return **h;
        }

    }

}
