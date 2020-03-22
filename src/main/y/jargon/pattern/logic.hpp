//! \file
#ifndef Y_JARGON_PATTERN_LOGIC_INCLUDED
#define Y_JARGON_PATTERN_LOGIC_INCLUDED 1

#include "y/jargon/pattern/operands.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! Logical operations based pattern
        class Logical : public Pattern, public Operands
        {
        public:
            virtual ~Logical() throw();                //!< cleanup
            void     add(const uint8_t );              //!< new single
            void     add(const uint8_t,const uint8_t); //!< new range
            
        protected:
            explicit Logical(const uint32_t) throw();   //!< forward uuid
            Logical(const Logical &);                   //!< full copy
            size_t   srz(ios::ostream &fp) const;       //!< serialize
            void     viz(ios::ostream &fp) const;       //!< graphviz
            
        private:
            Y_DISABLE_ASSIGN(Logical);
            virtual size_t serialize( ios::ostream &) const;
            
        public:
            static Pattern *Compact(Logical *p); //!< return head pattern/delete logical if only one item
            static Logical *NoMulti(Logical *p); //!< remove multiple alike, use Patten::RemoveRedundant
            
            static Pattern *Equal(const string &); //!< AND(content)
            static Pattern *Among(const string &); //!< OR(content)
            
        private:
            static Logical *Feed( Logical *p, const string &s ); //!< create a single for each char of s
        };
        
    }
    
}


#endif
