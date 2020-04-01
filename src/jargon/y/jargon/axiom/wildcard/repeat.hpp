

//! \file

#ifndef Y_JARGON_AXIOM_REPEAT_INCLUDED
#define Y_JARGON_AXIOM_REPEAT_INCLUDED 1

#include "y/jargon/axiom/wildcard.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        class Repeat : public Wildcard
        {
        public:
            const size_t minimalCount;
            virtual ~Repeat() throw();
                        
        protected:
            explicit Repeat(Axiom         &axiom,
                            const size_t   nmin,
                            const uint32_t user,
                            const char     mark);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Repeat);
            virtual bool accept(Y_JARGON_AXIOM_ARGS) const;
        };
        
        template <const size_t NMIN, const char MARK>
        class Rep : public Repeat
        {
        public:
            static const char     Mark         = MARK;
            static const uint32_t UUID         = Y_FOURCC('R','E','P',MARK);
            static const size_t   MinimalCount = NMIN;
            inline virtual ~Rep() throw() {}
            inline explicit Rep(Axiom &axiom) :
            Repeat(axiom,MinimalCount,UUID,Mark)
            {
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Rep);
        };
        
        typedef Rep<0,'*'> ZeroOrMore;
        typedef Rep<1,'+'> OneOrMore;
        
    }
    
}

#endif
