//! \file

#ifndef Y_MEMORY_GROOVES_INCLUDED
#define Y_MEMORY_GROOVES_INCLUDED 1

#include "y/memory/groove.hpp"
#include "y/sequence/addressable.hpp"

namespace upsylon {

    namespace memory {

        //______________________________________________________________________
        //
        //
        //! flexible cache multiple grooves
        //
        //______________________________________________________________________
        class grooves : public addressable<groove>
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! setup with collection memory model
            explicit grooves(const storage::model = storage::shared) throw();

            //! cleanup
            virtual ~grooves() throw();

            //__________________________________________________________________
            //
            // accessible interface
            //__________________________________________________________________
            virtual size_t size() const throw();                            //!< current number of grooves
            virtual groove       & operator[](const size_t) throw();        //!< [1..size()]
            virtual const groove & operator[](const size_t) const throw();  //!< [1..size()], const

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void           make(const size_t n);          //!< ensure exactly n clean grooves
            void           free() throw();                //!< free content, keep clean grooves
            void           release() throw();             //!< release all memory
            void           upgrade(const storage::model); //!< update with a valid model on a released state
            const groove & handler() const throw();       //!< for information
            groove       & create1();                     //!< make(1) and return front
            groove       & operator*()       throw();     //!< get first groove for size()>0
            const groove & operator*() const throw();     //!< get first groove for size()>0

            //! create one groove, default build with T
            template <typename T> inline
            T &shape(const storage::model which)
            {
                return create1().make<T>(which).template get<T>();
            }

            //! create one grooves, default build n times T
            template <typename T> inline
            void shape(const storage::model which, const size_t n)
            {
                create1().make<T>(which,n);
            }


            //! create two grooves, default build with T and U
            template <typename T,typename U> inline
            void shape(const storage::model which)
            {
                grooves &self = *this;
                make(2);
                self[1].make<T>(which);
                self[2].make<U>(which);
            }

            //! create three grooves, default build with T and U
            template <typename T,typename U,typename V> inline
            void shape(const storage::model which)
            {
                grooves &self = *this;
                make(3);
                self[1].make<T>(which);
                self[2].make<U>(which);
                self[3].make<V>(which);
            }

            //! build one groove from parameter U
            template <typename T, typename U> inline
            T & build(const storage::model which, const typename type_traits<U>::parameter_type argU)
            {
                return create1().build<T,U>(which,argU).template get<T>();
            }

            //! build one groove from parameter U repeated n times
            template <typename T, typename U> inline
            void build(const storage::model which, const size_t n, const typename type_traits<U>::parameter_type argU)
            {
                create1().build<T,U>(which,n,argU);
            }


            //! ready memory for one groove sizeof(T)
            template <typename T> inline
            void ready(const storage::model which)
            {
                static const size_t sz = sizeof(T);
                ready_(which,&sz,1);
            }

            //! ready memory for one groove, n times sizeof(T)
            template <typename T> inline
            void ready(const storage::model which, const size_t n)
            {
                static const size_t sz = n*sizeof(T);
                ready_(which,&sz,1);
            }

            //! ready memory for two grooves
            template <typename T,typename U> inline
            void ready(const storage::model which)
            {
                static const size_t sz[] = { sizeof(T), sizeof(U) };
                ready_(which,sz,2);
            }

            //! ready memory for three grooves
            template <typename T,typename U,typename V> inline
            void ready(const storage::model which)
            {
                static const size_t sz[] = { sizeof(T), sizeof(U), sizeof(V) };
                ready_(which,sz,3);
            }


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________

        private:
            Y_DISABLE_COPY_AND_ASSIGN(grooves);
            groove              *shift;
            groove               front;
            void ready_(const storage::model, const size_t sz[], const size_t ns);

        public:
            const storage::model model; //!< current memory model
        };

    }

}

#endif

