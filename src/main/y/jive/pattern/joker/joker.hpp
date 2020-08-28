//! \file

#ifndef Y_JIVE_PATTERN_JOKER_INCLUDED
#define Y_JIVE_PATTERN_JOKER_INCLUDED 1

#include "y/jive/pattern.hpp"

namespace upsylon
{

    namespace Jive
    {

        class Joker : public Pattern
        {
        public:

            const Motif motif;

            virtual ~Joker() throw();


        protected:
            explicit Joker(const uint32_t, const Motif &) throw();
            explicit Joker(const Joker&);

            //! write motif and link to it
            void vizLink(ios::ostream &) const;
            
        private:
            Y_DISABLE_ASSIGN(Joker);
        };

    }

}

#endif

