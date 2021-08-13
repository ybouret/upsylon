
//! \file

#ifndef Y_ALCHEMY_EQUILIBRIA_INCLUDED
#define Y_ALCHEMY_EQUILIBRIA_INCLUDED 1

#include "y/alchemy/equilibrium.hpp"
#include "y/alchemy/freezable.hpp"
#include "y/container/matrix.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        typedef matrix<double> Matrix;  //!< alias
        typedef matrix<unit_t> iMatrix; //!< alias
        class                  Library; //!< forward declaration
        
        //______________________________________________________________________
        //
        //
        //! database of equilibria
        //
        //______________________________________________________________________
        class Equilibria : public gateway<const Equilibrium::Set>, public Freezable
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

            //! slow access by index
            const Equilibrium & operator()(const size_t indx) const throw();

            //! print aligned eq name
            template <typename OSTREAM> inline
            OSTREAM & print(OSTREAM &os, const Equilibrium &eq) const
            {
                os << '<' << eq.name << '>';
                for(size_t i=eq.name.size();i<enw;++i) os << ' ';
                return os;
            }

            //! display at tdisp (initially set to 0.0)
            template <typename OSTREAM> inline
            friend OSTREAM & operator<<(OSTREAM &os, const Equilibria &eqs)
            {
                for(const_iterator it=eqs->begin();it!=eqs->end();++it)
                {
                    const Equilibrium &eq = **it;
                    eqs.print(os,eq) << ' ';
                    os << eq.reac << " <=> " << eq.prod;
                    os << vformat( " (%.15g) ", eq.K(eqs.tdisp) );
                    os << '\n';
                }
                return os;
            }

            //! fill topology matrix (eqs x components)
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

            //! verify each equilibrium
            void verify() const;

            //! compute K only
            void compute(Addressable &K, const double t) const throw();

            //! compute K and Gam for C at t
            void compute(Addressable      &K,
                         Addressable      &Gam,
                         const Accessible &C,
                         const double      t) const throw();

            //! upgrade Gam for C with pre-computed K
            void upgrade(const Accessible &K,
                         Addressable      &Gam,
                         const Accessible &C) const throw();

            //! compute K, then Gam and Phi for C at t
            void compute(Addressable      &K,
                         Addressable      &Gam,
                         Matrix           &Phi,
                         const Accessible &C,
                         const double      t) const throw();

            //! upgrade Gam and Phi for C with pre-computed K
            void upgrade(const Accessible &K,
                         Addressable      &Gam,
                         Matrix           &Phi,
                         const Accessible &C) const throw();

            //! guess all active species
            size_t guess(addressable<bool> &active) const throw();

            //! helper: create Water
            Equilibrium &water(Library &, const double);
            





        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibria);
            virtual const_type & bulk() const throw();

            Equilibrium::Set    edb; //!< database
            
        public:
            const size_t        enw; //!< equilibrium name width
            mutable double      tdisp; //!< to display status at specific time

        };


    }
}

#endif

