//! \file

#ifndef Y_JIVE_LANG_AXIOM_INCLUDED
#define Y_JIVE_LANG_AXIOM_INCLUDED 1

#include "y/jive/language/node.hpp"

namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {

            //__________________________________________________________________
            //
            //
            //! Axiom
            //
            //__________________________________________________________________
            class Axiom : public CountedObject, public inode<Axiom>
            {
            public:
                const Tag      name;
                const uint32_t uuid;

                string   gvName() const; //!< graphViz name
                virtual ~Axiom() throw();
                
            protected:
                template <typename ID> inline
                explicit Axiom(const ID &i, const uint32_t t) :
                name( Tags::Make(i) ), uuid( t )
                {
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Axiom);
            };

        }
    }
}

#endif

