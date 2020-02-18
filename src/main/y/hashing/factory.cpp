#include "y/hashing/factory.hpp"

namespace upsylon {

    namespace hashing {

        factory:: factory() : xfactory<function>(32)
        {
            initialize();
        }

        factory:: ~factory() throw()
        {
        }
        

    }
}

#include "y/hashing/adler32.hpp"
#include "y/hashing/bjh32.hpp"
#include "y/hashing/crc16.hpp"
#include "y/hashing/crc32.hpp"
#include "y/hashing/elf.hpp"
#include "y/hashing/fnv.hpp"
#include "y/hashing/pjw.hpp"
#include "y/hashing/sfh.hpp"
#include "y/hashing/sha1.hpp"
#include "y/hashing/md2.hpp"
#include "y/hashing/md4.hpp"
#include "y/hashing/md5.hpp"
#include "y/hashing/rmd128.hpp"
#include "y/hashing/rmd160.hpp"
#include "y/hashing/sha256.hpp"
#include "y/hashing/sha512.hpp"


namespace upsylon {

    namespace hashing {

#define Y_H_DECL(NAME) use( NAME::CLID, NAME::create )

        void factory:: initialize()
        {
            Y_H_DECL(adler32);
            Y_H_DECL(bjh32);
            Y_H_DECL(crc16);
            Y_H_DECL(crc32);
            Y_H_DECL(elf);
            Y_H_DECL(fnv);
            Y_H_DECL(pjw);
            Y_H_DECL(sfh);
            Y_H_DECL(sha1);
            Y_H_DECL(md2);
            Y_H_DECL(md4);
            Y_H_DECL(md5);
            Y_H_DECL(rmd128);
            Y_H_DECL(rmd160);
            Y_H_DECL(sha224);
            Y_H_DECL(sha256);
            Y_H_DECL(sha384);
            Y_H_DECL(sha512);
        }
    }

}
