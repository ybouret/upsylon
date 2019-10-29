#include "y/math/show.hpp"

namespace upsylon
{
    namespace math
    {

#define SHOW(T,NAME) std::cerr << "numeric<" #T ">::" #NAME "=" << numeric<T>::NAME << std::endl

#define SHOW_FOR(T) \
SHOW(T,minimum);    \
SHOW(T,maximum);    \
SHOW(T,mant_dig);   \
SHOW(T,epsilon);    \
SHOW(T,pi);         \
SHOW(T,two_pi);     \
SHOW(T,half_pi);    \
SHOW(T,min_exp);    \
SHOW(T,max_exp);    \
SHOW(T,dig);        \
SHOW(T,min_10_exp); \
SHOW(T,max_10_exp); \
SHOW(T,ftol);       \
SHOW(T,sqrt_ftol);  \
SHOW(T,tiny);       \
SHOW(T,huge);       \
SHOW(T,gold);\
SHOW(T,inv_gold)


        void show:: info()
        {
            SHOW_FOR(float);
            std::cerr << std::endl;
            SHOW_FOR(double);
        }
    }
}
