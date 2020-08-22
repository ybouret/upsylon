#include "y/code/decomp.hpp"
#include "y/utest/run.hpp"
#include "y/yap/rational.hpp"
#include "y/type/spec.hpp"

using namespace upsylon;

namespace {

    template <typename T>
    static inline
    void doDecomp()
    {
        std::cerr << "using <" << type_name_of<T>() << ">" << std::endl;
        typename decompose<T>::plist l;
        for(T n=1;n<=20;++n)
        {
            std::cerr << "\tdecomp(" << n << ")\t";
            decompose<T>::build(l,n);
            for(typename decompose<T>::pnode *node=l.head;node;node=node->next)
            {
                std::cerr << " " << node->x << "*" << node->y << ":" << node->d;
            }
            decompose<T>::clear(l);
            std::cerr << " => ";
            for(typename decompose<T>::pnode *node=l.head;node;node=node->next)
            {
                std::cerr << " " << node->x << "*" << node->y << ":" << node->d;
            }
            T x,y;
            decompose<T>::square(x,y,n);
            std::cerr << " => " << x << "," << y;
            std::cerr << std::endl;
        }
    }
}

Y_UTEST(decomp)
{
    doDecomp<int16_t>();
    doDecomp<int>();
    doDecomp<uint64_t>();
    doDecomp<apn>();
    doDecomp<apz>();
    doDecomp<apq>();
}
Y_UTEST_DONE()

