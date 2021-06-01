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
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                typedef core::list_of_cpp<Axiom> List;
                typedef Axiom                   *Handle;
                typedef suffix_storage<Handle>   RegistryType;

                //! a registry for Axiom look-up
                class Registry : public RegistryType
                {
                public:
                    explicit Registry();
                    virtual ~Registry() throw();

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(Registry);
                };


                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                string   gvName()  const; //!< create graphViz name
                virtual ~Axiom() throw(); //!< cleanup

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Tag      name; //!< label
                const uint32_t uuid; //!< UUID

            protected:
                template <typename ID> inline
                explicit Axiom(const ID &i, const uint32_t t) : name( Tags::Make(i) ), uuid( t ) {}


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Axiom);
            };

        }
    }
}

#endif

