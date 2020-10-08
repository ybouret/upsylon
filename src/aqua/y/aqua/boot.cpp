#include "y/aqua/boot.hpp"
#include "y/mkl/kernel/svd.hpp"
#include "y/counting/comb.hpp"

namespace upsylon {

    namespace Aqua
    {

        Constraint::Actor:: ~Actor() throw()
        {
            aliasing::_(weight) = 0;
        }

        Constraint:: Actor:: Actor(const Species &s, const int w) throw() :
        species(s),
        weight(w)
        {

        }

        void Constraint:: fill(addressable<Int> &P) const throw()
        {
            for(const Actor *a=actors.head;a;a=a->next)
            {
                P[a->species.indx] = a->weight;
            }
        }


        std::ostream & operator<<(std::ostream &os, const Constraint::Actor &a)
        {
            const int coef = a.weight;
            switch(coef)
            {
                case 1: os << '+'; break;
                case 0: return os;
                case -1: os << '-'; break;
                default:
                    if(coef>0)
                    {
                        assert(coef>1);
                        os << '+' << coef << '*';
                    }
                    else
                    {
                        assert(coef< -1);
                        os << coef << '*';
                    }

            }

            os << '[' << a.species.name << ']';
            return os;
        }

    }

}

namespace upsylon {

    namespace Aqua
    {
        Constraint:: Constraint(const double v) throw() :
        value(v),
        actors()
        {
        }

        Constraint:: ~Constraint() throw()
        {
        }

        void Constraint:: add(const Species &sp, const int weight)
        {

            if(weight!=0)
            {
                for(Actor *actor=actors.head;actor;actor=actor->next)
                {
                    if(&(actor->species) == &sp)
                    {
                        aliasing::_(actor->weight) += weight;
                        if(actor->weight==0)
                        {
                            delete actors.unlink(actor);
                        }
                    }
                }
                actors.push_back( new Actor(sp,weight) );
            }
        }

        std::ostream & operator<<(std::ostream &os, const Constraint &cc)
        {
            os << cc.value << '=';
            //const size_t nc = cc.actors.size;
            size_t       i  = 0;
            for(const Constraint::Actor *a=cc.actors.head;a;a=a->next,++i)
            {
                os << *a;
            }
            return os;
        }

    }

}

namespace upsylon {

    namespace Aqua
    {
        Boot:: Boot() throw() :
        Constraint::List(),
        R(),
        F(),
        d(0),
        S(),
        tS(),
        keep()
        {
            keep << aliasing::_(R);
            keep << aliasing::_(F);
            keep << aliasing::_(S);
            keep << aliasing::_(tS);
        }

        Boot:: ~Boot() throw()
        {
        }

        void Boot:: quit() throw()
        {
            keep.release_all();
            aliasing::_(d) = 0;
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

#include "y/aqua/library.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/mkl/kernel/gram-schmidt.hpp"
#include "y/mkl/kernel/adjoint.hpp"
#include "y/mkl/kernel/eigen.hpp"
#include "y/exception.hpp"

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

        void  Boot:: init(Library &lib)
        {
            static const char fn[] = "Aqua::Boot::init: ";
            quit();
            try
            {
                //--------------------------------------------------------------
                // parameters for P
                //--------------------------------------------------------------
                const size_t M  = lib.entries();
                if(M<=0)
                {
                    return;
                }
                const size_t Nc = size;
                if(Nc<=0)
                {
                    return;
                }
                aliasing::_(R).make(Nc,M);
                lib.buildIndices();
                {
                    size_t i=1;
                    for(const Constraint *cc=head;cc;cc=cc->next,++i)
                    {
                        cc->fill( aliasing::_(R[i]) );
                    }
                }

                //--------------------------------------------------------------
                // check P whilst building F
                //--------------------------------------------------------------
                aliasing::_(F).make(M,Nc);
                Matrix Ps(M,M);
                {
                    iMatrix tR(R,matrix_transpose);
                    iMatrix R2(Nc,Nc);
                    quark::mmul(R2,R,tR);
                    aliasing::_(d) = ideterminant(R2);
                    if(!d)
                    {
                        throw exception("%ssingular set of constraints",fn);
                    }
                    iMatrix aR2(Nc,Nc);
                    iadjoint(aR2,R2);
                    quark::mmul( aliasing::_(F),tR,aR2);
                    quark::mmul(Ps,F,R);
                    for(size_t i=M;i>0;--i)
                    {
                        for(size_t j=M;j>i;--j)
                        {
                            Ps[i][j] = -Ps[i][j];
                        }
                        Ps[i][i] = d-Ps[i][i];
                        for(size_t j=i-1;j>0;--j)
                        {
                            Ps[i][j] = -Ps[i][j];
                        }
                    }
                    std::cerr << "Ps=" << Ps << std::endl;
                    vector<double> dd(M,0);
                    Matrix         V(M,M);
                    if( eigen::build(Ps,dd,V) )
                    {
                        eigen::eigsrtA(dd,V);
                        std::cerr << "dd=" << dd << std::endl;
                        std::cerr << "V =" << V  << std::endl;
                    }
                    
                }




                if(Nc<M)
                {
                    const size_t N = M-Nc;
                    aliasing::_(S).make(N,M);
                    aliasing::_(tS).make(M,N);

                    iMatrix                   I(M,M);
                    combination               comb(M,N);
                    const accessible<size_t> &indx = comb;
                    for(comb.boot();comb.good();comb.next())
                    {
                        assert(indx.size()==N);
                        for(size_t i=Nc;i>0;--i)
                        {
                            quark::set(I[i],R[i]);
                        }
                        for(size_t i=1;i<=N;++i)
                        {
                            addressable<Int> &row = I[i+Nc];
                            quark::ld(row,0);
                            row[ indx[i] ] = 1;
                        }
                        if(GramSchmidt::iOrtho(I))
                        {
                            goto FINALIZE;
                        }
                    }
                    throw exception("%sunable to find orthogonal space!!!",fn);
                FINALIZE:
                    for(size_t i=N;i>0;--i)
                    {
                        quark::set( aliasing::_(S[i]), I[i+Nc]);
                    }
                    aliasing::_(tS).assign_transpose(S);
                    
                }

             }
            catch(...)
            {
                quit();
                throw;
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

