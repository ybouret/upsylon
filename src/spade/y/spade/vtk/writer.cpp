
#include "y/spade/vtk.hpp"
#include "y/type/spec.hpp"
#include "y/associative/be-key.hpp"
#include "y/exception.hpp"
#include "y/yap/rational.hpp"

namespace upsylon {

    namespace Spade {

        vtk::Writer::  Writer() throw() {}
        vtk::Writer:: ~Writer() throw() {}

        bool vtk::Writer:: isScalar() const throw()
        {
            return 1 == components();
        }
        
        static const char fn[] = "vtk::Writers: ";

        namespace {

            class apnWriter : public vtk::Writer
            {
            public:
                inline explicit apnWriter() throw() : vtk::Writer() {}
                inline virtual ~apnWriter() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(apnWriter);
                inline virtual
                ios::ostream & write( ios::ostream &fp, const void *addr ) const
                {
                    assert(addr);
                    const string s = static_cast<const apn *>(addr)->to_dec();
                    fp << s;
                    return fp;
                }

                inline virtual unsigned    components() const throw() { return 1; }
                inline virtual const char *dataType()   const throw() { return vtk::TypeInt; }

            };

            class apzWriter : public vtk::Writer
            {
            public:
                inline explicit apzWriter() throw() : vtk::Writer() {}
                inline virtual ~apzWriter() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(apzWriter);
                inline virtual
                ios::ostream & write( ios::ostream &fp, const void *addr ) const
                {
                    assert(addr);
                    const string s = static_cast<const apz *>(addr)->to_dec();
                    fp << s;
                    return fp;
                }
                inline virtual unsigned    components() const throw() { return 1; }
                inline virtual const char *dataType()   const throw() { return vtk::TypeInt; }
            };

            class apqWriter : public vtk::Writer
            {
            public:
                inline explicit apqWriter( const vtk::Writer &w ) throw() : vtk::Writer(), dWriter(w)
                {
                }

                inline virtual ~apqWriter() throw()
                {
                }

                const vtk::Writer &dWriter;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(apqWriter);
                inline virtual
                ios::ostream & write( ios::ostream &fp, const void *addr ) const
                {
                    assert(addr);
                    const double q = static_cast<const apq *>(addr)->to_double();
                    return dWriter.write(fp,&q);
                }
                inline virtual unsigned    components() const throw() { return 1; }
                inline virtual const char *dataType()   const throw() { return vtk::TypeDouble; }
            };

        }
        

        void vtk:: registerWriters()
        {
            tuples<complex,float>();
            tuples<complex,double>();

            tuples<point2d,float>();
            tuples<point3d,float>();

            tuples<point2d,double>();
            tuples<point3d,double>();
            
            tuples<point2d,unit_t>();
            tuples<point3d,unit_t>();


            record<apn>(new apnWriter());
            record<apz>(new apzWriter());
            record<apq>(new apqWriter( getNative<double>()));
        }


        const vtk::Writer & vtk::getWriter(const std::type_info &tid) const
        {
            const type_spec &t = type_spec_for(tid); //std::cerr << "getWriter<" << t.name() << ">" << std::endl;
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

        void vtk:: record(const std::type_info &tid, const IWriter &w)
        {
            const type_spec &t = type_spec_for(tid);
            const be_key     k = t;
            if( !writers.insert_by(k,w) )
            {
                throw exception("%smultiple <%s>", fn, *t.name());
            }
        }

    }

}
