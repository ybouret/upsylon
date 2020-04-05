
//! \file

#ifndef Y_JARGON_AXIOM_COMPOUND_INCLUDED
#define Y_JARGON_AXIOM_COMPOUND_INCLUDED 1

#include "y/jargon/axiom/internal.hpp"

namespace upsylon {
    
    namespace Jargon {
        
       
        typedef NodeOf<Axiom>             Member;  //!< member of a manifest
        typedef core::list_of_cpp<Member> Manifest;//! a manisfest is a list of members
        
       
        //----------------------------------------------------------------------
        //
        //! base class for Compound axioms (Aggregate|Alternate)
        //
        //----------------------------------------------------------------------
        class Compound : public Internal, public Manifest
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            static const char VanishingMark = '#'; //!< for vanishing compound labels
            
            //------------------------------------------------------------------
            //
            // building methods
            //
            //------------------------------------------------------------------
            void add(Axiom &);                         //!< axiom must be dynamic
            Compound & operator<<(Axiom &);            //!< syntactic sugar
            void displayMembers(ios::ostream &) const; //!< display each member
            
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            virtual ~Compound() throw(); //!< cleanup
            
            
            
        protected:
            //! setup for derived classes
            template <typename LABEL> inline
            explicit Compound(const LABEL &l, const uint32_t u) :
            Internal(l,u), Manifest() {}
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compound);
        };
    }
    
}

#endif
