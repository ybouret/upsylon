
//! \file

#ifndef Y_JARGON_AXIOM_VISITOR_INCLUDED
#define Y_JARGON_AXIOM_VISITOR_INCLUDED 1

#include "y/jargon/axiom/all.hpp"
#include "y/jargon/axiom/db.hpp"
#include "y/jargon/xcache.hpp"

namespace upsylon {
    
    namespace Jargon {
      
        
        //! light database of visited axioms
        class Visitor : public Axiom::DB
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            
            //! process to apply to each axiom, only once
            typedef bool (*Process)(const Axiom &, void *);
          
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            explicit Visitor();          //!< setup
            virtual ~Visitor() throw();  //!< cleanup
            
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            
            //! register axiom, return true if was not visited
            //bool greenLight( const Axiom & );
            
            //! check if was visited
            //bool wasVisited( const Axiom & ) const throw();
          
            //! apply proc on each not-visited axiom from root
            bool walkDown(const Axiom &root,
                          Process      proc,
                          void        *args);
            
            //! return true
            static bool Check(const Axiom &, void *) throw();
            
            //! populate addresses
            void supplyWith(const Axiom &);
            
            //! display the tree
            void display() const;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Visitor);
        };
        
    }
    
}

#endif
