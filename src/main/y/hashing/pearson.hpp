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
            
            explicit pearson_(randomized::bits *salt = 0) throw();         //!< initialize with salt
            virtual ~pearson_() throw();                                   //!< cleanup
            void     reset(randomized::bits *salt=0) throw();              //!< reset with salt
            uint8_t  next(const uint8_t h, const uint8_t c) const throw(); //!< table prediciton

            void initialize(void *addr,const size_t size) const throw();              //!< initialize bytes field
            void update(void *addr,const size_t size, const uint8_t c) const throw(); //!< update bytes field

            //! initialize key
            template <typename T> inline void initialize(T &arg)              const throw() { initialize(&arg,sizeof(T)); }

            //! update key
            template <typename T> inline void update(T &arg, const uint8_t c) const throw() { update(&arg,sizeof(T),c);   }

            const uint8_t table[256];  //!< interbal table

        private:
            Y_DISABLE_COPY_AND_ASSIGN(pearson_);
        };


        //! pearson hashing for a given key type
        template <typename T>
        class pearson : public function, public pearson_
        {
        public:
            //! setup
            inline explicit pearson() throw() : function( sizeof(T), sizeof(T) ), pearson_(), h(0) {}

            //! cleanup
            inline virtual ~pearson() throw() {}

            //! function interface: set
            virtual void set() throw() { initialize(h); }

            //! function interface: run
            virtual void run(const void *buffer, size_t buflen) throw() {
                assert( !(0==buffer&&buflen>0) );
                const uint8_t *p = static_cast<const uint8_t *>(buffer);
                while(buflen-- > 0) update(h,*(p++));
            }

            //! function interface: get
            virtual void get(void *output, size_t outlen) throw()
            {
                fill(output,outlen, &h, sizeof(T) );
            }

            //! function interface: name
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

