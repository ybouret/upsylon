#ifndef YOCTO_ZPIPE_INCLUDED
#define YOCTO_ZPIPE_INCLUDED 1

#include "y/ios/istream.hpp"
#include "y/ios/ostream.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/memory/zblock.hpp"

namespace upsylon
{

	namespace zlib
	{

		class zpipe
		{
		public:
			explicit zpipe( size_t chunk_size );
			virtual ~zpipe() throw();

			void def( ios::ostream &target, ios::istream &source, size_t level);
			void inf( ios::ostream &target, ios::istream &source);

			

		private:
			typedef zblock<char,memory::global> buff_t;
			const size_t          CHUNK;
			buff_t                wksp_;
			char                 *in;
			char                 *out;
			bool                  eof_;

			Y_DISABLE_COPY_AND_ASSIGN(zpipe);
			size_t _read( ios::istream &source );                   //!< fill  in, return>0 if more chars
			void   _save( ios::ostream &target, size_t len ) const; //!< write len bytes of out
		};
	}

}


#endif
