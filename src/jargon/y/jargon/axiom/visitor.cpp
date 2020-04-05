#include "y/jargon/axiom/visitor.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Visitor:: Visitor() : AxiomAddresses()
        {
        }
        
        Visitor:: ~Visitor() throw()
        {
        }
        
        bool Visitor:: wasVisited(const Axiom &axiom) const throw()
        {
            const AxiomAddress addr = axiom;
            return search_by(addr) != NULL;
        }
        
        bool Visitor:: greenLight( const Axiom &axiom )
        {
            const AxiomAddress addr = axiom;
            return insert_by(addr,addr);
        }
        
        bool Visitor::Check(const Axiom &, void *) throw()
        {
            return true;
        }
        
        void Visitor:: display() const
        {
            std::cerr << "<Visitor entries=" << entries() << ">" << std::endl;
            for( const_iterator it=begin(); it!=end();++it)
            {
                const AxiomAddress &addr  = *it;
                const Axiom        *axiom = static_cast<const Axiom *>( addr[0] );
                std::cerr << "\t<" << axiom->label << ">" << std::endl;
            }
            std::cerr << "<Visitor/>" << std::endl;

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
            assert( visitor.wasVisited(axiom) );
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
            assert( visitor.wasVisited(axiom) );
            const Axiom &used = axiom.as<WILDCARD>().axiom;
            return visitor.walkDown(used, proc, args);
        }
        
        bool Visitor:: walkDown(const Axiom &root,
                                Process      proc,
                                void        *args)
        {
            assert(proc);
            
            if(greenLight(root))
            {
                if( !proc(root,args) )
                {
                    return false; // early return
                }
                
                assert( wasVisited(root) );
                switch(root.uuid)
                {
                    case Terminal::  UUID: return true;
                        
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

