
//! \file

#ifndef Y_JARGON_AXIOM_COMPOUND_INCLUDED
#define Y_JARGON_AXIOM_COMPOUND_INCLUDED 1

#include "y/jargon/axiom/internal.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Member : public Object, public inode<Member>, public Canon
        {
        public:
            explicit Member(const Canon &) throw();
            virtual ~Member() throw();
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Member);
        };
        
        typedef core::list_of_cpp<Member> Manifest;
        
        class Compound : public Internal, public Manifest
        {
        public:
            
            virtual ~Compound() throw();
            
            //! axiom must be dynamic
            void add(Axiom &axiom);
            
            //! syntactic sugar
            Compound & operator<<( Axiom &axiom );
            
        protected:
            template <typename LABEL> inline
            explicit Compound(const LABEL &id, const uint32_t u) :
            Internal(id,u), Manifest()
            {
            }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Compound);
            
        };
    }
    
}

#endif
