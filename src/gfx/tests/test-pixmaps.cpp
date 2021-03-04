
#include "y/gfx/pixmaps.hpp"
#include "y/gfx/bitrow.hpp"
#include "y/utest/run.hpp"
#include "y/type/spec.hpp"
#include "support.hpp"

using namespace upsylon;
using namespace graphic;

template <>
inline rgb support:: get<rgb>()
{
    return rgb( alea.full<uint8_t>(), alea.full<uint8_t>(), alea.full<uint8_t>() );
}

template <>
inline rgba support:: get<rgba>()
{
    return rgb( alea.full<uint8_t>(), alea.full<uint8_t>(), alea.full<uint8_t>() );
}

template <>
inline YUV support:: get<YUV>()
{
    return YUV( alea.to<float>(), alea.range(YUV::Umin,YUV::Umax), alea.range(YUV::Vmin,YUV::Vmax) );
}


namespace
{
    template <typename T>
    static inline void do_pixmap()
    {
        std::cerr << "pixmap<" << type_name_of<T>() << ">" << std::endl;
        Y_CHECK(sizeof(bitrow)==sizeof(pixrow<T>));

        pixmap<T>       pxm( 1+alea.leq(1000), 1+alea.leq(1000) );
        const pixmap<T> cpy = pxm;
        std::cerr << pxm << std::endl;
        std::cerr << cpy << std::endl;

        for(unit_t j=0;j<pxm.h;++j)
        {
            for(unit_t i=0;i<pxm.w;++i)
            {
                const T tmp = support::get<T>();
                pxm[j][i] = tmp;
                const T &src = cpy[j][i];
                Y_ASSERT( 0== memcmp(&tmp,&src,sizeof(T)) );
                Y_ASSERT( &src == pxm.at(i,j) );

            }
        }



        std::cerr << std::endl;
    }
}

Y_UTEST(pixmaps)
{
    do_pixmap<uint8_t>();
    do_pixmap<float>();
    do_pixmap<rgb>();
    do_pixmap<rgba>();
    do_pixmap<YUV>();

}
Y_UTEST_DONE()

