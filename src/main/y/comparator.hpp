//! \file

#ifndef Y_COMPARATOR_INCLUDED
#define Y_COMPARATOR_INCLUDED 1

#include "y/comparison.hpp"
#include "y/type/args.hpp"

namespace upsylon
{

    //! prototype comparator
    template <typename T>
    class comparator
    {
    public:
        Y_DECL_ARGS(T,type);                                         //!< aliases
        inline  virtual ~comparator() throw() {}                     //!< cleanup
        virtual int      operator()(const_type &, const_type &) = 0; //!< return comparison

    protected:
        inline  explicit comparator() throw() {}                     //!< setup

    private:
        Y_DISABLE_COPY_AND_ASSIGN(comparator);
    };
    
    //! implementation: increasing
    template <typename T>
    class increasing_comparator : public comparator<T>
    {
    public:
        Y_DECL_ARGS(T,type); //!< alias

        inline virtual ~increasing_comparator() throw() {} //!< cleanup
        inline explicit increasing_comparator() throw() {} //!< setup
        
        //! increasing
        virtual int operator()(const_type &lhs, const_type &rhs) { return comparison::increasing<T>(lhs,rhs); }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(increasing_comparator);

    };
    
    //! implementation: decreasing
    template <typename T>
    class decreasing_comparator : public comparator<T>
    {
    public:
        Y_DECL_ARGS(T,type); //!< alias
        
        inline virtual ~decreasing_comparator() throw() {} //!< cleanup
        inline explicit decreasing_comparator() throw() {} //!< setup
        
        //! decreasing
        virtual int operator()(const_type &lhs, const_type &rhs) { return comparison::decreasing<T>(lhs,rhs); }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(decreasing_comparator);
        
    };
    
    
}

#endif

