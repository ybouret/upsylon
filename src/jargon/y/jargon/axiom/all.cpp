
#include "y/jargon/axiom/all.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        bool Axiom:: isTerminal() const throw()
        {
            return Terminal::UUID == uuid;
        }
        
        bool Axiom:: isCompound() const throw()
        {
            switch(uuid)
            {
                case Aggregate::UUID:
                case Alternate::UUID: return true;
                default: break;
            }
            return false;
        }
        
        bool Axiom:: isWildcard() const throw()
        {
            switch(uuid)
            {
                case Option::     UUID:
                case ZeroOrMore:: UUID:
                case OneOrMore::  UUID: return true;
                default: break;
            }
            return false;
        }
        
        bool Axiom:: isApparent() const throw()
        {
            
            switch(uuid)
            {
                case Terminal:: UUID: return true;
                case Aggregate::UUID: return as<Aggregate>().feature != Aggregate::Vanishing;
                default: break;
            }
            
            return false;
        }
        
        template <typename WILDCARD> static inline
        bool __proxy(const Axiom &axiom, Manifest &apparent )
        {
            return axiom.as<WILDCARD>().axiom.joinFirstApparentTo(apparent);
        }
        
        static inline
        bool __proxy_alt( const Alternate &alt, Manifest &apparent )
        {
            // catenate all possibilities
            bool found = false;
            for( const Member *m=alt.head;m;m=m->next)
            {
                if( m->axiom.joinFirstApparentTo(apparent) )
                {
                    found = true;
                }
            }
            return found;
        }
        
        static inline
        bool __proxy_agg( const Aggregate &agg, Manifest &apparent )
        {
            if( Aggregate::Vanishing==agg.feature )
            {
                // first apparents
                for( const Member *m=agg.head;m;m=m->next)
                {
                    if( m->axiom.joinFirstApparentTo(apparent) )
                    {
                        return true;
                    }
                }
                return false;
            }
            else
            {
                // itself
                apparent.push_back( new Member(agg) );
                return true;
            }
        }
        
        bool Axiom:: joinFirstApparentTo(Manifest &apparent) const
        {
            switch(uuid)
            {
                case Terminal::   UUID:  apparent.push_back( new Member(*this) ); return true;
                case Option::     UUID:  return __proxy<Option>(*this,apparent);
                case ZeroOrMore:: UUID:  return __proxy<ZeroOrMore>(*this,apparent);
                case OneOrMore::  UUID:  return __proxy<OneOrMore>(*this,apparent);
                case Alternate::  UUID:  return __proxy_alt( as<Alternate>(), apparent );
                case Aggregate::  UUID:  return __proxy_agg( as<Aggregate>(), apparent );
                default:
                    break;
            }
            return false;
        }
        
        
    }
    
}

