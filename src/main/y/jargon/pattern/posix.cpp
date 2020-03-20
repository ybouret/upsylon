#include "y/jargon/pattern/posix.hpp"
#include "y/jargon/pattern/all.hpp"
#include "y/ptr/auto.hpp"
namespace upsylon {
 
    namespace Jargon {
        
        Pattern * posix:: lower()
        {
            return Range::Create('a','z');
        }
        
        Pattern * posix:: upper()
        {
            return Range::Create('A','Z');
        }
        
        Pattern * posix:: alpha()
        {
            auto_ptr<Logical> p = OR::Create();
            p->push_back( lower() );
            p->push_back( upper() );
            return p.yield();
        }
        
        Pattern * posix:: digit()
        {
            return Range::Create('0','9');
        }
        
        Pattern * posix:: alnum()
        {
            auto_ptr<Logical> p = OR::Create();
            p->push_back( lower() );
            p->push_back( upper() );
            p->push_back( digit() );
            return p.yield();
        }
        
        Pattern * posix:: word()
        {
            auto_ptr<Logical> p = OR::Create();
            p->push_back( lower() );
            p->push_back( upper() );
            p->push_back( digit() );
            p->add(  '_'  );
            return p.yield();
        }
        
        Pattern * posix:: xdigit()
        {
            auto_ptr<Logical> p = OR::Create();
            p->push_back( digit() );
            p->add('a','f');
            p->add('A','F');
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
            ops.push_back( Single::Create('\n') );
            ops.push_back( Single::Create('\r') );
            ops.push_back( Logical::Equal("\r\n") );
        }
        
        Pattern * posix:: endl()
        {
            auto_ptr<Logical> p = OR::Create();
            __fill_endl(*p);
            return p.yield();
        }
        
        Pattern * posix:: dot()
        {
            auto_ptr<Logical> p = NONE::Create();
            __fill_endl(*p);
            return p.yield();
        }
        
        Pattern * posix:: core()
        {
            auto_ptr<Logical> p = OR::Create();
            p->add( 0x20 );
            p->add( 0x21 );
            p->add( 0x23,0x26 );
            p->add( 0x28,0x5B );
            p->add( 0x5D,0x7F );
            return p.yield();
        }
    }
    
}
