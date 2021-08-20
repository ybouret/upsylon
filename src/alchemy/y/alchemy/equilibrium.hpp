//! \file

#ifndef Y_ALCHEMY_EQUILIBRIUM_INCLUDED
#define Y_ALCHEMY_EQUILIBRIUM_INCLUDED 1

#include "y/alchemy/actors.hpp"
#include "y/associative/hash/set.hpp"

namespace upsylon
{


    namespace Alchemy
    {

        class Library; //!< forward declaration

        //______________________________________________________________________
        //
        //
        //! base class for equilibrium
        //
        //______________________________________________________________________
        class Equilibrium : public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef intr_ptr<string,Equilibrium>       Pointer;   //!< alias
            typedef hash_set<string,Pointer>           Set;       //!< alias
            typedef Set::node_type                     Node;      //!< alias

            static const unsigned Default = 0x00; //!< will check charges
            static const unsigned Minimal = 0x01; //!< will check coprimality of coefficients
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~Equilibrium() throw();        //!< cleanup
            virtual  double K(double) const = 0;   //!< constant at given time
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void operator()(const long nu, const Species &sp); //!< register
            const string & key() const throw();                //!< for hash_set

            void parse(const string &, Library &); //!< parse integer blank species



           
            //! display at this tdisp
            template <typename OSTREAM> inline
            friend OSTREAM &operator<<(OSTREAM &os, const Equilibrium &eq)
            {
                os << '<' << eq.name << '>' << ' ';
                os << eq.reac << " <=> " << eq.prod;
                os << vformat(" (%.15g)", eq.K(0));
                return os;
            }
            

            //! fill coefficients row of signed integer type
            template <typename T> inline
            void fill(addressable<T> &Nu) const throw()
            {
                reac.fill(Nu,-1);
                prod.fill(Nu, 1);
            }

            //! verify
            /**
             - charge conservation (Default=always)
             - co-primality        (flags&Minimal)
             */
            void verify(const unsigned flags) const;

            //! find extents
            Extents findExtents(const Accessible &C) const throw();

            //! compute K0*reac - prod
            double  compute(const double K0, const Accessible &C) const throw();

            //! compute with extent
            double  compute(const double K0, const Accessible &C, const double xi) const throw();

            //! solve unique equilibrium
            bool   solve(const double K0, Addressable  &C) const;

            //! compute jacobian row and return K0*reac-prod
            double compute(Addressable &phi, const double K0, const Accessible &C) const throw();

            //! guess who is active among reac and prod
            void guess(addressable<bool> &active) const throw();

            //! test ownership
            bool owns(const Species &sp) const throw();

            //! positive coefficient, for balancing
            size_t weight(const Species &sp) const throw();


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const size_t   indx;  //!< index
            const string   name;  //!< identifier
            const Actors   reac;  //!< reactants
            const Actors   prod;  //!< products

        protected:
            //! initialize
            template <typename ID> inline
            explicit Equilibrium(const ID &id) : indx(0), name(id), reac(), prod() {}

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Equilibrium);
        };
        
        //______________________________________________________________________
        //
        //
        //! a truly constant equilibrium
        //
        //______________________________________________________________________
        class ConstEquilibrium : public Equilibrium
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~ConstEquilibrium() throw(); //!< cleanup

            //! initialize
            template <typename ID> inline
            explicit ConstEquilibrium(const ID &id, const double k) :
            Equilibrium(id), Kvalue(k) {}

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual double K(double) const; //!< return constant value
            
        private:
            const double Kvalue;
            Y_DISABLE_COPY_AND_ASSIGN(ConstEquilibrium);
        };

    }
    
}

#endif

