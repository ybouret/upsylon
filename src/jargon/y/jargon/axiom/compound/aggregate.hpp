
//! \file

#ifndef Y_JARGON_AXIOM_AGG_INCLUDED
#define Y_JARGON_AXIOM_AGG_INCLUDED 1

#include "y/jargon/axiom/compound.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! Aggregate compound, must accept all members
        //
        //----------------------------------------------------------------------
        class Aggregate : public Compound
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            static const uint32_t UUID = Y_FOURCC('A', 'G', 'G', 'R'); //!< UUID
            
            //------------------------------------------------------------------
            //
            // C++
            //
            //------------------------------------------------------------------
            
            //! cleanup
            virtual ~Aggregate() throw();
            
            //! setup
            template <typename LABEL> inline
            explicit Aggregate(const LABEL &l) :
            Compound(l,UUID)
            {
                setup();
            }
            
            explicit Aggregate(unsigned &indx);
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Aggregate);
            virtual bool accept(Y_JARGON_AXIOM_ARGS) const;
            virtual const char *vizShape() const throw() { return "house"; }
            void setup();
        };
    }
}
    
#endif
    
