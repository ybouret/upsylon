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
        class actors
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char clid[];                       //!< identifier
            typedef hash_set<string,actor>  db_type;        //!< alias
            typedef db_type::const_iterator const_iterator; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit actors();          //!< setup empty
            virtual ~actors() throw();  //!< cleanup



            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void           operator()(const species &, const unsigned long); //!< register a new actor
            bool           search(const string &) const throw();             //!< look for existing
            const db_type &operator*()  const throw();                       //!< content
            const db_type *operator->() const throw();                       //!< content

            //! display with widths for names and coefficients
            template <typename OSTREAM> inline
            OSTREAM &  display(OSTREAM &os, const size_t name_width=0, const size_t coef_width=0) const
            {
                if(db.size()>=1)
                {
                    const_iterator it = db.begin();
                    (*it).display(os,name_width,coef_width);
                    while( ++it != db.end() )
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
        private:
            Y_DISABLE_COPY_AND_ASSIGN(actors);
            db_type      db;
        public:
            const size_t cwidth; //!< max coefficients decimal chars

        };

    }

}

#endif

