//! \file

#ifndef Y_ALCHEMY_ACTORS_INCLUDED
#define Y_ALCHEMY_ACTORS_INCLUDED 1

#include "y/alchemy/actor.hpp"
#include "y/associative/hash/set.hpp"

namespace upsylon
{
    namespace alchemy
    {
        //______________________________________________________________________
        //
        //! base class for actors
        //______________________________________________________________________
        typedef hash_set<string,actor> actors_;

        //______________________________________________________________________
        //
        //
        //! collection of actors
        //
        //______________________________________________________________________
        class actors : public actors_
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char clid[]; //!< identifier

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit actors();
            virtual ~actors() throw();


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! register a new actor
            void operator()(const species &sp, const unsigned long nu);

            //! display with widths for names and coefficients
            template <typename OSTREAM> inline
            OSTREAM &  display(OSTREAM &os, const size_t name_width=0, const size_t coef_width=0) const
            {
                if(size()>=1)
                {
                    const_iterator it = begin();
                    (*it).display(os,name_width,coef_width);
                    while( ++it != end() )
                    {
                        os << '+';
                        (*it).display(os,name_width,coef_width);
                    }
                }
                return os;
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t max_coef;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(actors);
        };

    }

}

#endif

