

//! \file

#ifndef Y_ALCHEMY_GUARD_INCLUDED
#define Y_ALCHEMY_GUARD_INCLUDED 1

#include "y/alchemy/leading.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        //______________________________________________________________________
        //
        //
        //! guard leading conditions for an equilibrium
        //
        //______________________________________________________________________
        class Guard : public object, public counted
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef arc_ptr<Guard>            Pointer;
            typedef vector<Pointer,Allocator> Array_;

            //! class of guard
            enum Class
            {
                HasNoBound,   //!< no leading condition
                HasOnlyLEQ,   //!< leq.size>0, geq.size==0
                HasOnlyGEQ,   //!< leq.size==0, geq.size>0
                IsBothWays    //!< leq.size>0, geq.size>0
            };

            static const char * ClassText(const Class c) throw();


            //! pre-allocated arrays
            class Array : public Array_
            {
            public:
                explicit Array(size_t);   //!< setup
                virtual ~Array() throw(); //!< cleanup
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Array);
            };

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Guard() throw();
            virtual ~Guard() throw();

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void         addLEQ(const Leading &);
            void         addGEQ(const Leading &);
            const char * classText() const throw();
            template <typename OSTREAM> inline
            OSTREAM &print(OSTREAM          &os,
                           const Library    &lib,
                           const Equilibria &eqs,
                           const Accessible &C,
                           const char       *pfx = 0) const
            {
                if(!pfx) pfx = "";
                print_(os,lib,eqs,leq,C,pfx);
                print_(os,lib,eqs,geq,C,pfx);
                return os;
            }

            //__________________________________________________________________
            //
            // computation methods
            //__________________________________________________________________
            
            //! getMax = min of leq, leq.size()>0
            const Leading &xiMax(const Accessible &C) const throw();

            //! getMin = max of geq, geq.size()>0
            const Leading &xiMin(const Accessible &C) const throw();

            void solve(Addressable &C, const iMatrix &NuT, Addressable &xi) const throw();

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Leading::Array leq;
            const Leading::Array geq;
            const Class          cls;


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Guard);
            void update() throw();
            template <typename OSTREAM> inline static
            OSTREAM &print_(OSTREAM               &os,
                            const Library         &lib,
                            const Equilibria      &eqs,
                            const Leading::Array  &arr,
                            const Accessible      &C,
                            const char            *pfx)
            {
                assert(pfx);
                for(size_t i=1;i<=arr.size();++i)
                {
                    arr[i].print(os << pfx ,lib,eqs,C) << '\n';
                }
                return os;
            }

        };

    }

}

#endif
