
//! \file

#ifndef Y_ALCHEMY_LIMIT_INCLUDED
#define Y_ALCHEMY_LIMIT_INCLUDED 1

#include "y/alchemy/library.hpp"
#include "y/container/tuple.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        //______________________________________________________________________
        //
        //
        //! define an extent limit
        //
        //______________________________________________________________________
        class Limit
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            Limit()              throw(); //!< setup
            Limit(const Limit &) throw(); //!< copy
            ~Limit()             throw(); //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void reset() throw(); //!< off/0

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            bool   on; //!< active or not
            double xi; //!< extent

        private:
            Y_DISABLE_ASSIGN(Limit);
        };


        //______________________________________________________________________
        //
        //
        //! set of limits
        //
        //______________________________________________________________________
        class Limits
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________

            //! possible states
            enum State
            {
                Unbounded, //!< lower.off and upper.off
                UpperOnly, //!< lower.off and upper.on
                LowerOnly, //!< lower.on  and upper.off
                WithRange, //!< lower.on  and upper.on,  lower.xi <= upper.xi
                Exclusive  //!< lower.on  and upper.on,  lower.xi >  upper.xi
            };

            //! base type for Array
            typedef vector<Limits,Allocator> Array_;

            //! Array of Limits
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
            Limits()               throw(); //!< setup
            ~Limits()              throw(); //!< cleanup
            Limits(const Limits &) throw(); //!< copy

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void  reset()    throw(); //!< reset all, set Unbounded
            bool  mobility() throw(); //!< update and return mobility

            //! show info
            template <typename OSTREAM> inline
            OSTREAM & show(OSTREAM &os) const
            {
                switch(state)
                {
                    case Unbounded:
                        assert(!lower.on);
                        assert(!upper.on);
                        os << "Unbounded";
                        break;

                    case UpperOnly:
                        assert(!lower.on);
                        assert(upper.on);
                        os << "UpperOnly <= " << upper.xi;
                        break;

                    case LowerOnly:
                        assert(lower.on);
                        assert(!upper.on);
                        os << "LowerOnly >= " << lower.xi;
                        break;

                    case WithRange:
                        assert(lower.on);
                        assert(upper.on);
                        assert(lower.xi<=upper.xi);
                        os << "WithRange [" << lower.xi << "->" << upper.xi << "]";
                        break;

                    case Exclusive:
                        assert(lower.on);
                        assert(upper.on);
                        assert(lower.xi>upper.xi);
                        os << "Exclusive " << lower.xi << " > " << upper.xi;
                        break;
                }
                return os;
            }

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            Limit       lower; //!< lower limit
            Limit       upper; //!< upper limit
            State       state; //!< matching state

        private:
            Y_DISABLE_ASSIGN(Limits);
        };


    }

}

#endif
