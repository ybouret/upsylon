//! \file
#ifndef Y_IOS_CODEC_INCLUDED
#define Y_IOS_CODEC_INCLUDED 1

#include "y/ios/istream.hpp"
#include "y/ios/ostream.hpp"
#include "y/sequence/list.hpp"

namespace upsylon
{
    namespace ios
    {
        //! CoderDecoder
        class codec : public istream, public ostream
        {
        public:
            //! destructor
            virtual ~codec() throw();

            //! reset codec
            virtual void reset() throw() = 0;

            //! read/process/write until end of input
            void filter( ios::ostream &output, ios::istream &input );

            //! utility : reset/write/read
            string to_string( const void *data, const size_t size );

            //! utility to encode a buffer
            inline string to_string( const memory::ro_buffer &buff )
            {
                return to_string( buff.ro(), buff.length() );
            }

        protected:
            //! initialize
            explicit codec() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(codec);
        };


        //! implements the istream part through a char list
        class q_codec : public codec
        {
        public:
            virtual ~q_codec() throw();

            virtual bool query(char &C);
            virtual void store(const char C);

        protected:
            //! initialize
            explicit   q_codec() throw();
            list<char> Q; //!< local buffer for istream

        private:
            Y_DISABLE_COPY_AND_ASSIGN(q_codec);
        };
    }
}

#endif

