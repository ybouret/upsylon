
//! \file

#ifndef Y_JARGON_XCACHE_INCLUDED
#define Y_JARGON_XCACHE_INCLUDED 1

#include "y/jargon/axiom/inactive.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! cache of syntax xnode for tree construction
        //
        //------------------------------------------------------------------
        class XCache : public XList
        {
        public:
            
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
           
            ///! cleanup
            virtual ~XCache() throw();

            //! setup
            template <typename LABEL> inline
            explicit XCache(const LABEL &inactiveID, size_t n=0 ) :
            dull( new Inactive(inactiveID) )
            {
                reserve(n);
            }
            
            //------------------------------------------------------------------
            //
            // I/O methods
            //
            //------------------------------------------------------------------
            void reserve( size_t n);                  //!< reserve inactive nodes
            XNode *query();                           //!< query an inactive node...
            XNode *query(const Internal &);           //!< query an internal node
            XNode *query(const Terminal &, Lexeme *); //!< query a terminal node
            void   store(XNode *xnode) throw();       //!< clean and store a node
            
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            
            //! shared inactive axiom
            const arc_ptr<Inactive> dull;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(XCache);
            
        };
    }
    
}

#endif
