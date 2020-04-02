
//! \file

#ifndef Y_JARGON_AXIOM_VISITOR_INCLUDED
#define Y_JARGON_AXIOM_VISITOR_INCLUDED 1

#include "y/jargon/axiom/all.hpp"
#include "y/jargon/xcache.hpp"
#include "y/container/key-address.hpp"

namespace upsylon {
    
    namespace Jargon {
      
        typedef key_address<1>            AxiomAddress;
        typedef suffix_tree<AxiomAddress> AxiomAddresses;
        
        class Visitor : public AxiomAddresses
        {
        public:
            typedef bool (*Process)(const Axiom &, void *);
            
            explicit Visitor();
            virtual ~Visitor() throw();
            
            //! register axiom, return true if was not visited
            bool greenLight( const Axiom & );
            
            //! check if was visited
            bool wasVisited( const Axiom & ) const throw();
          
            //! apply proc on each not-visited axiom from root
            bool walkDown(const Axiom &root,
                          Process      proc,
                          void        *args);
            
            //! return true
            static bool Check(const Axiom &, void *) throw();
            
            //! populate addresses
            void supplyWith(const Axiom &);
            
            void display() const;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Visitor);
        };
        
    }
    
}

#endif
