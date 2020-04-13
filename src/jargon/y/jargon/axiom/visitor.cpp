#include "y/jargon/axiom/visitor.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Visitor:: Visitor() : Axiom::DB()
        {
        }
        
        Visitor:: ~Visitor() throw()
        {
        }
        
        
        bool Visitor::Check(const Axiom &, void *) throw()
        {
            return true;
        }
       

        
    }
    
}

#include "y/jargon/axiom/all.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        template <typename COMPOUND> static inline
        bool __visit_compound(const Axiom     &axiom,
                              Visitor         &visitor,
                              Visitor::Process proc,
                              void            *args)
        {
            assert( visitor.search(axiom) );
            for(const Member *m = axiom.as<COMPOUND>().head;m;m=m->next)
            {
                const Axiom &used = m->axiom;
                if( !visitor.walkDown(used, proc, args))
                {
                    return false;
                }
            }
            return true;
        }
        
        template <typename WILDCARD> static inline
        bool __visit_wildcard(const Axiom     &axiom,
                              Visitor         &visitor,
                              Visitor::Process proc,
                              void            *args)
        {
            assert( visitor.search(axiom) );
            const Axiom &used = axiom.as<WILDCARD>().axiom;
            return visitor.walkDown(used, proc, args);
        }
        
        bool Visitor:: walkDown(const Axiom &root,
                                Process      proc,
                                void        *args)
        {
            assert(proc);
            
            if(insert(root))
            {
                if( !proc(root,args) )
                {
                    return false; // early return
                }
                
                assert( search(root) );
                switch(root.uuid)
                {
                    case Terminal::  UUID: return true;
                    case Operator::  UUID: return true;

                        // wildcards
                    case Option::    UUID: return __visit_wildcard<Option>(    root,*this,proc,args);
                    case OneOrMore:: UUID: return __visit_wildcard<OneOrMore>( root,*this,proc,args);
                    case ZeroOrMore::UUID: return __visit_wildcard<ZeroOrMore>(root,*this,proc,args);

                        // compounds
                    case Alternate::UUID:  return __visit_compound<Alternate>(root,*this,proc,args);
                    case Aggregate::UUID:  return __visit_compound<Aggregate>(root,*this,proc,args);

                    default:
                        break;
                }
                throw exception("Visitor::walkDown(unhandled <%s>,UUID=%s)", **(root.label), fourcc_(root.uuid) );
                
            }
            else
            {
                // already there!
                return true;
            }
        }
        
        void Visitor:: supplyWith(const Axiom &root)
        {
            if( !walkDown(root, Check, NULL) )
                throw exception("Visitor::supplyWith: unexpected failure");
        }

        
    }
}

