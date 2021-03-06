#include "y/information/iobits.hpp"

namespace upsylon {

    namespace Information {


        const uint8_t TableOf<uint8_t>::Bit[8] =
        {
            1,2,4,8,16,32,64,128
        };

        const uint16_t  TableOf<uint16_t>::Bit[16] =
        {
            1,   2,   4,    8,    16,   32,   64,    128,
            256, 512, 1024, 2048, 4096, 8192, 16384, 32768
        };

        const uint32_t TableOf<uint32_t>::Bit[32] =
        {
            0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080,
            0x00000100, 0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000,
            0x00010000, 0x00020000, 0x00040000, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000,
            0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000
        };

        const uint64_t  TableOf<uint64_t>::Bit[64] =
        {
            Y_U64(0x1), Y_U64(0x2), Y_U64(0x4), Y_U64(0x8),
            Y_U64(0x10), Y_U64(0x20), Y_U64(0x40), Y_U64(0x80),
            Y_U64(0x100), Y_U64(0x200), Y_U64(0x400), Y_U64(0x800),
            Y_U64(0x1000), Y_U64(0x2000), Y_U64(0x4000), Y_U64(0x8000),
            Y_U64(0x10000), Y_U64(0x20000), Y_U64(0x40000), Y_U64(0x80000),
            Y_U64(0x100000), Y_U64(0x200000), Y_U64(0x400000), Y_U64(0x800000),
            Y_U64(0x1000000), Y_U64(0x2000000), Y_U64(0x4000000), Y_U64(0x8000000),
            Y_U64(0x10000000), Y_U64(0x20000000), Y_U64(0x40000000), Y_U64(0x80000000),
            Y_U64(0x100000000), Y_U64(0x200000000), Y_U64(0x400000000), Y_U64(0x800000000),
            Y_U64(0x1000000000), Y_U64(0x2000000000), Y_U64(0x4000000000), Y_U64(0x8000000000),
            Y_U64(0x10000000000), Y_U64(0x20000000000), Y_U64(0x40000000000), Y_U64(0x80000000000),
            Y_U64(0x100000000000), Y_U64(0x200000000000), Y_U64(0x400000000000), Y_U64(0x800000000000),
            Y_U64(0x1000000000000), Y_U64(0x2000000000000), Y_U64(0x4000000000000), Y_U64(0x8000000000000),
            Y_U64(0x10000000000000), Y_U64(0x20000000000000), Y_U64(0x40000000000000), Y_U64(0x80000000000000),
            Y_U64(0x100000000000000), Y_U64(0x200000000000000), Y_U64(0x400000000000000), Y_U64(0x800000000000000),
            Y_U64(0x1000000000000000), Y_U64(0x2000000000000000), Y_U64(0x4000000000000000), Y_U64(0x8000000000000000)
        };

    }
}

#include <iostream>

namespace upsylon {

    namespace Information {

        IOBits:: IOBits() throw() : IOBits_()
        {
        }

        IOBits:: IOBits(const size_t n) throw() : IOBits_(n,as_capacity)
        {
        }

        IOBits:: ~IOBits() throw()
        {

        }


        bool IOBits:: pop() throw()
        {
            const bool ans = front();
            pop_front();
            return ans;
        }
        
        std::ostream & IOBits:: display( std::ostream &os, const size_t nmax) const
        {
            const bool check = (nmax>0);
            os << '[';
            size_t n = 0;
            for(const_iterator it=begin();it!=end();++it)
            {
                const bool flag = *it;
                os << (flag ? '1' : '0' );
                if(check&&++n>0) break;
            }
            os << ']';
            return os;
        }


        std::ostream & operator<<( std::ostream &os, const IOBits &Q )
        {
            return Q.display(os);
        }

        void IOBits:: zpad()
        {
            while( 0 != (size()&7) )
            {
                push_back( false );
            }
        }

        void IOBits:: zpad( randomized::bits &ran)
        {
            while( 0 != (size()&7) )
            {
                push_back(  ran.choice()  );
            }
        }

        void IOBits:: drop() throw()
        {
            while( 0 != (size()&7) )
            {
                pop_front();
            }
        }

        void IOBits:: write(char C)
        {
            _push<uint8_t>(C);
        }

        void IOBits:: flush()
        {
            zpad();
        }

        bool IOBits:: query( char &C )
        {
            if( size() >= 8 )
            {
                C = _pop<uint8_t>();
                return true;
            }
            else
            {
                return false;
            }
        }

        void IOBits:: store( char C )
        {
            const uint8_t U(C);
            for(int i=7;i>=0;--i)
            {
                push_front(   0 != (U& TableOf<uint8_t>::Bit[i]) );
            }
        }

        void IOBits:: compile(sequence<char> &seq)
        {
            while( size() >= 8 )
            {
                seq.push_back( pop<char>() );
            }
        }

        void IOBits:: zfinish(sequence<char> &seq)
        {
            zpad();
            compile(seq);
            assert(0==size());
        }
        
        IOBits::Broker::  Broker() throw() {}
        IOBits::Broker:: ~Broker() throw() {}

    }

}

