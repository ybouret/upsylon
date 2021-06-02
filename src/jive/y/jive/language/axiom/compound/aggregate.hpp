//! \file

#ifndef Y_JIVE_LANG_AGGREGATE_INCLUDED
#define Y_JIVE_LANG_AGGREGATE_INCLUDED 1

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
            //! gather a list of axioms
            //
            //__________________________________________________________________
            class Aggregate :  public Compound
            {
            public:
                //______________________________________________________________
                //
                // types and definition
                //______________________________________________________________
                static const uint32_t UUID = Y_FOURCC('A','G','G','R');

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~Aggregate() throw();

                template <typename ID> inline
                explicit Aggregate(const ID &i) :
                Compound(i,UUID)
                {
                    I_am<Aggregate>();
                }

                Y_LANG_AXIOM_DECL();


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Aggregate);
            };

        }

    }

}

#endif
