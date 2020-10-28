//! \file
#ifndef Y_CONCURRENT_EXECUTOR_INCLUDED
#define Y_CONCURRENT_EXECUTOR_INCLUDED 1

#include "y/parallel.hpp"
#include "y/concurrent/fake-lock.hpp"
//#include "y/mkl/transform.hpp"

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
        class executor : public addressable<parallel>
        {
        public:
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


            //__________________________________________________________________
            //
            //
            // flat memory per thread
            //
            //__________________________________________________________________

            void acquire(const size_t);   //!< acquire bytes for all contexts
            void free()    throw();       //!< for all contexts
            void release() throw();       //!< for all contexts

            //__________________________________________________________________
            //
            //
            // objects per threads
            //
            //__________________________________________________________________

            //! build ONE default T for each context
            template <typename T> inline
            void make()
            {
                executor    &self = *this;
                const size_t nthr = size();
                for(size_t i=1;i<=nthr;++i)
                {
                    (void) self[i].make<T>();
                }
            }

            //! build n default T for each context
            template <typename T> inline
            void make(size_t n)
            {
                executor    &self = *this;
                const size_t nthr = size();
                for(size_t i=1;i<=nthr;++i)
                {
                    (void) self[i].make<T>(n);
                }
            }

            //! access default first item
            template <typename T>
            T & _(const size_t indx) throw()
            {
                executor &self = *this;
                return    self[indx]._<T>();
            }

            //! access default first const item
            template <typename T>
            const T & _(const size_t indx) const throw()
            {
                const accessible<parallel> &self = *this;
                return self[indx]._<T>();
            }

            //! access for multiple index
            template <typename T>
            T & _(const size_t i, const size_t j) throw()
            {
                executor &self = *this;
                return    self[i]._<T>(j);
            }

            //! access for multiple index, const
            template <typename T>
            const T & _(const size_t i, const size_t j) const throw()
            {
                const accessible<parallel> &self = *this;
                return    self[i]._<T>(j);
            }

            //! get thread specific data
            template <typename T>
            struct dowload
            {
                //! first items with conversion
                template <typename TARGET> inline
                static void to(TARGET &target, const executor &source)
                {
                    const accessible<parallel> &self = source;
                    for(size_t i=self.size();i>0;--i)
                    {
                        target[i] = self[i]._<T>();
                    }
                }
            };

            //! put thread specific data
            template <typename T>
            struct upload
            {
                //! first items with conversion
                template <typename SOURCE> inline
                static void to(executor &target, SOURCE &source)
                {
                    for(size_t i=target.size();i>0;--i)
                    {
                        target[i]._<T>() = source[i];
                    }
                }
            };

            

            
        protected:
            //! constructor
            explicit  executor() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(executor);
        };

        //! implements a sequential executor
        class sequential : public executor
        {
        public:
            explicit sequential() throw();             //!< constructor
            virtual ~sequential() throw();             //!< destructor
            virtual void             run(kernel proc, void *);               //!< direct call
            virtual size_t           size()                   const throw(); //!< only one
            virtual parallel       & operator[](const size_t) throw();       //!< [1]
            virtual const parallel & operator[](const size_t) const throw(); //!< [1]

        private:
            parallel  context;

        public:
            fake_lock access; //!< shared access

        };

    }
}

#endif

