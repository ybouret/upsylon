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
            virtual void       run( kernel, void * ) = 0;

            //! number of underlying threads
            virtual size_t     num_threads()   const throw() = 0;

            //! access to individual context
            virtual parallel & get_context(const size_t indx) throw() = 0;

            //__________________________________________________________________
            //
            //
            // non-virtual interface
            //
            //__________________________________________________________________
            //! access to individual context
            parallel & operator[](const size_t) throw();

            //! access to individual context, CONST
            const parallel & operator[](const size_t) const throw();

            //! allocate memory per context
            void acquire_all(const size_t n);

            //! free all memory per context
            void free_all() throw();

            //! allocate memory per context
            template <typename T>
            void acquire_for() { acquire_all( sizeof(T) ); }

            //! sum of local stored type
            template <typename T> inline
            T sum() const
            {
                const executor &self = *this;
                T               ans(0);
                size_t          i    = num_threads();
                while(i-->0)
                {
                    ans +=  self[i].get<T>();
                }
                return ans;
            }

            //! zero argument build data for each context
            template <typename T>
            inline void build()
            {
                executor    &self = *this;
                const size_t n    = num_threads();
                try
                {
                    for(size_t i=0;i<n;++i)
                    {
                        self[i].build<T>();
                    }
                }
                catch(...)
                {
                    free_all();
                    throw;
                }
            }

            //! zero argument build data for each context
            template <typename T>
            inline void build_from( typename type_traits<T>::parameter_type arg )
            {
                executor    &self = *this;
                const size_t n    = num_threads();
                try
                {
                    for(size_t i=0;i<n;++i)
                    {
                        self[i].build_from<T>(arg);
                    }
                }
                catch(...)
                {
                    free_all();
                    throw;
                }
            }

            //! build with one argument
            template <typename T,typename U> inline
            void build( typename type_traits<U>::parameter_type u )
            {
                executor    &self = *this;
                const size_t n    = num_threads();
                try
                {
                    for(size_t i=0;i<n;++i)
                    {
                        self[i].build<T,U>(u);
                    }
                }
                catch(...)
                {
                    free_all();
                    throw;
                }
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

