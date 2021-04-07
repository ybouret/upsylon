//! \file

#ifndef Y_IOS_ECHOSTREAM_INCLUDED
#define Y_IOS_ECHOSTREAM_INCLUDED 1

#include "y/ios/ostream.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{

    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! echo to multiple streams
        //
        //______________________________________________________________________
        class echostream : public ostream
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit echostream();
            virtual ~echostream() throw();

            //__________________________________________________________________
            //
            // ostream interface
            //__________________________________________________________________
            virtual void write(char); //!< write to all streams
            virtual void flush();     //!< flush all streams

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void         clear() throw();                //!< clean list of stream
            echostream & link(const arc_ptr<ostream> &); //!< link an existing stream
            echostream & link(ostream                *); //!< link a new stream


        private:
            Y_DISABLE_COPY_AND_ASSIGN(echostream);
            void *impl;
        };

    }

}

#endif

