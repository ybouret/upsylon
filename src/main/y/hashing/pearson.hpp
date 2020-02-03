//! \file
#ifndef Y_HASHING_PEARSON_INCLUDED
#define Y_HASHING_PEARSON_INCLUDED 1

#include "y/randomized/bits.hpp"
#include "y/hashing/function.hpp"

namespace upsylon {

    namespace hashing
    {

        //! pearson hashing
        class pearson_
        {
        public:
            
            explicit pearson_(randomized::bits *salt = 0) throw();
            virtual ~pearson_() throw();

            const uint8_t table[256];
            void          reset(randomized::bits *salt=0) throw();

            uint8_t next(const uint8_t h, const uint8_t c) const throw();

            void initialize(void *addr,const size_t size) const throw();
            void update(void *addr,const size_t size, const uint8_t c) const throw();

            template <typename T> inline void initialize(T &arg)              const throw() { initialize(&arg,sizeof(T)); }
            template <typename T> inline void update(T &arg, const uint8_t c) const throw() { update(&arg,sizeof(T),c);   }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pearson_);
        };


        template <typename T>
        class pearson : public function, public pearson_
        {
        public:
            inline explicit pearson() throw() : function( sizeof(T), sizeof(T) ), pearson_(), h(0) {}
            inline virtual ~pearson() throw() {}

            virtual void set() throw() { initialize(h); }
            virtual void run(const void *buffer, size_t buflen) throw() {
                assert( !(0==buffer&&buflen>0) );
                const uint8_t *p = static_cast<const uint8_t *>(buffer);
                while(buflen-- > 0) update(h,*(p++));
            }
            
            virtual void get(void *output, size_t outlen) throw()
            {
                fill(output,outlen, &h, sizeof(T) );
            }
            
            const char *name() const throw()
            {
                return "pearson";
            }

        private:
            T h;
            Y_DISABLE_COPY_AND_ASSIGN(pearson);
        };



    }

}


#endif

