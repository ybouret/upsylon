
#include "y/hashing/pearson.hpp"
#include "y/type/aliasing.hpp"
#include <cstring>

namespace upsylon {

    namespace  hashing  {
        
        pearson_:: ~pearson_() throw()
        {
            memset( (void*)table,0,sizeof(table));
        }

        pearson_:: pearson_(randomized::bits *salt) throw() :
        table()
        {
            reset(salt);
        }

        void pearson_:: reset(randomized::bits *salt) throw()
        {
            uint8_t *t = aliasing::_(table);
            for(size_t i=0;i<256;++i)
            {
                t[i] = uint8_t(i);
            }
            if(salt) salt->shuffle(t,256);
        }

        uint8_t pearson_:: next(const uint8_t h, const uint8_t c) const throw()
        {
            return table[ h ^ c ];
        }

        void pearson_:: initialize(void *addr,const size_t size) const throw()
        {
            assert(addr!=0);
            assert(size>0);

            uint8_t *p = static_cast<uint8_t *>(addr);
            for(size_t i=0;i<size;++i)
            {
                p[i] = uint8_t(i);
            }
        }

        void pearson_:: update(void *addr,const size_t size, const uint8_t c) const throw()
        {
            assert(addr!=0);
            assert(size>0);

            uint8_t *p = static_cast<uint8_t *>(addr);
            for(size_t i=0;i<size;++i)
            {
                uint8_t &h = p[i];
                h = table[h ^ c];
            }
        }


    }

}
