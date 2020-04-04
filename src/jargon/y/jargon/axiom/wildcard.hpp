//! \file

#ifndef Y_JARGON_AXIOM_WILDCARD_INCLUDED
#define Y_JARGON_AXIOM_WILDCARD_INCLUDED 1

#include "y/jargon/axiom/internal.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! base class for Wildcards, a smart pointer to an axiom
        //
        //----------------------------------------------------------------------
        class Wildcard : public Internal
        {
        public:
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            static string * MakeTag(const char mark, const string &name); //!< proper tag for derived class
           
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            virtual ~Wildcard() throw(); //!< cleanup

            
            //------------------------------------------------------------------
            //
            // member
            //
            //------------------------------------------------------------------
            const Axiom &axiom; //!< shared axiom to use

        protected:
            //! setup for derived classes
            template <typename LABEL> inline
            explicit Wildcard(const LABEL &l, const uint32_t u, Axiom &a) :
            Internal(l,u), axiom(a)
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Wildcard);
        };
    }
    
}

#endif
