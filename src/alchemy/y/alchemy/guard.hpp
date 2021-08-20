

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
            typedef arc_ptr<Guard>            Pointer; //!< alias
            typedef vector<Pointer,Allocator> Array_;  //!< alias

            //! class of guard
            enum Class
            {
                HasNoBound,   //!< no leading condition
                HasOnlyLEQ,   //!< leq.size>0, geq.size==0
                HasOnlyGEQ,   //!< leq.size==0, geq.size>0
                IsBothWays    //!< leq.size>0, geq.size>0
            };

            //! named class
            static const char * ClassText(const Class c) throw();


            //! state of solving
            enum State
            {
                WasValid, //!< didn't need to be moved
                WasMoved, //!< was moved
                IsJammed  //!< jammed
            };

            //! named state
            static const char * StateText(const State s) throw();

            //! pre-allocated array of guards
            class Array : public Array_
            {
            public:
                explicit Array(size_t);   //!< setup
                virtual ~Array() throw(); //!< cleanup
                
                //! apply all limitation to an extent
                void     limit(Addressable      &xi,
                               const Accessible &C) const throw();
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Array);
            };

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Guard() throw(); //!< setup
            virtual ~Guard() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void         addLEQ(const Leading &);   //!< add to leq, update
            void         addGEQ(const Leading &);   //!< add to geq, update
            const char * classText() const throw(); //!< get named class

            //! print status w.r.t. concentrations
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
            
            //!  min of leq, leq.size()>0
            const Leading &xiMax(const Accessible &C) const throw();

            //!  max of geq, geq.size()>0
            const Leading &xiMin(const Accessible &C) const throw();

            //! solve in different cases
            State solve(Addressable    &C,
                        const iMatrix  &NuT,
                        Addressable    &xi,
                        Flags          &ok) const throw();

            //! limit in different cases
            void limit(Addressable &xi, const Accessible &C) const throw();
            

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Leading::Array leq; //!< nu * xi <= [conc]
            const Leading::Array geq; //!< nu * xi >= -[conc]
            const Class          cls; //!< depending on leq/geq


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
