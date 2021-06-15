

#ifndef Y_JIVE_LANG_COMPOUND_INCLUDED
#define Y_JIVE_LANG_COMPOUND_INCLUDED 1

#include "y/jive/language/axiom.hpp"


namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {
            //__________________________________________________________________
            //
            //
            //! a list of other atoms
            //
            //__________________________________________________________________
            class Compound : public Axiom, public Axiom::Manifest
            {
            public:
                virtual   ~Compound() throw();        //!< cleanup
                Compound & operator<<(const Axiom &); //!< append an axiom

                string     enumerate(const char sep) const;

            protected:
                //! setup
                template <typename ID> inline
                explicit Compound(const ID &i, const uint32_t t) :
                Axiom(i,t),
                Axiom:: Manifest()
                {
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Compound);
            };

        }

    }

}

#endif
