#include "y/aqua/boot.hpp"

namespace upsylon {

    namespace Aqua
    {

        void Boot:: setup() throw()
        {
            keep << aliasing::_(R);
            keep << aliasing::_(tR);
            keep << aliasing::_(pL);
            keep << aliasing::_(S);
            keep << aliasing::_(tS);
            keep << aliasing::_(pS);
        }

        Boot:: ~Boot() throw()
        {
        }

        void Boot:: quit() throw()
        {
            keep.release_all();
            aliasing::_(dL) = 0;
            aliasing::_(dS) = 0;
        }

        Constraint & Boot:: operator()( const double value )
        {
            return * push_back( new Constraint(value) );
        }

        void Boot:: conserve(const double C0, const Species &a)
        {
            Constraint &cc = (*this)(C0);
            cc.add(a,1);
        }

        void Boot:: conserve(const double C0, const Species &a, const Species &b)
        {
            Constraint &cc = (*this)(C0);
            cc.add(a,1);
            cc.add(b,1);
        }

        void Boot:: conserve(const double C0, const Species &a, const Species &b, const Species &c)
        {
            Constraint &cc = (*this)(C0);
            cc.add(a,1);
            cc.add(b,1);
            cc.add(c,1);
        }


        void Boot:: conserve(const double C0, const Species &a, const Species &b, const Species &c, const Species &d)
        {
            Constraint &cc = (*this)(C0);
            cc.add(a,1);
            cc.add(b,1);
            cc.add(c,1);
            cc.add(d,1);
        }


        std::ostream & operator<<(std::ostream &os, const Boot &b)
        {
            os << "<boot #" << b.size << ">" << std::endl;
            for(const Constraint *cc=b.head;cc;cc=cc->next)
            {
                os << ' ' << ' ' << *cc << std::endl;
            }
            os <<  "<boot/>";
            return os;
        }

    }

}

#include "y/aqua/equilibria.hpp"
#include "y/aqua/library.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/gram-schmidt.hpp"
#include "y/mkl/kernel/adjoint.hpp"
#include "y/mkl/simplify.hpp"
#include "y/exception.hpp"
#include "y/aqua/engine.hpp"

namespace upsylon {

    namespace Aqua
    {

        using namespace mkl;

        void Boot:: fill(addressable<double> &Lambda) const throw()
        {
            assert(Lambda.size() == size);
            size_t i=1;
            for(const Constraint *cc=head;cc;cc=cc->next,++i)
            {
                Lambda[i] = cc->value;
            }
        }

       

    }

}


namespace upsylon {

    namespace Aqua
    {
        void Boot:: electroneutrality(const Library &lib)
        {
            Constraint &EN = (*this)(0);
            for(Library::const_iterator it=lib.begin();it!=lib.end();++it)
            {
                const Species &species = **it;
                EN.add(species,species.Z);
            }
        }
    }

}

namespace upsylon {

    namespace Aqua
    {
       
    }
}

