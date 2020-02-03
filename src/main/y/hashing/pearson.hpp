//! \file
#ifndef Y_HASHING_PEARSON_INCLUDED
#define Y_HASHING_PEARSON_INCLUDED 1

#include "y/randomized/bits.hpp"
#include "y/hashing/function.hpp"

namespace upsylon {

    namespace hashing
    {

        //! pearson hashing
        class pearson
        {
        public:
            
            explicit pearson(randomized::bits *salt = 0) throw();
            virtual ~pearson() throw();

            const uint8_t table[256];
            void          reset(randomized::bits *salt=0) throw();

            uint8_t next(const uint8_t h, const uint8_t c) const throw();

            void initialize(void *addr,const size_t size) const throw();
            void update(void *addr,const size_t size, const uint8_t c) const throw();

            template <typename T> inline void initialize(T &arg)              const throw() { initialize(&arg,sizeof(T)); }
            template <typename T> inline void update(T &arg, const uint8_t c) const throw() { update(&arg,sizeof(T),c);   }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pearson);
        };

    }

}


#endif

