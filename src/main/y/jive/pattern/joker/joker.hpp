//! \file

#ifndef Y_JIVE_PATTERN_JOKER_INCLUDED
#define Y_JIVE_PATTERN_JOKER_INCLUDED 1

#include "y/jive/pattern.hpp"

namespace upsylon
{

    namespace Jive
    {
        //______________________________________________________________________
        //
        //
        //! base class for jokers
        //
        //______________________________________________________________________
        class Joker : public Pattern
        {
        public:

            //__________________________________________________________________
            //
            // pattern common interface
            //__________________________________________________________________
            virtual void     start(FirstChars &) const;         //!< from motif
            virtual void     update(Entropy &)   const throw(); //!< from motif
            virtual void     optimize() throw();                //!< motif->optimize

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Motif motif; //!< the motif which is tested

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Joker() throw(); //!< cleanup

            


        protected:
            explicit Joker(const uint32_t, const Motif &) throw(); //!< setup
            explicit Joker(const Joker&);                          //!< copy

            //! write motif and link to it
            void vizLink(ios::ostream &) const;
            
        private:
            Y_DISABLE_ASSIGN(Joker);
        };

    }

}

#endif

