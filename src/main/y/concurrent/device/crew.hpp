
//! \file

#ifndef Y_CONCURRENT_CREW_INCLUDED
#define Y_CONCURRENT_CREW_INCLUDED 1

#include "y/concurrent/worker.hpp"
#include "y/concurrent/sync/condition.hpp"
#include "y/concurrent/topology.hpp"
#include "y/concurrent/device/engine.hpp"

#include "y/ptr/auto.hpp"
#include "y/sequence/slots.hpp"

//______________________________________________________________________________
//
//
//! environment variable to trigger verbosity
//
//______________________________________________________________________________
#define Y_VERBOSE_THREADS "Y_VERBOSE_THREADS"

namespace upsylon
{
    namespace concurrent
    {
        namespace nucleus
        {
            //__________________________________________________________________
            //
            //
            //! base class to build const topology before workers
            //
            //__________________________________________________________________
            class crew
            {
            public:
                //______________________________________________________________
                //
                // member
                //______________________________________________________________
                const auto_ptr<const topology> topo; //!< built topology

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~crew() throw(); //!< cleanup

            protected:
                explicit crew();                   //!< default
                explicit crew(const size_t start,
                              const size_t width,
                              const size_t every); //!< overrides

            private:
                Y_DISABLE_COPY_AND_ASSIGN(crew);
            };
        }


        //______________________________________________________________________
        //
        //
        //! base class to build const topology before workers
        //
        //______________________________________________________________________
#define Y_CREW_PRINTLN(MSG) do { if(verbose) { std::cerr << MSG << std::endl; } } while(false)

        //______________________________________________________________________
        //
        //
        //! low level crew of "one time" workers
        //
        //______________________________________________________________________
        class crew : public engine, public nucleus::crew
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit crew();         //!< create all threads an wait on start
            virtual ~crew() throw(); //!< broacast all threads and wait on stall

            //__________________________________________________________________
            //
            // engine
            //__________________________________________________________________
            virtual lockable &     sync()       throw();    //!< synchronize
            virtual void           once(executable,void*);  //!< send on squad

            //__________________________________________________________________
            //
            // accessible<context>
            //__________________________________________________________________
            virtual size_t         size() const throw();                   //!< topo->size() == squad.size()
            virtual const context &operator[](const size_t) const throw(); //!< 1..size
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static bool query_threads_verbosity();  //!< from Y_VERBOSE_THREADS

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            mutex synchronize; //!< mutex for all internal operations


        private:
            Y_DISABLE_COPY_AND_ASSIGN(crew);
            slots<worker> squad; //!< threads
            size_t        ready; //!< num ready
            condition     start; //!< waiting to start
            condition     stall; //!< waiting to end
            size_t        yoked; //!< joined after run
            executable    kcode; //!< kernel code
            void         *kdata; //!< kernel data
            
            void        on();                 //!< setup all waiting
            void        off() throw();        //!< waiting yoked>ready

            static void entry_stub(void *) throw(); //!< call entry()
            void        entry() throw();            //!< threaded function
            
        public:
            bool  verbose; //!< verbosity, mostly to debug
            
        };
    }
    
}

#endif

