
#ifndef Y_AQUA_LIBRARY_INCLUDED
#define Y_AQUA_LIBRARY_INCLUDED 1

#include "y/aqua/species.hpp"
#include "y/associative/suffix/tree.hpp"

namespace upsylon {

    namespace Aqua
    {

        //! a library of species
        class Library : public suffix_tree<Species::Pointer>
        {
        public:
            virtual ~Library() throw(); //!< cleanup
            explicit Library();         //!< setup

            //! create a new species
            template <typename ID>
            Species & operator()(const ID &name, const int Z)
            {
                const  Species::Pointer sp = new Species(name,Z);
                return create(sp);
            }


            std::ostream        & display(std::ostream &) const;            //! display
            friend std::ostream & operator<<(std::ostream&,const Library&); //!< display

            // members
            const size_t maxNameSize; //!< for names alignment

        private:
            Y_DISABLE_ASSIGN(Library);
            Species & create(const Species::Pointer &sp);
        };

    }

}

#endif
