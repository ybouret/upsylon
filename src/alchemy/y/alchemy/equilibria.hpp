
//! \file

#ifndef Y_ALCHEMY_EQUILIBRIA_INCLUDED
#define Y_ALCHEMY_EQUILIBRIA_INCLUDED 1

#include "y/alchemy/equilibrium.hpp"

namespace upsylon
{
    namespace alchemy
    {
        
        
        class equilibria
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef  hash_set<string,equilibrium::pointer> db_type;
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit equilibria();
            virtual ~equilibria() throw();
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
           
            //! register a new equilibrium
            equilibrium & operator()(equilibrium *);
            
            //! helper for a new constant equilibrium
            template <typename ID> inline
            equilibrium & operator()(const ID &id, const double K)
            {
                return (*this)( new constant_equilibrium(id,K) );
            }
            
            std::ostream & display(std::ostream &, const library &, const double t) const;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(equilibria);
            db_type      db;
            
        public:
            const size_t max_name; //!< max equilibrium name lentgth
        };
        
    }
}

#endif

