
//! \file

#ifndef Y_JARGON_AXIOM_DB_INCLUDED
#define Y_JARGON_AXIOM_DB_INCLUDED 1

#include "y/jargon/axiom.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        class AxiomDB : public suffix_tree<Axiom::Address>
        {
        public:
            explicit AxiomDB();
            virtual ~AxiomDB() throw();
            
            bool insert(const Axiom &);
            bool search(const Axiom &) const throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(AxiomDB);
        };
    }
    
}

#endif

