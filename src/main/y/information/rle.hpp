#ifndef Y_INFORMATION_RLE_INCLUDED
#define Y_INFORMATION_RLE_INCLUDED 1

#include "y/information/filter/queue.hpp"

namespace upsylon {

    namespace information {

        namespace RLE {


            class Context : public filterQ
            {
            public:
                virtual ~Context() throw();

            protected:
                explicit Context() throw();

                int last;

                void restart() throw(); //!< last=-1, free()


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Context);
            };


            class Encoder : public Context
            {
            public:
                explicit Encoder() throw();
                virtual ~Encoder() throw();

                virtual void reset() throw();
                virtual void write(char);
                virtual void flush();

            private:
                enum Flag
                {
                    waiting,
                    counting,
                };
                Flag   flag;
                size_t count;

                Y_DISABLE_COPY_AND_ASSIGN(Encoder);
            };

            class Decoder : public Context
            {
            public:
                explicit Decoder() throw();
                virtual ~Decoder() throw();

                virtual void reset() throw();
                virtual void write(char);
                virtual void flush();

            private:


                Y_DISABLE_COPY_AND_ASSIGN(Decoder);
            };


        }

    }

}

#endif

