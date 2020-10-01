//! \file

#ifndef Y_AQUA_LIBRARY_INCLUDED
#define Y_AQUA_LIBRARY_INCLUDED 1

#include "y/aqua/species.hpp"
#include "y/associative/suffix/tree.hpp"

namespace upsylon {

    namespace Aqua
    {

        //______________________________________________________________________
        //
        //
        //! a library of species
        //
        //______________________________________________________________________
        class Library : public suffix_tree<Species::Pointer>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Library() throw(); //!< cleanup
            explicit Library();         //!< setup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! create a new species
            template <typename ID>
            Species & operator()(const ID &name, const int Z)
            {
                const  Species::Pointer sp = new Species(name,Z);
                return create(sp);
            }

            std::ostream        & display(std::ostream &) const;            //!< display
            friend std::ostream & operator<<(std::ostream&,const Library&); //!< display
            void buildIndices() throw();                                    //!< build indices for species
            void show(std::ostream &, const accessible<double> &) const;    //!< readable concentrations

            const size_t maxNameSize; //!< for names alignment


        private:
            Y_DISABLE_ASSIGN(Library);
            Species & create(const Species::Pointer &sp);
        };

    }

}

#endif
