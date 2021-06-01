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
                typedef core::list_of_cpp<Axiom> List;      //!< alias
                typedef Axiom                   *Handle;    //!< alias
                typedef suffix_storage<Handle>   Registry;  //!< alias

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
                //! setup
                template <typename ID> inline
                explicit Axiom(const ID &i, const uint32_t t) : name( Tags::Make(i) ), uuid( t ) {}


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Axiom);
            };

        }
    }
}

#endif

