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

            static size_t UnitStamp = 0;
            
            Unit:: ~Unit() throw()
            {
            }

            Unit:: Unit(const Context &c,
                        const Tag     &t,
                        const size_t   s) throw() :
            Token(), Context(c), inode<Unit>(), label(t),
            stamp(s)
            {
            }

            Unit * Unit::Create(const Context &c,
                                const Tag     &t )
            {
                static Supply &mgr = Supply::instance();
                Y_LOCK(mgr.access);
                ++UnitStamp;
                return mgr.acquire<const Context &,const Tag &,size_t>(c,t,UnitStamp);
            }
            
            void Unit:: Delete(Unit *unit) throw()
            {
                static Supply &mgr = Supply::location();
                mgr.release(unit);
            }
            
        }

    }
}




