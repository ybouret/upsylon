
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
#include "y/string.hpp"
#include "y/hashing/sha1.hpp"
#include "y/fs/disk/file.hpp"

namespace upsylon {

    namespace information {

        size_t Translator:: Fibonacci(const string &fileName, const uint8_t a, const uint8_t b)
        {
            ios::ocstream fp(fileName);
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

        void Translator:: testCODEC(const string &fileName,
                                    const string &compName,
                                    const string &backName,
                                    Translator   *decoder)
        {
            static const char fn[] = "testCODEC";
            Translator *encoder    = this;
            size_t      readBytes  = 0;
            size_t      compBytes  = 0;
            {
                std::cerr << "<encoding>" << std::endl;
                ios::icstream source( fileName );
                ios::ocstream target( compName );
                encoder->reset();
                compBytes = encoder->process(target,source,&readBytes);
            }
            std::cerr << "\tencoder: " << readBytes << " -> " << compBytes << std::endl;

            if(decoder)
            {
                std::cerr << "<decoding>" << std::endl;
                size_t loadBytes = 0;
                size_t backBytes = 0;
                {
                    ios::icstream source( compName );
                    ios::ocstream target( backName );
                    decoder->reset();
                    backBytes = decoder->process(target,source,&loadBytes);
                }
                std::cerr << "\tdecoder: " << loadBytes << " -> " << backBytes << std::endl;
                if( loadBytes != compBytes ) throw exception("%s: loadBytes != compBytes", fn);
                if( readBytes != backBytes ) throw exception("%s: readBytes != backBytes", fn);
                hashing::sha1 H;
                const digest MDfile = ios::disk_file::md(H,fileName);
                std::cerr << H.name() << " : " << MDfile << std::endl;
                const digest MDback = ios::disk_file::md(H,backName);
                if(MDfile!=MDback) throw exception("%s: invalid checksum", fn);
            }


        }
    }
}
