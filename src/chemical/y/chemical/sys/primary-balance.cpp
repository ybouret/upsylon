#include "y/chemical/system.hpp"
#include "y/mkl/tao.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {
        
        bool System:: balancePrimary(Addressable &C) throw()
        {
            static const size_t from=2;
            static const size_t curr=from+2;
            static const size_t next=curr+2;

            //------------------------------------------------------------------
            //
            // enter info
            //
            //------------------------------------------------------------------
            if(Verbosity)
            {
                Library::Indent(std::cerr,from) << "<Balance Primary>" << std::endl;
                lib.display(std::cerr,C,curr)  << std::endl;
                showPrimary(std::cerr,C,curr);
                Library::Indent(std::cerr,curr) << "<Solve Primary>" << std::endl;
            }

            //------------------------------------------------------------------
            //
            // collect extents, and species to set to zero
            //
            //------------------------------------------------------------------
            bool  result = true;
            who.free();
            for(size_t i=N;i>0;--i)
            {
                const Actor *pA = 0;
                if( false == (ok[i]=primary[i]->search(xi[i],pA,C)) )
                {
                    assert(0==pA);
                    result = false;
                }
                else
                {
                    if(pA) who.push_back_( (Actor *)pA );
                }
            }

            //------------------------------------------------------------------
            //
            // search info
            //
            //------------------------------------------------------------------
            if(Verbosity)
            {
                for(size_t i=1;i<=N;++i)
                {
                    Library::Indent(std::cerr,next) << '@' << **primary[i] << " : ";
                    if(ok[i])
                    {
                        std::cerr << xi[i];
                    }
                    else
                    {
                        std::cerr << "N/A";
                    }
                    std::cerr << std::endl;
                }
                Library::Indent(std::cerr,next) << "vanishing = {";
                const size_t nwho = who.size();
                for(size_t j=1;j<=nwho;++j)
                {
                    std::cerr << ' ' << (*who[j]).sp;

                }
                std::cerr << " }" << std::endl;
            }

            //------------------------------------------------------------------
            //
            // if at least one need to be moved => who is not empty
            //
            //------------------------------------------------------------------
            if(who.size())
            {
                tao::mul_add(C,NuT,xi);
                for(size_t i=who.size();i>0;--i)
                {
                    const Actor &a = *who[i];
                    C[a.sp.indx]   = 0;
                }
                for(size_t i=N;i>0;--i)
                {
                    if(ok[i]) primary[i]->ensurePositive(C);
                }
            }

            //------------------------------------------------------------------
            //
            // leave info
            //
            //------------------------------------------------------------------
            if(Verbosity)
            {
                Library::Indent(std::cerr,curr) << "<Solve Primary/>" << std::endl;
                lib.display(std::cerr,C, next)  << std::endl;
                eqs.display(std::cerr,ok,next)  << std::endl;
                Library::Indent(std::cerr,curr) << " ==> " << Outcome(result) << " <==" << std::endl;
                Library::Indent(std::cerr,from) << "<Balance Primary/>" << std::endl;
            }
            return result;
        }
        
        
    }
}
