
#include "y/lang/pattern/posix.hpp"
#include "y/lang/pattern/basic.hpp"
#include "y/lang/pattern/logic.hpp"


namespace upsylon
{
    namespace Lang
    {
        Pattern * Posix:: lower()
        {
            return new Range('a','z');
        }

        Pattern * Posix:: upper()
        {
            return new Range('A','Z');
        }

        Pattern * Posix:: alpha()
        {
            auto_ptr<Logical> p = new OR();
            p->add( lower() );
            p->add( upper() );
            return p.yield();
        }

        Pattern * Posix:: digit()
        {
            return new Range('0','9');
        }

        Pattern * Posix:: alnum()
        {
            auto_ptr<Logical> p = new OR();
            p->add( lower() );
            p->add( upper() );
            p->add( digit() );
            return p.yield();
        }

        Pattern * Posix:: word()
        {
            auto_ptr<Logical> p = new OR();
            p->add( lower() );
            p->add( upper() );
            p->add( digit() );
            p->add( new Single( '_' ) );
            return p.yield();
        }

        Pattern * Posix:: xdigit()
        {
            auto_ptr<Logical> p = new OR();
            p->add( digit() );
            p->add( new Range('a','f') );
            p->add( new Range('A','F') );
            return p.yield();
        }

        Pattern * Posix:: blank()
        {
            return Logical::Among(" \t");
        }

        Pattern * Posix:: space()
        {
            return Logical::Among(" \t\n\r\v\f");
        }

        static inline void __fill_endl( Pattern::List &ops )
        {
            ops.push_back( new Single('\n') );
            ops.push_back( new Single('\r') );
            ops.push_back( Logical::Equal("\r\n") );
        }

        Pattern * Posix:: endl()
        {
            auto_ptr<Logical> p = new OR();
            __fill_endl(p->operands);
            return p.yield();
        }

        Pattern * Posix:: dot()
        {
            auto_ptr<Logical> p = new NONE();
            __fill_endl(p->operands);
            return p.yield();
        }

    }
}

