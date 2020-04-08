
//! \file

#ifndef Y_JARGON_AXIOM_VISITOR_INCLUDED
#define Y_JARGON_AXIOM_VISITOR_INCLUDED 1

#include "y/jargon/axiom/all.hpp"
#include "y/jargon/xcache.hpp"

namespace upsylon {
    
    namespace Jargon {
      
        //----------------------------------------------------------------------
        //
        //! using database to visit once the presented axioms
        //
        //----------------------------------------------------------------------
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
            
            //! apply proc on each not-visited axiom from root
            bool walkDown(const Axiom &root,
                          Process      proc,
                          void        *args);
            
            //! return true
            static bool Check(const Axiom &, void *) throw();
            
            //! populate addresses
            void supplyWith(const Axiom &);
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Visitor);
        };
        
    }
    
}

#endif
