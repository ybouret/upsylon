
#include "y/lang/pattern/posix.hpp"
#include "y/lang/pattern/basic.hpp"
#include "y/lang/pattern/logic.hpp"


namespace upsylon
{
    namespace Lang
    {
        Pattern * posix:: lower()
        {
            return new Range('a','z');
        }

        Pattern * posix:: upper()
        {
            return new Range('A','Z');
        }

        Pattern * posix:: alpha()
        {
            auto_ptr<Logical> p = new OR();
            p->add( lower() );
            p->add( upper() );
            return p.yield();
        }

        Pattern * posix:: digit()
        {
            return new Range('0','9');
        }

        Pattern * posix:: alnum()
        {
            auto_ptr<Logical> p = new OR();
            p->add( lower() );
            p->add( upper() );
            p->add( digit() );
            return p.yield();
        }

        Pattern * posix:: word()
        {
            auto_ptr<Logical> p = new OR();
            p->add( lower() );
            p->add( upper() );
            p->add( digit() );
            p->add( new Single( '_' ) );
            return p.yield();
        }

        Pattern * posix:: xdigit()
        {
            auto_ptr<Logical> p = new OR();
            p->add( digit() );
            p->add( new Range('a','f') );
            p->add( new Range('A','F') );
            return p.yield();
        }

        Pattern * posix:: blank()
        {
            return Logical::Among(" \t");
        }

        Pattern * posix:: space()
        {
            return Logical::Among(" \t\n\r\v\f");
        }

        Pattern * posix::punct()
        {
            return Logical::Among("][!\"#$%&'()*+,./:;<=>?@\\^_`{|}~-");
        }

        static inline void __fill_endl( Pattern::List &ops )
        {
            ops.push_back( new Single('\n') );
            ops.push_back( new Single('\r') );
            ops.push_back( Logical::Equal("\r\n") );
        }

        Pattern * posix:: endl()
        {
            auto_ptr<Logical> p = new OR();
            __fill_endl(p->operands);
            return p.yield();
        }

        Pattern * posix:: dot()
        {
            auto_ptr<Logical> p = new NONE();
            __fill_endl(p->operands);
            return p.yield();
        }

        Pattern * posix:: core()
        {
            auto_ptr<Logical> p = new OR();
            p->add( new Single(0x20)     );
            p->add( new Single(0x21)     );
            p->add( new Range(0x23,0x26) );
            p->add( new Range(0x28,0x5B) );
            p->add( new Range(0x5D,0x7F) );
            return p.yield();
        }

    }
}

