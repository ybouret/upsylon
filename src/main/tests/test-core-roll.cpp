
#include "y/core/roll.hpp"
#include "y/utest/run.hpp"
#include "y/object.hpp"

using namespace upsylon;

namespace {

    class dummy : public object
    {
    public:
        static int counter;

        const int value;
        dummy    *next;

        inline dummy() : value(++counter), next(0)
        {
            //std::cerr << "[+dummy#" << value << "]" << std::endl;
        }

        inline virtual ~dummy() throw()
        {
           // std::cerr << "[-dummy#" << value << "]" << std::endl;
        }


    private:

    };

    int dummy::counter = 0;

}

Y_UTEST(core_roll)
{
    {
        core::roll_of<dummy> D;
        std::cerr << '[';
        for(size_t i=16;i>0;--i)
        {
            dummy *d = new dummy;
            if(alea.choice())
            {
                D.push_back(d);
                std::cerr << '>';
            }
            else
            {
                D.push_front(d);
                std::cerr << '<';
            }
            std::cerr << d->value;
            Y_ASSERT(D.owns(d));
        }
        std::cerr << ']' << std::endl;

        std::cerr << '[';
        for(const dummy *d=D.head;d;d=d->next)
        {
            std::cerr << ':' << d->value;
        }
        std::cerr << ']' << std::endl;
        D.reverse();

        std::cerr << '[';
        for(const dummy *d=D.head;d;d=d->next)
        {
            std::cerr << ':' << d->value;
        }
        std::cerr << ']' << std::endl;
        std::cerr << '[';
        while(D.size)
        {
            dummy *d = D.pop_front();
            std::cerr << '~' << d->value;
            delete d;
        }
        std::cerr << ']' << std::endl;
    }
}
Y_UTEST_DONE()

