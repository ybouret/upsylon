
#include "y/aqua/equilibria.hpp"
#include "y/exception.hpp"
#include "y/type/utils.hpp"

namespace upsylon {

    namespace Aqua
    {

        Equilibria:: ~Equilibria() throw()
        {
        }

        Equilibria:: Equilibria() :
        suffix_tree<Equilibrium::Pointer>(),
        maxNameSize(0)
        {}

        Equilibrium & Equilibria:: operator()( Equilibrium *eq )
        {
            assert(eq);
            const Equilibrium::Pointer p(eq);
            if(!insert_by(p->name,p))
            {
                throw exception("multiple equilibrium <%s>",*(p->name));
            }
            maxNameSize = max_of(maxNameSize,p->name.size());
            return *eq;
        }

        std::ostream & Equilibria:: display(std::ostream &os) const
        {
            os << "<equilibria>" << std::endl;
            for(const_iterator it=begin();it!=end();++it)
            {
                const Equilibrium &eq = **it;
                eq.display(os << ' ',maxNameSize) << std::endl;
            }
            os << "<equilibria/>";
            return os;
        }

        std::ostream & operator<<(std::ostream &os, const Equilibria &eqs)
        {
            return eqs.display(os);
        }

#if 0
        void Equilibria:: fillNu( iMatrix &nu ) const throw()
        {
            assert(nu.rows==entries());
            size_t i=0;
            for(const_iterator it=begin();it!=end();++it)
            {
                (**it).fillNu(nu[++i]);
            }
        }

        void Equilibria:: computeK(addressable<double> &K, const double t) const
        {
            assert(K.size()==entries());
            size_t i=0;
            for(const_iterator it=begin();it!=end();++it)
            {
                K[++i] = (**it).K(t);
            }
        }

        void Equilibria:: validate() const
        {
            for(const_iterator it=begin();it!=end();++it)
            {
                (**it).validate();
            }
        }

        void Equilibria:: computePhi(Matrix                   &Phi,
                                     const accessible<double> &K,
                                     const accessible<double> &C) const throw()
        {
            assert(Phi.rows==entries());
            assert(K.size()==entries());
            assert(C.size()>=Phi.cols);

            size_t i=0;
            for(const_iterator it=begin();it!=end();++it)
            {
                ++i;
                (**it).computePhi(Phi[i],K[i],C);
            }

        }

        void Equilibria:: computeQ(addressable<double> &Q, const accessible<double> &K, const accessible<double> &C) const throw()
        {
            assert(Q.size()==entries());
            assert(K.size()==entries());
            assert(C.size()>=entries());
            size_t i=0;
            for(const_iterator it=begin();it!=end();++it)
            {
                ++i;
                Q[i] = (**it).computeQ(K[i],C);
            }

        }
#endif

    }

}


