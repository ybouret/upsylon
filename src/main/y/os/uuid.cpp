#include "y/os/uuid.hpp"
#include "y/hashing/sha1.hpp"
#include "y/code/utils.hpp"
#include "y/randomized/bits.hpp"

namespace upsylon
{
    uuid:: ~uuid() throw()
    {
        memset(byte,0,sizeof(byte));
    }

    uuid:: uuid() :
    byte()
    {

        hashing::sha1 H;
        H.set();

        {
            randomized::bits & b = randomized::bits::crypto();
            for(size_t i=BYTES;i>0;--i)
            {
                const uint32_t dw = b.next32();
                H.run_type(dw);
            }
        }

        H.get(byte,sizeof(byte));
    }

    uuid:: uuid( const uuid &other ) throw() :
    byte()
    {
        memcpy(byte,other.byte,sizeof(byte));
    }

    const void * uuid:: ro()     const throw() { return byte; }
    size_t       uuid:: length() const throw() { return BYTES; }

    std::ostream &   operator<<( std::ostream &os, const uuid &u)
    {
        for(size_t i=0;i<uuid::BYTES;++i)
        {
            os << hexadecimal::uppercase[u.byte[i]];
            switch(i)
            {
                case 3:
                case 7:
                case 11:
                    os << '-';
                    break;

                default: break;
            }
        }
        return os;
    }

    string uuid:: to_string() const
    {
        string ans(35,as_capacity);
        for(size_t i=0;i<uuid::BYTES;++i)
        {
            ans << hexadecimal::uppercase[byte[i]];
            switch(i)
            {
                case 3:
                case 7:
                case 11:
                    ans << '-';
                    break;

                default: break;
            }
        }
        return ans;
    }


}
