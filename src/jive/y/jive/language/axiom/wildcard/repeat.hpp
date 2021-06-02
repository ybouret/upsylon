
#ifndef Y_JIVE_LANG_REPEAT_INCLUDED
#define Y_JIVE_LANG_REPEAT_INCLUDED 1

#include "y/jive/language/axiom/wildcard.hpp"


namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {

            //__________________________________________________________________
            //
            //
            //! make a repeating axiom >= atLeast
            //
            //__________________________________________________________________
            class Repeat : public Wildcard
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static const uint32_t UUID = Y_FOURCC('R', 'E', 'P', 'T'); //!< UUID

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                //! setup
                template <typename ID> inline
                explicit Repeat(const ID    &i,
                                const Axiom &a,
                                const size_t n,
                                const bool   m ) :
                Wildcard(i,UUID,a), atLeast(n), hasMark(m)
                {}

                //! cleanup
                virtual ~Repeat() throw();

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                Y_LANG_AXIOM_DECL();
                static string Mark(const size_t); //!< according to size

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const size_t atLeast;             //!< counting limit
                const bool   hasMark;             //!< named or automatic

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Repeat);
            };

        }
    }
}

#endif
