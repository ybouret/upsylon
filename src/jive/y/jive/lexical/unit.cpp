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
                return mgr.acquire(c,t);
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
    namespace Jive
    {
        namespace Lexical
        {
            Unit::Pointer::Pointer( Unit *lexeme ) throw() :
            ptr<Unit>(lexeme)
            {
            }
            
            Unit::Pointer:: ~Pointer() throw()
            {
            }
            
            void Unit::Pointer:: zap() throw()
            {
                if(pointee)
                {
                    Unit::Delete(pointee);
                    pointee=0;
                }
            }
            
            Unit::Pointer &  Unit::Pointer:: operator=( Unit *other ) throw()
            {
                if(other!=pointee)
                {
                    zap();
                    pointee = other;
                }
                return *this;
            }
            
            Unit * Unit::Pointer:: yield() throw()
            {
                Unit *unit = pointee;
                pointee    = 0;
                return unit;
            }
        }
    }
}

namespace upsylon
{
    namespace Jive
    {
        namespace Lexical
        {
            Unit::List:: List() throw() : ListType()
            {
            }
            
            
            Unit::List:: ~List() throw()
            {
                clr();
            }
            
            void Unit:: List:: clr() throw()
            {
                while(size)
                {
                    Unit::Delete( pop_back() );
                }
            }
            
            void Unit::List:: release() throw()
            {
                clr();
            }
            
            
            

        }
    }
}



namespace upsylon
{
    namespace Jive
    {
        Y_SINGLETON_IMPL_WITH(Char::Supply::life_time-1,Lexical::Unit::Supply);

        
        namespace Lexical
        {
            Unit::Supply:: Supply()
            {
            }
            
            Unit::Supply:: ~Supply() throw()
            {
            }
            
            Unit * Unit::Supply:: acquire(const Context &c, const Tag &t)
            {
                Y_LOCK(access);
                return new (zquery()) Unit(c,t);
            }
            
            void Unit::Supply:: release(Unit *unit) throw()
            {
                Y_LOCK(access);
                assert(unit);
                unit->~Unit();
                zstore(unit);
            }
                
                
            
        }
        
    }
}

