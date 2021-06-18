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

                //! UUID
                static const uint32_t UUID = Y_FOURCC('A','G','G','R');

                //! type of aggregate
                enum Type
                {
                    Standard, //!< a named entity
                    Variadic, //!< merge if only one member
                    Grouping  //!< transient => always merge
                };

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~Aggregate() throw(); //!< cleanup

                //! setup
                template <typename ID> inline
                explicit Aggregate(const ID &i, const Type t) :
                Compound(i,UUID), type(t)
                {
                    I_am<Aggregate>();
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                Y_LANG_AXIOM_DECL();
                bool             isGrouping() const throw(); //!< type == Grouping
                bool             isApparent() const throw(); //!< type != Grouping
                const Aggregate *asApparent() const throw(); //!< this if isApparent

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                const Type type; //!< acting as...

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Aggregate);
            };

        }

    }

}

#endif
