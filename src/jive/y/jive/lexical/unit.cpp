#include "y/jive/lexical/unit.hpp"


namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {

            Unit:: ~Unit() throw()
            {
            }

            Unit:: Unit(const Context &c,
                        const Tag     &t) throw() :
            Token(), Context(c), inode<Unit>(), label(t)
            {
            }

            Unit * Unit::Create(const Context &c,
                                const Tag     &t )
            {
                static Supply &mgr = Supply::instance();
                return mgr.acquire<const Context &,const Tag &>(c,t);
            }
            
            void Unit:: Delete(Unit *unit) throw()
            {
                static Supply &mgr = Supply::location();
                mgr.release(unit);
            }
            
        }

    }
}



namespace upsylon
{
    namespace memory {

        Y_SINGLETON_TEMPLATE_WITH(Jive::Char::Supply::life_time-1,Jive::Lexical::Unit::Supply);
        
    }

}

