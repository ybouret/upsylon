
//! \file

#ifndef Y_JARGON_AXIOM_INTERNAL_INCLUDED
#define Y_JARGON_AXIOM_INTERNAL_INCLUDED 1

#include "y/jargon/axiom.hpp"
namespace upsylon {
    
    namespace Jargon {
        
        //----------------------------------------------------------------------
        //
        //! base class for internal axiom
        //
        //----------------------------------------------------------------------
        class Internal : public Axiom
        {
        public:
            virtual ~Internal() throw(); //!< cleanup
            
        protected:
            //! setup
            template <typename LABEL> inline
            explicit Internal(const LABEL &id, const uint32_t u) :
            Axiom(id,u)
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Internal);
            virtual const char * vizShape() const throw();
            virtual const char * vizStyle() const throw();
        };
        
        //----------------------------------------------------------------------
        //
        //! dummy Internal for testing
        //
        //----------------------------------------------------------------------
        class Internal_ : public Internal
        {
        public:
            //! cleanup
            virtual ~Internal_() throw();
            
            //! setup
            template <typename LABEL> inline
            explicit Internal_(const LABEL &id, const uint32_t u) :
            Internal(id,u)
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Internal_);
            virtual bool           accept(Y_JARGON_AXIOM_ARGS) const;
            virtual ios::ostream & display(ios::ostream &)     const;
        };
    }
    
}

#endif

