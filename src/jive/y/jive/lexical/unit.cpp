#include "y/jive/lexical/unit.hpp"

namespace upsylon
{
	Y_MAGAZINE_IMPL(Jive::Lexical::Unit::Supply,Jive::Char::Supply::life_time-1)

    namespace Jive
    {
        namespace Lexical
        {


            Unit::Supply & Unit:: Instance()
            {
                static Supply &_ = Supply::instance();
                return _;
            }


            Unit:: ~Unit() throw()
            {
            }

            Unit:: Unit(const Context &c,
                        const Tag     &t,
                        const size_t   s) throw() :
            Token(), Context(c), inode<Unit>(), label(t),
            stamp(s),
            owner(NULL),
            usage(Core)
            {
            }

            const char  * Unit:: usageText() const throw()
            {
                switch(usage)
                {
                    case Core: return "core";
                    case Done: return "done";
                }
                return "???";
            }


            Unit * Unit::Create(const Context &c,
                                const Tag     &t,
                                size_t        &s)
            {
                static Supply &mgr = Supply::instance();
                Unit          *ans = mgr.acquire<const Context &,const Tag &,size_t>(c,t,s);
                ++s;
                return ans;
            }
            
            void Unit:: Delete(Unit *unit) throw()
            {
                static Supply &mgr = Supply::location();
                mgr.release(unit);
            }
         
            exception  & Unit:: labelTo(exception &e) const throw()
            {
                e.cat("<%s>",**label);
                return e;
            }

            exception & Unit:: writeTo(exception &e, const bool withText) const throw()
            {
                labelTo(e);
                if(withText&&size>0)
                {
                    e.cat(" = '");
                    charsTo(e);
                    e.cat("'");
                }
                return e;
            }
 
        }

    }
}




