
#include "y/ink/ops/crypt.hpp"
#include "y/utest/run.hpp"
#include "y/ink/image.hpp"
#include "y/ink/ops/filter.hpp"

#include "y/crypto/sc/arc4.hpp"
#include "y/os/uuid.hpp"

using namespace upsylon;
using namespace Ink;

Y_UTEST(kr)
{
    ImageIO   &img   = Image::Init();
   // Dispatcher par   = new concurrent::simd();

    for(int iarg=1;iarg<argc;++iarg)
    {
        const string   fn  = argv[iarg];
        const Pixmap3  ini = img.load3(fn,NULL);
        Pixmap3        enc(ini.w,ini.h);
        Pixmap3        dec(ini.w,ini.h);

        img.save("ini.png",ini,NULL);

        const uuid   key;
        crypto::arc4 ks(key);
        Crypt::Run(enc,ini,ks);
        img.save("enc.png",enc,NULL);

        ks.schedule(key);
        Crypt::Run(dec,enc,ks);
        img.save("dec.png",dec,NULL);



    }

}
Y_UTEST_DONE()

