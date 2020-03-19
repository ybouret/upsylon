
//! \file

#ifndef Y_JARGON_PATTERN_OPERANDS_INCLUDED
#define Y_JARGON_PATTERN_OPERANDS_INCLUDED 1

#include "y/jargon/pattern.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        class Operand : public Object, public inode<Operand>, public Motif
        {
        public:
            typedef core::list_of_cpp<Operand> List;
            virtual ~Operand() throw();
            
            static Operand *Create(const Motif   &);
            static Operand *Create(const Pattern *);
            Operand         *clone() const;
            
        private:
            explicit Operand(const Motif &) throw();
            Y_DISABLE_COPY_AND_ASSIGN(Operand);
        };
        
        
        class Operands : public Operand::List
        {
        public:
            explicit Operands() throw();
            virtual ~Operands() throw();
            Operands(const Operands &);
            
            Operands & operator<<(const Motif   &);
            Operands & operator<<(const Pattern *);
            
        private:
            Y_DISABLE_ASSIGN(Operands);
        };
    }
    
}

#endif

