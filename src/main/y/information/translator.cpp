
#include "y/information/translator.hpp"

namespace upsylon {

    namespace information {


        Translator:: Translator() throw()
        {
        }

        Translator:: ~Translator() throw()
        {
        }

        size_t Translator :: process(ios::ostream &target,
                                     ios::istream &source,
                                     size_t       *sourceLength)
        {
            size_t n_inp = 0;
            size_t n_out = 0;
            {
                char   C     = 0;
                while( source.query(C) )
                {
                    ++n_inp;
                    this->write(C);
                    while( this->query(C) )
                    {
                        ++n_out;
                        target.write(C);
                    }
                }
                this->flush();
                while( this->query(C) )
                {
                    ++n_out;
                    target.write(C);
                }
            }
            if(sourceLength) *sourceLength = n_inp;
            return n_out;
        }

    }
}

#include "y/ios/ocstream.hpp"
#include "y/ios/icstream.hpp"
#include "y/type/cswap.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace information {

        size_t Translator:: Fibonnaci( const string &fibName, const uint8_t a, const uint8_t b)
        {
            ios::ocstream fp(fibName);
            size_t A = a;
            size_t B = b;
            if(A>B) _cswap(A,B);
            size_t u0=0;
            size_t u1=1;
            size_t count = 0;
            for(size_t i=A;i<B;++i)
            {
                const size_t u2 = u1+u0;
                fp.repeat(u2,uint8_t(i));
                u0=u1;
                u1=u2;
                count += u2;
            }
            return count;
        }

        size_t Translator:: testFibonnaci(const string &encoded, const string &fibonacci, const uint8_t a, const uint8_t b, size_t &ngen)
        {
            reset();
            ngen = Fibonnaci(fibonacci,a,b);
            size_t nenc = 0;
            {
                size_t nread = 0;
                ios::icstream source( fibonacci );
                ios::ocstream target( encoded   );
                nenc = process(target,source,&nread);
                if( nread != ngen ) throw exception("testFibonacci read error");
            }

            return nenc;
        }
    }
}
