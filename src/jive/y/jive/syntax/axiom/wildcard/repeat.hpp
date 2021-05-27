

//! \file
#ifndef Y_JIVE_SYNTAX_REPEAT_INCLUDED
#define Y_JIVE_SYNTAX_REPEAT_INCLUDED 1

#include "y/jive/syntax/axiom/wildcard.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {

            //__________________________________________________________________
            //
            //
            //! Repeat an Axiom >= atLeast
            //
            //__________________________________________________________________
            class Repeat : public Wildcard
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static const uint32_t UUID = Y_FOURCC('R','E','P',' '); //!< UUID

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                
                //! cleanup
                virtual ~Repeat() throw();

                //! setup
                template <typename ID> inline
                explicit Repeat(const ID    &i,
                                const Axiom &a,
                                const size_t n,
                                const bool   f) :
                Wildcard(i,a,UUID),
                atLeast(n),
                hasMark(f)
                {
                    Y_JIVE_AXIOM(Repeat);
                }
                
                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                //! create an extension mark depending on n
                /**
                 - used for automatic labelling
                 - used for graphViz is !hasMark
                 */
                static string CreateMark(const size_t n);
                
                
                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const size_t atLeast; //!< validity threshold
                const bool   hasMark; //!< if name contains atLeast

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Repeat);
                Y_JIVE_AXIOM_ACCEPT_DECL();
            };


        }

    }

}

#endif
