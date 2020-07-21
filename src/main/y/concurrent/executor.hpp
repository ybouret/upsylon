//! \file
#ifndef Y_CONCURRENT_EXECUTOR_INCLUDED
#define Y_CONCURRENT_EXECUTOR_INCLUDED 1

#include "y/parallel.hpp"
#include "y/concurrent/fake-lock.hpp"

namespace upsylon
{
    namespace concurrent
    {
        //__________________________________________________________________
        //
        //
        //! a function that may be run in parallel
        //
        //__________________________________________________________________
        typedef void (*kernel)(void *, parallel &, lockable &);

        //__________________________________________________________________
        //
        //
        //! interface to run kernels
        //
        //__________________________________________________________________
        class executor
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef parallel::cache_type cache_type; //!< alias

            //__________________________________________________________________
            //
            //
            // virtual interface
            //
            //__________________________________________________________________
            //! destructor
            virtual           ~executor() throw();

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
            // access to context(s)
            //__________________________________________________________________

            //! access to individual context
            parallel & operator[](const size_t) throw();

            //! access to individual context, CONST
            const parallel & operator[](const size_t) const throw();

            //__________________________________________________________________
            //
            // access to cache(s)
            //__________________________________________________________________

            //! access to individual cache
            cache_type & operator()(const size_t) throw();

            //! access to individual cache, CONST
            const cache_type & operator()(const size_t) const throw();

            //__________________________________________________________________
            //
            // cleaning memory management
            //__________________________________________________________________

            //! free per engine
            void free() throw();

            //! release per engine
            void release() throw();

            //__________________________________________________________________
            //
            // acquire flat memory
            //__________________________________________________________________

            //! acquire space for T for each engine
            template <typename T>
            void acquire(const memory::storage::model which)
            {
                executor     &self = *this;
                const size_t  nthr = num_threads();
                for(size_t i=0;i<nthr;++i)
                {
                    self(i).acquire(which, sizeof(T) );
                }
            }

            //! reserve space for n times T for each engine
            template <typename T>
            void acquire(const memory::storage::model which, const size_t n)
            {
                executor     &self = *this;
                const size_t  nthr = num_threads();
                for(size_t i=0;i<nthr;++i)
                {
                    self(i).acquire(which,n*sizeof(T));
                }
            }

            //__________________________________________________________________
            //
            // making one object
            //__________________________________________________________________

            //! build one T for each engine with default constructor
            template <typename T>
            void make(const memory::storage::model which)
            {
                executor     &self = *this;
                const size_t  nthr = num_threads();
                for(size_t i=0;i<nthr;++i)
                {
                   self(i).make<T>(which);
                }
            }

            //! make one T for each engine from parameter
            template <typename T, typename U>
            void make(const memory::storage::model which, const typename type_traits<U>::parameter_type argU)
            {
                executor     &self = *this;
                const size_t  nthr = num_threads();
                for(size_t i=0;i<nthr;++i)
                {
                    self(i).make<T,U>(which,argU);
                }
            }


            //! make one T for each engine from parameters
            template <typename T, typename U, typename V>
            void make(const memory::storage::model                  which,
                      const typename type_traits<U>::parameter_type argU,
                      const typename type_traits<V>::parameter_type argV)
            {
                executor     &self = *this;
                const size_t  nthr = num_threads();
                for(size_t i=0;i<nthr;++i)
                {
                    self(i).make<T,U,V>(which,argU,argV);
                }
            }

            //__________________________________________________________________
            //
            // making multiple objects
            //__________________________________________________________________

            //! build n times  T for each engine with default constructor
            template <typename T>
            void vmake(const memory::storage::model which, const size_t n)
            {
                executor     &self = *this;
                const size_t  nthr = num_threads();
                for(size_t i=0;i<nthr;++i)
                {
                    self(i).vmake<T>(which,n);
                }
            }



            //! make n time T for each engine from parameter
            template <typename T, typename U>
            void vmake(const memory::storage::model which, const size_t n, const typename type_traits<U>::parameter_type argU)
            {
                executor     &self = *this;
                const size_t  nthr = num_threads();
                for(size_t i=0;i<nthr;++i)
                {
                    self(i).vmake<T,U>(which,n,argU);
                }
            }

            //__________________________________________________________________
            //
            // copy objects
            //__________________________________________________________________

            //! copy objects
            template <typename T>
            void copy(const memory::storage::model which, const typename type_traits<T>::parameter_type args)
            {
                executor     &self = *this;
                const size_t  nthr = num_threads();
                for(size_t i=0;i<nthr;++i)
                {
                    self(i).copy<T>(which,args);
                }
            }

            //! copy objects
            template <typename T>
            void vcopy(const memory::storage::model which, const size_t n, const typename type_traits<T>::parameter_type args)
            {
                executor     &self = *this;
                const size_t  nthr = num_threads();
                for(size_t i=0;i<nthr;++i)
                {
                    self(i).vcopy<T>(which,n,args);
                }
            }


            //__________________________________________________________________
            //
            // helpers
            //__________________________________________________________________

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
                    ans += self(i).get<T>();
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

