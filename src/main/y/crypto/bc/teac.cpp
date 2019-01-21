#include "y/crypto/bc/teac.hpp"
#include "y/hashing/md5.hpp"
#include "y/type/utils.hpp"
#include "y/code/round.hpp"
#include <cstdio>

namespace upsylon {

    namespace crypto {

        tea_cipher:: ~tea_cipher() throw() {
        }

        tea_cipher:: tea_cipher(  size_t block_size , const memory::ro_buffer &k ) :
        k128_( 16, 0 ),
        size_( Y_ROUND4( max_of<size_t>( 8, block_size ) ) ),
        nU32_( size_ / sizeof(uint32_t) ),
        rU32_( size_, 0 )
        {
            //-- build key
            hashing::md5 alg;
            alg(k);
            alg.get( k128_.rw(), k128_.length() );
        }

        size_t tea_cipher:: size() const throw() {
            return size_;
        }


    }

}
