//! \file

#ifndef Y_ALCHEMY_ACTORS_INCLUDED
#define Y_ALCHEMY_ACTORS_INCLUDED 1

#include "y/alchemy/actor.hpp"
#include "y/associative/hash/set.hpp"
#include "y/code/compilable.hpp"

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
        class actors : public compilable
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            static const char                          clid[]; //!< identifier
            typedef hash_set<string,actor>             db_type;        //!< alias
            typedef db_type::const_iterator            const_iterator; //!< alias
            
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
            friend OSTREAM &  operator<<(OSTREAM &os, const actors &A)
            {
                if(A.db.size()>=1)
                {
                    const_iterator it = A.db.begin();
                    os << *it;
                    while( ++it != A.db.end() )
                    {
                        os << " + " << *it;
                    }
                }
                return os;
            }
            
            //! display compact format
            template <typename OSTREAM> inline
            OSTREAM & display_code(OSTREAM &os) const
            {
                const size_t n = db.size();
                os << '[';
                for(size_t i=1;i<=n;++i)
                {
                    os << ' ' << '(' << coef[i] << '*' << '@' << indx[i] << ')';
                }
                os << ' ' << ']';
                return os;
            }
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t * const indx; //!< compiled index of active species
            const size_t * const coef; //!< compiled coefficients of active species
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(actors);
            virtual void on_compile();
            db_type      db;
            size_t       wlen;
        public:
            const size_t       cwidth; //!< max coefficients decimal chars
        };
        
    }
    
}

#endif

