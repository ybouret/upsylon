//! \file

#ifndef Y_JARGON_AXIOM_INCLUDED
#define Y_JARGON_AXIOM_INCLUDED 1

#include "y/jargon/xnode.hpp"
#include "y/jargon/tags.hpp"
#include "y/type/fourcc.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        class Axiom : public  CountedObject
        {
        public:
            const Tag      label;
            const uint32_t uuid;
            
            virtual ~Axiom() throw();
            
            virtual const char * vizShape() const throw() = 0;
            virtual const char * vizStyle() const throw() = 0;

            
        protected:
            template <typename LABEL> inline
            Axiom( const LABEL &id, const uint32_t u) :
            label( Tags::Make(id) ),
            uuid(u)
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Axiom);
        };
        
        class Inactive : public Axiom
        {
        public:
            static const uint32_t UUID = Y_FOURCC('D','U','L','L');
            virtual ~Inactive() throw() {}
            template <typename LABEL> inline
            explicit Inactive(const LABEL &id) : Axiom(id, UUID)
            {
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Inactive);
            virtual const char * vizShape() const throw() { return "circle"; }
            virtual const char * vizStyle() const throw() { return "dotted";  }
            
        };
        
        class Terminal : public Axiom
        {
        public:
            static const uint32_t UUID = Y_FOURCC('T','E','R','M');
            
            virtual ~Terminal() throw() {}
            
            template <typename LABEL>
            explicit Terminal(const LABEL &id) :
            Axiom(id,UUID)
            {}
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Terminal);
            virtual const char * vizShape() const throw() { return "rectangle"; }
            virtual const char * vizStyle() const throw() { return "solid";  }
        };
    }
}

#endif

