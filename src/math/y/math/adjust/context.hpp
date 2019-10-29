//! \file

#ifndef Y_MATH_ADJUST_CONTEXT_INCLUDED
#define Y_MATH_ADJUST_CONTEXT_INCLUDED 1

#include "y/math/adjust/sample.hpp"

namespace upsylon {

    namespace math {

        namespace Adjust {

            //! modification information
            enum ModifyStatus
            {
                LeftUntouched,
                ModifiedState,
                ModifiedShift,
                EmergencyExit
            };

            //! context to modify a current fit or use as callback
            template <typename T>
            class Context : public accessible< Sample<T> >
            {
            public:
                //! cleanup
                inline virtual ~Context() throw() {}

                //! number of used single samples
                inline virtual size_t size() const throw() { return _data.size(); }

                //! access the single smaples
                inline const Sample<T> & operator[]( const size_t indx ) const
                {
                    return *static_cast< const Sample<T> *>(_data[indx]);
                }

                //! setup
                inline explicit Context(const SampleType<T> & _self,
                                        const accessible<T> & _aorg,
                                        const accessible<bool>         & _used,
                                        addressable<T>      & _atry,
                                        addressable<T>      & _step) :
                flags( _used ),
                start( _aorg ),
                state( _atry ),
                shift( _step ),
                cycle(0),
                _data()
                {
                    _self.collect(_data);
                }

                const accessible<bool> &flags; //!< currently used flags
                const accessible<T> &start; //!< starting position
                addressable<T>      &state; //!< current  position
                addressable<T>      &shift; //!< current  step
                const size_t         cycle; //!< internal cycle index

                //! function to modify in progress
                typedef functor<ModifyStatus,TL1(Context&)> Modify;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Context);
                vector<void*,Allocator> _data;

            };


        }

    }

}

#endif
