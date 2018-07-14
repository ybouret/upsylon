//! \file
#ifndef Y_RANDOMIZED_ISAAC_INCLUDED
#define Y_RANDOMIZED_ISAAC_INCLUDED 1

#include "y/randomized/bits.hpp"
#include "y/memory/buffer.hpp"

namespace upsylon
{

	namespace randomized
	{

        //! how to initialize
		enum ISAAC_INIT
		{
			ISAAC_INIT_NULL, //!< init with null space
			ISAAC_INIT_AUTO, //!< init with isaac auto procedure
			ISAAC_INIT_RAND  //!< init with time based seed
		};

        //! implementation
		template <const size_t ALPHA>
		class isaac {
		public:
			static const size_t SIZE = ( 1<<ALPHA ); //!< internal size
			
			virtual ~isaac() throw();
			explicit isaac( ISAAC_INIT how = ISAAC_INIT_NULL ) throw(); //!< default init, with null workspace
			explicit isaac( const memory::ro_buffer &key ) throw();     //!< fill and init
			explicit isaac( const uint32_t wksp[], const size_t wlen ) throw(); //!< fill and init
			
			uint32_t         &rsl( const size_t index ) throw();       //!< manual access, 0 - SIZE-1
			const uint32_t   &rsl( const size_t index ) const throw(); //!< manual access, 0 - SIZE-1
			
			void              ini(bool flag) throw(); //!< call randinit
			void              run() throw();          //!< call isaac


            uint32_t rand32() throw(); //!< generate a new 32 bit random number
			void     reset( const memory::ro_buffer &key ) throw();  //!< fill and init
			
		private:
			Y_DISABLE_COPY_AND_ASSIGN(isaac);
			struct randctx
			{
				uint32_t randcnt;
				uint32_t randrsl[(1<<ALPHA)];
				uint32_t randmem[(1<<ALPHA)];
				uint32_t randa;
				uint32_t randb;
				uint32_t randc;
			};
			randctx ctx_;
			
			static void     randinit( randctx *ctx, bool flag ) throw();  //!< initialize
			static void     call(randctx *ctx) throw();                   //!< perform isaac
			static uint32_t rand(randctx *ctx) throw();                   //!< generate a next 32 bits value

		};

        //! interface of ISAAC
        template <const size_t ALPHA>
        class ISAAC : public bits
        {
        public:
            //! constructor, default is use null space
            inline explicit ISAAC(ISAAC_INIT how = ISAAC_INIT_NULL) throw() :
            bits(0xffffffff),
            _isaac(how)
            {
            }

            //! desctructor
            inline virtual ~ISAAC() throw() {}

            //! bits interface : generate
            inline virtual uint32_t next32() throw() { return _isaac.rand32(); }

            //! bits interface : reseed
            inline virtual void     reseed( bits &bits ) throw()
            {
                for(size_t i=0;i<isaac<ALPHA>::SIZE;++i)
                {
                    _isaac.rsl(i) = bits.next32();
                }
                _isaac.ini(true);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ISAAC);
            isaac<ALPHA> _isaac;
        };
		
        
	}
	
}

#endif
