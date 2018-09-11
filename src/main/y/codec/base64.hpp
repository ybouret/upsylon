//! \file

#ifndef Y_CODEC_BASE64_INCLUDED
#define Y_CODEC_BASE64_INCLUDED

#include "y/ios/codec.hpp"

namespace upsylon
{
    namespace ios
    {
        //! Base64 I/O
        struct base64
        {

            static const char  encode_std[64]; //!< standard encoding
            static const char  encode_url[64]; //!< URL type encoding
            static const short decoded[256];   //!< inverse look up table
        private:
            static int find_in_table(const char *table, const char C) throw();
        public:
            inline static int find_in_std(const char C) throw() { return find_in_table(encode_std,C); } //!< manual inverse lookup
            inline static int find_in_url(const char C) throw() { return find_in_table(encode_url,C); } //!< manual inverse lookup

            //! binary -> base64
            class encoder : public q_codec
            {
            public:
                //! states
                enum status
                {
                    wait0, //!< waiting for byte0
                    wait1, //!< waiting for byte1
                    wait2  //!< waiting for byte2
                };

                //! initialize, with padding or not, and choose the encoding
                explicit encoder(bool use_pad=true, bool use_url=false) throw();

                //! destructor
                virtual ~encoder() throw();

                virtual void reset() throw(); //!< state and data
                virtual void write( char C ); //!< store data and output accordingly
                virtual void flush();         //!< check data and output accordingly
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(encoder);
                status      flag;     //!< where are we
                uint8_t     data[4];  //!< ready
                const char *table;
                const bool  pad;
                void     emit(const size_t n);
            };

            //! base64 -> decoder
            class decoder : public q_codec
            {
            public:
                explicit decoder() throw();
                virtual ~decoder() throw();

                virtual void reset() throw(); //! clear all data
                virtual void write( char C ); //!< check and store character
                virtual void flush();         //!< output depending on status

            private:
                Y_DISABLE_COPY_AND_ASSIGN(decoder);
                size_t  count;
                short   input[4];
                void emit();
            };

        };
    }
}

#endif


