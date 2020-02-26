
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
#include "y/os/wtime.hpp"

namespace upsylon {

    namespace information {

        const char Translator:: EncoderID[] = "Encoder";
        const char Translator:: DecoderID[] = "Decoder";


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
            wtime       chrono;

            {
                std::cerr << "<encoding with " << encoder->family() << " " << encoder->name() << ">" << std::endl;
                ios::icstream source( fileName );
                ios::ocstream target( compName );
                encoder->reset();
                const uint64_t mark = wtime::ticks();
                compBytes = encoder->process(target,source,&readBytes);
                const double   tmx  = chrono(wtime::ticks() - mark);
                const double   rate = (readBytes/tmx)/1e6;
                std::cerr << "\tencoder: " << readBytes << " -> " << compBytes << " @ " << rate << " MB/s" << std::endl;
                std::cerr << "<encoding with " << encoder->family() << " " << encoder->name() << "/>" << std::endl << std::endl;

            }

            if(decoder)
            {
                std::cerr << "<decoding with " << decoder->family() << " " << decoder->name() << ">" << std::endl;
                size_t loadBytes = 0;
                size_t backBytes = 0;
                {
                    ios::icstream source( compName );
                    ios::ocstream target( backName );
                    decoder->reset();
                    const uint64_t mark = wtime::ticks();
                    backBytes = decoder->process(target,source,&loadBytes);
                    const double   tmx  = chrono(wtime::ticks() - mark);
                    const double   rate = (readBytes/tmx)/1e6;
                    std::cerr << "\tdecoder: " << loadBytes << " -> " << backBytes << " @ " << rate << " MB/s" << std::endl;
                }
                if( loadBytes != compBytes ) throw exception("%s: loadBytes != compBytes", fn);
                if( readBytes != backBytes ) throw exception("%s: readBytes != backBytes", fn);
                hashing::sha1 H;
                const digest MDfile = ios::disk_file::md(H,fileName);
                const digest MDback = ios::disk_file::md(H,backName);
                std::cerr << H.name() << " : " << MDfile << "/" << MDback << std::endl;
                if(MDfile!=MDback) throw exception("%s: invalid checksum", fn);
                std::cerr << "<decoding with " << decoder->family() << " " << decoder->name() << "/>" << std::endl << std::endl;
            }


        }
    }
}
