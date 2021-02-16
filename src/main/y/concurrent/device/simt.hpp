


//! \file

#ifndef Y_CONCURRENT_SIMT_INCLUDED
#define Y_CONCURRENT_SIMT_INCLUDED 1

#include "y/concurrent/device/looper.hpp"
#include "y/concurrent/device/crew.hpp"

namespace upsylon {

    namespace concurrent
    {

#define Y_VERBOSE_SIMT "Y_VERBOSE_SIMT"

        //______________________________________________________________________
        //
        //
        //! single instruction multiple threads
        //
        //______________________________________________________________________
        class simt : public looper
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit simt();          //!< create crew, prepare loop
            virtual ~simt() throw();  //!< cleanup

            virtual void loop(executable code,
                              void      *args);
            virtual void join() throw();

            static bool query_simt_verbosity();  //!< from Y_VERBOSE_SIMT


        private:
            Y_DISABLE_COPY_AND_ASSIGN(simt);
            virtual const engine & bulk() const throw();

            crew         self;   //!< the crew
            mutex       &access; //!< crew.synchronize
            const size_t number; //!< crew size

            size_t       online; //!< num ready to loop
            condition    course; //!< waiting to course

            size_t       joined; //!< for barrier
            condition    finish; //!< for barrier






            executable kcode;
            void      *kargs;

            static void loop_stub(void *, const context &, lockable &) throw();
            void        loop_code(const context &) throw();

        public:
            bool verbose;
        };

    }
}

#endif
