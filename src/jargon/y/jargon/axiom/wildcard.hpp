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
            const Canon canon; //!< shared axiom to use
            
            
            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------
            const  Axiom  & operator*() const throw();                    //!< access the Axiom
            static string * MakeTag(const char mark, const string &name); //!< proper tag for derived class
           
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            virtual ~Wildcard() throw(); //!< cleanup

        protected:
            //! setup for derived classes
            template <typename LABEL> inline
            explicit Wildcard(const LABEL   &id,
                              const uint32_t u,
                              Axiom         &axiom ) :
            Internal(id,u), canon( checkShared(axiom) )
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Wildcard);
            Axiom * checkShared(Axiom &axiom) const;
            
        };
    }
    
}

#endif
