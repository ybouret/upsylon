
#include "y/jargon/pattern/operands.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Operand:: ~Operand() throw() {}
        
        Operand * Operand:: Create( const Motif   &m ) { return new Operand(m); }
        
        Operand * Operand:: Create( const Pattern *p ) { const Motif m = p; return Create(m); }
        Operand * Operand:: clone() const { const Pattern &self = **this; return Create(self.clone()); }
        
        Operand:: Operand(const Motif &m) throw() : Object(), inode<Operand>(), Motif(m) {}

    }
    
}

namespace upsylon {
    
    namespace Jargon {

        Operands:: ~Operands() throw()
        {
        }
        
        Operands::  Operands() throw() : Operand::List() {}
        
        Operands & Operands:: operator<<( const Motif &m)
        {
            (void) push_back( Operand::Create(m) );
            return *this;
        }
        
        Operands & Operands:: operator<<( const Pattern *p)
        {
            (void) push_back( Operand::Create(p) );
            return *this;
        }
        
    }
}

