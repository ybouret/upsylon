
//! \file

#ifndef Y_ALCHEMY_EQUILIBRIA_INCLUDED
#define Y_ALCHEMY_EQUILIBRIA_INCLUDED 1

#include "y/alchemy/equilibrium.hpp"
#include "y/container/matrix.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        //______________________________________________________________________
        //
        //
        //! database of equilibria
        //
        //______________________________________________________________________
        class Equilibria :  public gateway<const Equilibrium::Set>
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef type::const_iterator const_iterator; //!< alias
            static  const char           CLID[];         //!< Equilibria

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Equilibria();
            virtual ~Equilibria() throw();



            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! register a new equilibrium
            Equilibrium & operator()(Equilibrium *);

            //! helper for a new constant equilibrium
            template <typename ID> inline
            Equilibrium & operator()(const ID &id, const double K)
            {
                return (*this)( new ConstEquilibrium(id,K) );
            }


            //! display at tdisp
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Equilibria &eqs)
            {
                for(const_iterator it=eqs->begin();it!=eqs->end();++it)
                {
                    const Equilibrium &eq = **it;
                    os << '<' << eq.name << '>';
                    for(size_t i=eq.name.size();i<eqs.enw;++i) os << ' ';
                    os << ' ';
                    os << eq.reac << " <=> " << eq.prod;
                    os << vformat( " (%.15g) ", eq.K(eqs.tdisp) );
                    os << '\n';
                }
                return os;
            }

            template <typename T> inline
            void fill(matrix<T> &Nu) const
            {
                size_t row = 0;
                for(const_iterator it=edb.begin();it!=edb.end();++it)
                {
                    const Equilibrium &eq = **it;
                    eq.fill(Nu[++row]);
                }
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            virtual const_type & bulk() const throw();

            Equilibrium::Set edb; //!< database
            const size_t     enw; //!< equilibrium name width
            
        public:
            mutable double   tdisp; //!< to display status at specific time

        };


    }
}

#endif

