
#include "y/ink/ops/crypt.hpp"
#include "y/utest/run.hpp"
#include "y/ink/image.hpp"
#include "y/ink/ops/filter.hpp"

#include "y/crypto/sc/arc4.hpp"
#include "y/crypto/bc/aes.hpp"
#include "y/crypto/bc/ecb.hpp"

#include "y/os/uuid.hpp"

using namespace upsylon;
using namespace Ink;

Y_UTEST(kr)
{
    ImageIO   &img   = Image::Init();
    //Dispatcher par   = new concurrent::simd();

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
        img.save("enc_sc.png",enc,NULL);

        ks.schedule(key);
        Crypt::Run(dec,enc,ks);
        img.save("dec_sc.png",dec,NULL);


        enc.zset();
        {
            crypto::ciphers::pointer cph = crypto::aes128::create(key);
            crypto::ecb::encrypter   ECB_ENC;
            Crypt::Run(enc,ini, ECB_ENC,*cph);
            img.save("enc_ecb.png",enc,NULL);

        }

        dec.zset();
        {
            crypto::ciphers::pointer cph = crypto::aes128::create(key);
            crypto::ecb::decrypter   ECB_DEC;
            Crypt::Run(dec,enc, ECB_DEC,*cph);
            img.save("dec_ecb.png",dec,NULL);
        }


    }

}
Y_UTEST_DONE()

