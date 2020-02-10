#include "y/information/qbits.hpp"

namespace upsylon {

    bool       & qbit:: operator*()       throw() { return code; }
    const bool & qbit:: operator*() const throw() { return code; }

    qbit *qbit:: acquire(const bool flag)
    {
        qbit *q = object::acquire1<qbit>();
        **q = flag;
        return q;
    }

    void qbit:: release(qbit * &q) throw()
    {
        object::release1<qbit>(q);
    }

    qbit::list:: list() throw() : core::list_of<qbit>()
    {
    }

    qbit::list:: ~list() throw()
    {
        clear();
    }

    void  qbit::list:: clear() throw()
    {
        while(size>0)
        {
            qbit *node = pop_back();
            qbit::release(node);
        }
    }

    qbit::pool:: pool() throw() : core::pool_of<qbit>()
    {
    }

    qbit::pool:: ~pool() throw()    {
        clear();
    }

    void qbit::pool:: clear() throw()
    {
        while(size>0)
        {
            qbit *node = query();
            qbit::release(node);
        }
    }

    const uint8_t qbit::table<uint8_t>::bit[8] =
    {
        1,2,4,8,16,32,64,128
    };

    const uint16_t qbit::table<uint16_t>::bit[16] =
    {
        1,   2,   4,    8,    16,   32,   64,    128,
        256, 512, 1024, 2048, 4096, 8192, 16384, 32768
    };

    const uint32_t qbit::table<uint32_t>::bit[32] =
    {
        0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080,
        0x00000100, 0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000,
        0x00010000, 0x00020000, 0x00040000, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000,
        0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000
    };

    const uint64_t qbit::table<uint64_t>::bit[64] =
    {
        Y_U64(0x1), Y_U64(0x2), Y_U64(0x4), Y_U64(0x8), Y_U64(0x10), Y_U64(0x20), Y_U64(0x40), Y_U64(0x80),
        Y_U64(0x100), Y_U64(0x200), Y_U64(0x400), Y_U64(0x800), Y_U64(0x1000), Y_U64(0x2000), Y_U64(0x4000), Y_U64(0x8000),
        Y_U64(0x10000), Y_U64(0x20000), Y_U64(0x40000), Y_U64(0x80000), Y_U64(0x100000), Y_U64(0x200000), Y_U64(0x400000), Y_U64(0x800000),
        Y_U64(0x1000000), Y_U64(0x2000000), Y_U64(0x4000000), Y_U64(0x8000000), Y_U64(0x10000000), Y_U64(0x20000000), Y_U64(0x40000000), Y_U64(0x80000000),
        Y_U64(0x100000000), Y_U64(0x200000000), Y_U64(0x400000000), Y_U64(0x800000000), Y_U64(0x1000000000), Y_U64(0x2000000000), Y_U64(0x4000000000), Y_U64(0x8000000000),
        Y_U64(0x10000000000), Y_U64(0x20000000000), Y_U64(0x40000000000), Y_U64(0x80000000000), Y_U64(0x100000000000), Y_U64(0x200000000000), Y_U64(0x400000000000), Y_U64(0x800000000000),
        Y_U64(0x1000000000000), Y_U64(0x2000000000000), Y_U64(0x4000000000000), Y_U64(0x8000000000000), Y_U64(0x10000000000000), Y_U64(0x20000000000000), Y_U64(0x40000000000000), Y_U64(0x80000000000000),
        Y_U64(0x100000000000000), Y_U64(0x200000000000000), Y_U64(0x400000000000000), Y_U64(0x800000000000000), Y_U64(0x1000000000000000), Y_U64(0x2000000000000000), Y_U64(0x4000000000000000), Y_U64(0x8000000000000000)
    };

}

#include <iostream>

namespace upsylon {

    qbits:: qbits() throw() : qbit::list(), qpool()
    {
    }

    qbits:: ~qbits() throw()
    {

    }

    void qbits:: free() throw()
    {
        qpool.store( *this );
    }

    void qbits:: release() throw()
    {
        clear();
        qpool.clear();
    }

    qbit * qbits:: get(const bool flag)
    {
        if( qpool.has_nodes() )
        {
            qpool.head->code = flag;
            return qpool.query();
        }
        else
        {
            return qbit::acquire(flag);
        }
    }

    void qbits:: push( const bool flag )
    {
        push_back( get(flag) );
    }

    bool qbits:: peek() const throw()
    {
        assert(has_nodes());
        return head->code;
    }

    bool qbits:: pop() throw()
    {
        assert( has_nodes() );
        return **qpool.store( pop_front() );
    }

    void qbits:: reserve(size_t n)
    {
        while( n-- > 0 )
        {
            qpool.store( qbit::acquire(false) );
        }
    }

    std::ostream & qbits:: display( std::ostream &os, const size_t nmax) const
    {
        const bool check = (nmax>0);
        os << '[';
        size_t n = 0;
        for(const qbit *q = head;q;q=q->next)
        {
            os << *q;
            if(check&&++n>0) break;
        }
        os << ']';
        return os;
    }

    std::ostream & operator<<( std::ostream &os, const qbit &q)
    {
        if(q.code)
        {
            os << '1';
        }
        else
        {
            os << '0';
        }
        return os;
    }

    std::ostream & operator<<( std::ostream &os, const qbits &Q )
    {
        return Q.display(os);
    }

    void qbits:: zpad()
    {
        while( 0 != (size&7) )
        {
            push_back( get(false) );
        }
    }

    void qbits:: zpad( randomized::bits &ran)
    {
        while( 0 != (size&7) )
        {
            push_back( get( ran.choice() ) );
        }
    }

    void qbits:: drop() throw()
    {
        while( 0 != (size&7) )
        {
            qpool.store( pop_front() );
        }
    }

    void qbits:: write(char C)
    {
        _push<uint8_t>(C);
    }

    void qbits:: flush()
    {
        zpad();
    }

    bool qbits:: query( char &C )
    {
        if( size >= 8 )
        {
            C = _pop<uint8_t>();
            return true;
        }
        else
        {
            return false;
        }
    }

    void qbits:: store( char C )
    {
        const uint8_t U(C);
        for(int i=7;i>=0;--i)
        {
            push_front( get( 0 != (U&qbit::table<uint8_t>::bit[i]) ) );
        }
    }
}
