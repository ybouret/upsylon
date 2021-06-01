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

                //! get derived class
                template <typename T> T &as() throw()
                {
                    assert(self); assert(uuid==T::UUID);
                    return *static_cast<T*>(self);
                }

                //! get derived class, const
                template <typename T> const T &as() const throw()
                {
                    assert(self); assert(uuid==T::UUID);
                    return *static_cast<T*>(self);
                }

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Tag      name; //!< label
                const uint32_t uuid; //!< UUID

            protected:
                //! setup
                template <typename ID> inline
                explicit Axiom(const ID &i, const uint32_t t) : name( Tags::Make(i) ), uuid( t ), self(NULL) {}

                //! signature, mandatory in derived constructors
                template <typename T> inline
                void I_am() throw()
                {
                    aliasing::_(self) = static_cast<T*>(this);
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Axiom);
                void    *self; //!< derived class pointer
            };

        }
    }
}

#endif

