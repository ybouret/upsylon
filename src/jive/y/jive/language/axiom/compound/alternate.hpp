//! \file

#ifndef Y_JIVE_LANG_ALTERNATE_INCLUDED
#define Y_JIVE_LANG_ALTERNATE_INCLUDED 1

#include "y/jive/language/axiom/compound.hpp"


namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {

            //__________________________________________________________________
            //
            //
            //! choice in a list of axioms
            //
            //__________________________________________________________________
            class Alternate :  public Compound
            {
            public:
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
              
                //! UUID
                static const uint32_t UUID = Y_FOURCC('A','L','T','R');

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~Alternate() throw(); //!< cleanup

                //! setup
                template <typename ID> inline
                explicit Alternate(const ID &i) :
                Compound(i,UUID)
                {
                    I_am<Alternate>();
                }
                
                Y_LANG_AXIOM_DECL();


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Alternate);
            };

        }

    }

}

#endif
