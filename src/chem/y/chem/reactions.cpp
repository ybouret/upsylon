#include "y/chem/reactions.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Chemical
    {
        Reactions:: ~Reactions() throw()
        {
        }

        Reactions:: Reactions(const size_t n) :
        ReactionsType(n,as_capacity),
        N(0),
        M(0),
        Nu(),
        Phi()
        {
        }

        Reaction & Reactions:: enroll( Reaction *rxn )
        {
            assert(rxn);
            Reaction::Pointer p(rxn);
            if( !insert(p) )
            {
                throw exception("multiple reaction '%s'",*(p->name));
            }
            return *p;
        }

        size_t Reactions:: max_name_length() const throw()
        {
            size_t ans = 0;
            for(const_iterator i=begin();i!=end();++i)
            {
                const size_t tmp = (*i)->name.size();
                if(tmp>ans)
                {
                    ans = tmp;
                }
            }
            return ans;
        }

        std::ostream & operator<<( std::ostream &os, const Reactions &cs )
        {
            const size_t sz = cs.max_name_length();
            const size_t num = cs.size();
            size_t       idx = 1;
            for(Reactions::const_iterator i=cs.begin();i!=cs.end();++i,++idx)
            {
                os << " (*)";
                for(size_t j=(*i)->name.size();j<=sz;++j) os << ' ';
                os << (**i);
                if(idx<num) os << std::endl;
            }
            return os;
        }

        void Reactions:: release_all() throw()
        {
            (size_t &)N = 0;
            (size_t &)M = 0;
            rxn.free();
        }

        void Reactions:: compile_for( Library &lib )
        {

            lib.update();
            for(iterator i=begin();i!=end();++i)
            {
                (*i)->validate();
            }
            
            try
            {
                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________
                release_all();
                size_t &n = (size_t &)N;
                size_t &m = (size_t &)M;
                n = this->size();
                m = lib.size();
                if(M<=0)
                {
                    if(N>0)
                    {
                        throw exception("unexpected %u reactions with no species!", unsigned(N));
                    }
                    return;
                }

                assert(M>0);
                active.make(M,false);

                if(N>0)
                {
                    rxn.free();
                    rxn.ensure(N);
                    Nu.    make(N,M);
                    Phi.   make(N,M);
                    K.     make(N,0);
                    Gamma. make(N,0);
                    for(iterator i=begin();i!=end();++i)
                    {
                        rxn.push_back(*i);
                    }
                }


            }
            catch(...)
            {
                release_all();
                throw;
            }
        }

        void Reactions:: computeK(const double t)
        {
            for(size_t i=N;i>0;--i)
            {
                K[i] = (*rxn[i])(t);
            }
        }

        void Reactions:: computeGamma(const array<double> &C)
        {
            for(size_t i=N;i>0;--i)
            {
                Gamma[i] = rxn[i]->Gamma(K[i],C);
            }
        }

    }

}
