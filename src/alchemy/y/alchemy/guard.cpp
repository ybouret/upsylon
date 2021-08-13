
#include "y/alchemy/guard.hpp"

namespace upsylon
{
    namespace Alchemy
    {

#define Y_ALCHEMY_GUARD(CLS) case CLS: return #CLS

        const char * Guard:: ClassText(const Class c) throw()
        {
            switch(c)
            {
                    Y_ALCHEMY_GUARD(HasNoBound);
                    Y_ALCHEMY_GUARD(HasOnlyLEQ);
                    Y_ALCHEMY_GUARD(HasOnlyGEQ);
                    Y_ALCHEMY_GUARD(IsBothWays);
            }
            return "???";
        }

        const char * Guard:: classText() const throw()
        {
            return ClassText(cls);
        }


        Guard:: ~Guard() throw()
        {
        }

        Guard:: Guard() throw() :
        leq(), geq(), cls(HasNoBound)
        {
        }

        Guard:: Array:: ~Array() throw()
        {
        }

        Guard::Array:: Array(size_t n) :
        Array_(n,as_capacity)
        {
            while(n-- > 0)
            {
                const Guard::Pointer p = new Guard();
                push_back_(p);
            }
        }

        void Guard:: addLEQ(const Leading &leading)
        {
            aliasing::_(leq).push_back(leading);
            update();
        }

        void Guard:: addGEQ(const Leading &leading)
        {
            aliasing::_(geq).push_back(leading);
            update();
        }

        void Guard:: update() throw()
        {
            if(leq.size()>0)
            {
                if(geq.size()>0)
                {
                    aliasing::_(cls) = IsBothWays;
                }
                else
                {
                    assert(geq.size()<=0);
                    aliasing::_(cls) = HasOnlyLEQ;
                }
            }
            else
            {
                if(geq.size()>0)
                {
                    assert(leq.size()<=0);
                    aliasing::_(cls) = HasOnlyGEQ;
                }
                else
                {
                    assert(geq.size()<=0);
                    assert(leq.size()<=0);
                    aliasing::_(cls) = HasNoBound;
                }
            }
        }

    }

}
