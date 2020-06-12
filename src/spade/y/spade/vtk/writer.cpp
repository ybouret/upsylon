
#include "y/spade/vtk.hpp"
#include "y/type/spec.hpp"
#include "y/associative/be-key.hpp"
#include "y/exception.hpp"
#include "y/mpl/rational.hpp"

namespace upsylon {

    namespace Spade {

        vtk::Writer::  Writer() throw() {}
        vtk::Writer:: ~Writer() throw() {}

        static const char fn[] = "vtk::Writers: ";

        namespace {

            class mpnWriter : public vtk::Writer
            {
            public:
                inline explicit mpnWriter() throw() : vtk::Writer() {}
                inline virtual ~mpnWriter() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(mpnWriter);
                inline virtual
                ios::ostream & write( ios::ostream &fp, const void *addr ) const
                {
                    assert(addr);
                    const string s = static_cast<const mpn *>(addr)->to_decimal();
                    (void)fp.write_block(s);
                    return fp;
                }

                inline virtual unsigned    components() const throw() { return 1; }
                inline virtual const char *dataType()   const throw() { return vtk::TypeInt; }

            };

            class mpzWriter : public vtk::Writer
            {
            public:
                inline explicit mpzWriter() throw() : vtk::Writer() {}
                inline virtual ~mpzWriter() throw() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(mpzWriter);
                inline virtual
                ios::ostream & write( ios::ostream &fp, const void *addr ) const
                {
                    assert(addr);
                    const string s = static_cast<const mpz *>(addr)->to_decimal();
                    (void)fp.write_block(s);
                    return fp;
                }
                inline virtual unsigned    components() const throw() { return 1; }
                inline virtual const char *dataType()   const throw() { return vtk::TypeInt; }
            };

            class mpqWriter : public vtk::Writer
            {
            public:
                inline explicit mpqWriter( const vtk::Writer &w ) throw() : vtk::Writer(), dWriter(w)
                {
                }

                inline virtual ~mpqWriter() throw()
                {
                }

                const vtk::Writer &dWriter;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(mpqWriter);
                inline virtual
                ios::ostream & write( ios::ostream &fp, const void *addr ) const
                {
                    assert(addr);
                    const double q = static_cast<const mpq *>(addr)->to_real();
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


            record<mpn>(new mpnWriter());
            record<mpz>(new mpzWriter());
            record<mpq>(new mpqWriter( getNative<double>()));


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
