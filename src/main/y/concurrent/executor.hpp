//! \file
#ifndef Y_CONCURRENT_EXECUTOR_INCLUDED
#define Y_CONCURRENT_EXECUTOR_INCLUDED 1

#include "y/parallel.hpp"
#include "y/concurrent/fake-lock.hpp"

namespace upsylon
{
    namespace concurrent
    {
        //! a function that may be run in parallel
        typedef void (*kernel)(void *, parallel &, lockable &);

        //! interface to run kernels
        class executor
        {
        public:
            //__________________________________________________________________
            //
            //
            // virtual interface
            //
            //__________________________________________________________________

            //! destructor
            virtual  ~executor() throw();

            //! run a kernel with data, presenting a context and a shared access
            virtual void       run(kernel, void *) = 0;

            //! number of underlying threads
            virtual size_t     num_threads()   const throw() = 0;

            //! access to individual context in [0..num_threads-1]
            virtual parallel & get_context(const size_t indx) throw() = 0;

            //__________________________________________________________________
            //
            //
            // non-virtual interface
            //
            //__________________________________________________________________

            //__________________________________________________________________
            //
            // access to context
            //__________________________________________________________________


            //! access to individual context
            parallel & operator[](const size_t) throw();

            //! access to individual context, CONST
            const parallel & operator[](const size_t) const throw();


            //__________________________________________________________________
            //
            // memory management
            //__________________________________________________________________

            //! free per engine
            void free() throw();

            //! release per engine
            void release() throw();

            //! reserve space for T for each engine
            template <typename T>
            void ready(const memory::storage::model which)
            {
                executor     &self = *this;
                const size_t  nthr = num_threads();
                for(size_t i=0;i<nthr;++i)
                {
                    self[i].ready<T>(which);
                }
            }

            //! reserve space for n times T for each engine
            template <typename T>
            void ready(const memory::storage::model which, const size_t n)
            {
                executor     &self = *this;
                const size_t  nthr = num_threads();
                for(size_t i=0;i<nthr;++i)
                {
                    self[i].ready<T>(which,n);
                }
            }

            //! build one T for each engine
            template <typename T>
            void shape(const memory::storage::model which)
            {
                executor     &self = *this;
                const size_t  nthr = num_threads();
                for(size_t i=0;i<nthr;++i)
                {
                    self[i].shape<T>(which);
                }
            }

            //! build n times  T for each engine
            template <typename T>
            void shape(const memory::storage::model which, const size_t n)
            {
                executor     &self = *this;
                const size_t  nthr = num_threads();
                for(size_t i=0;i<nthr;++i)
                {
                    self[i].shape<T>(which,n);
                }
            }

            //! build one T for each engine from parameter
            template <typename T, typename U>
            void build(const memory::storage::model which, const typename type_traits<U>::parameter_type argU)
            {
                executor     &self = *this;
                const size_t  nthr = num_threads();
                for(size_t i=0;i<nthr;++i)
                {
                    self[i].build<T,U>(which,argU);
                }
            }

            //! build one T for each engine from parameter, n times
            template <typename T, typename U>
            void build(const memory::storage::model which, const size_t n, const typename type_traits<U>::parameter_type argU)
            {
                executor     &self = *this;
                const size_t  nthr = num_threads();
                for(size_t i=0;i<nthr;++i)
                {
                    self[i].build<T,U>(which,n,argU);
                }
            }

            //! sum of local stored type
            /**
             assuming first item in cache is T
             */
            template <typename T> inline
            T sum() const
            {
                const executor &self = *this;
                T               ans(0);
                size_t          i    = num_threads();
                while(i-- > 0)
                {
                    const memory::groove &g = *self[i];
                    ans += g.get<T>();
                }
                return ans;
            }
            
        protected:
            //! constructor
            explicit  executor() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(executor);
        };

        //! implements a sequential exectuor
        class sequential : public executor
        {
        public:
            //! constructor
            inline explicit sequential() throw() :
            executor(),context(),access() {}

            //! destructor
            inline virtual ~sequential() throw() {}

            //! direct call
            inline virtual void run( kernel proc, void *data )
            {
                assert(proc);
                proc(data,context,access);
            }

            //! only one
            inline virtual size_t num_threads() const throw() { return 1; }

            //! context
            inline virtual parallel & get_context(const size_t) throw() { return context; }
            
        private:
            parallel  context;
            
        public:
            fake_lock access; //!< shared access

        };

    }
}

#endif

