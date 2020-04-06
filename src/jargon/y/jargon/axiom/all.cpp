
#include "y/jargon/axiom/all.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        bool Axiom:: isTerminal() const throw()
        {
            return Terminal::UUID == uuid;
        }
        
#if 0
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
#endif
        
    }
    
}

namespace upsylon {
    
    namespace Jargon {
        
        template <typename WILDCARD> static inline
        bool __proxy_jfa(const Axiom &axiom,
                         AlphaList   &alpha)
        {
            return axiom.as<WILDCARD>().axiom.joinFirstApparentTo(alpha);
        }
        
        static inline
        bool __proxy_jfa_alt(const Alternate &alt,
                             AlphaList       &alpha )
        {
            // catenate all possibilities
            bool found = false;
            for( const Member *m=alt.head;m;m=m->next)
            {
                if( m->axiom.joinFirstApparentTo(alpha) )
                {
                    found = true;
                }
            }
            return found;
        }
        
        static inline
        bool __proxy_jfa_agg(const Aggregate &agg,
                             AlphaList       &alpha )
        {
            if( Aggregate::Vanishing==agg.feature )
            {
                // first apparents
                for( const Member *m=agg.head;m;m=m->next)
                {
                    if( m->axiom.joinFirstApparentTo(alpha) )
                    {
                        return true;
                    }
                }
                return false;
            }
            else
            {
                // itself
                alpha.push_back( new AlphaNode(agg) );
                return true;
            }
        }
        
        bool Axiom:: joinFirstApparentTo(AlphaList &alpha) const
        {
            switch(uuid)
            {
                case Terminal::   UUID:  alpha.push_back( new AlphaNode(*this) ); return true;
                case Option::     UUID:  return __proxy_jfa<Option>(*this,alpha);
                case ZeroOrMore:: UUID:  return __proxy_jfa<ZeroOrMore>(*this,alpha);
                case OneOrMore::  UUID:  return __proxy_jfa<OneOrMore>(*this,alpha);
                case Alternate::  UUID:  return __proxy_jfa_alt( as<Alternate>(), alpha );
                case Aggregate::  UUID:  return __proxy_jfa_agg( as<Aggregate>(), alpha );
                default:
                    break;
            }
            return false;
        }
        
        
    }
    
}

namespace upsylon {
    
    namespace Jargon {
        
        template <typename WILDCARD> static inline
        bool __proxy_jft(const Axiom &axiom,
                         TermPool    &tl)
        {
            return axiom.as<WILDCARD>().axiom.joinFirstTerminalTo(tl);
        }
        
        static inline
        bool __proxy_jft_alt(const Alternate &alt, TermPool &tl)
        {
            // catenate all possibilities
            bool found = false;
            for( const Member *m=alt.head;m;m=m->next)
            {
                if( m->axiom.joinFirstTerminalTo(tl) )
                {
                    found = true;
                }
            }
            return found;
        }
        
        static inline
        bool __proxy_jft_agg( const Aggregate &agg, TermPool &tl )
        {
            
            // first terminal
            for( const Member *m=agg.head;m;m=m->next)
            {
                if( m->axiom.joinFirstTerminalTo(tl) )
                {
                    return true;
                }
            }
            return false;
        }
        
        bool Axiom:: joinFirstTerminalTo(TermPool &tl) const
        {
            switch(uuid)
            {
                case Terminal::   UUID:  tl.push_back( new TermNode(as<Terminal>()) ); return true;
                case Option::     UUID:  return __proxy_jft<Option>(*this,tl);
                case ZeroOrMore:: UUID:  return __proxy_jft<ZeroOrMore>(*this,tl);
                case OneOrMore::  UUID:  return __proxy_jft<OneOrMore>(*this,tl);
                case Alternate::  UUID:  return __proxy_jft_alt( as<Alternate>(), tl );
                case Aggregate::  UUID:  return __proxy_jft_agg( as<Aggregate>(), tl );
                default:
                    break;
            }
            return false;
        }
        
    }
}

