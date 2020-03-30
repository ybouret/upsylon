//! \file

#ifndef Y_JARGON_AXIOM_INCLUDED
#define Y_JARGON_AXIOM_INCLUDED 1

#include "y/jargon/xnode.hpp"
#include "y/jargon/tags.hpp"
#include "y/type/fourcc.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! base class for a syntax axiom
        class Axiom : public  CountedObject
        {
        public:
            const Tag      label; //!< shared label
            const uint32_t uuid;  //!< UUID
            
            virtual ~Axiom() throw();
            
            virtual const char * vizShape() const throw() = 0; //!< graphViz shape
            virtual const char * vizStyle() const throw() = 0; //!< graphViz sty;e

            
        protected:
            //! setup
            template <typename LABEL> inline
            Axiom( const LABEL &id, const uint32_t u) :
            label( Tags::Make(id) ),
            uuid(u)
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Axiom);
        };
        
        //! inactive axiom, for memory
        class Inactive : public Axiom
        {
        public:
            static const uint32_t UUID = Y_FOURCC('D','U','L','L'); //!< UUID
            virtual ~Inactive() throw() {}
            
            //! setup
            template <typename LABEL> inline
            explicit Inactive(const LABEL &id) : Axiom(id, UUID)
            {
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Inactive);
            virtual const char * vizShape() const throw() { return "oval"; }
            virtual const char * vizStyle() const throw() { return "dotted";  }
        };
       
        //! base class for internal axiom
        class Internal : public Axiom
        {
        public:
            
            virtual ~Internal() throw() {} //!< cleanup
            template <typename LABEL> inline
            explicit Internal(const LABEL &id, const uint32_t u) : Axiom(id,u)
            {
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Internal);
            virtual const char * vizShape() const throw() { return "house"; }
            virtual const char * vizStyle() const throw() { return "solid";  }
        };
        
        
    }
}

#endif

