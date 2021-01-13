//! \file

#ifndef Y_MKL_FITTING_CIRCLES_INCLUDED
#define Y_MKL_FITTING_CIRCLES_INCLUDED 1

#include "y/sequence/list.hpp"
#include "y/sequence/vector.hpp"
#include "y/yap/rational.hpp"
#include "y/container/matrix.hpp"
#include "y/mkl/tao.hpp"
#include "y/type/point2d.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            namespace built_in
            {

                //______________________________________________________________
                //
                //
                //! interface to circle computation
                //
                //______________________________________________________________
                template <typename T>
                class _circle
                {
                public:
                    typedef _circle<T> circle_type;      //!< alias

                    inline virtual ~_circle() throw() {} //!< cleanup

                    virtual void free() throw() = 0;     //!< free all

                    //! interface to compute center/radius as floating point
                    inline bool solve(point2d<double> &center,
                                      double          &radius)
                    {
                        // initialize
                        assert( x.size() == y.size() );
                        mu.ld(zero);
                        tao::ld(rhs,zero);
                        center.x = center.y = radius = 0;

                        // call specialized
                        return solve_(center,radius);
                    }

                protected:
                    inline explicit _circle() :
                    zero(0), x(), y(), mu(3,3), rhs(3,zero) {}

                    const T        zero; //!< a zero value
                    list<T>        x;    //!< x coord
                    list<T>        y;    //!< y coord
                    matrix<T>      mu;   //!< moment matrix
                    vector<T>      rhs;  //!< rhs vector

                    //! regularize symmetric matrix
                    inline void regularize()
                    {
                        mu[2][1] = mu[1][2];
                        mu[3][1] = mu[1][3];
                        mu[3][2] = mu[2][3];
                    }

                    //! free x and y
                    void free_xy() throw()
                    {
                        x.free();
                        y.free();
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(_circle);
                    virtual bool solve_(point2d<double> &center, double &radius) = 0;

                };

                //______________________________________________________________
                //
                //
                //! using integer coordinates
                //
                //______________________________________________________________
                class iCircle : public _circle<apq>
                {
                public:
                    explicit iCircle();           //!< setup
                    virtual ~iCircle() throw();   //!< cleanup

                    void         add(const unit_t X, const unit_t Y); //!< converted to rational
                    virtual void free() throw();                      //!< free all

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(iCircle);
                    virtual bool solve_(point2d<double> &center, double &radius) ;
                    list<apq> z;

                };

                //______________________________________________________________
                //
                //
                //! using floating point coordinates
                //
                //______________________________________________________________
                class dCircle : public _circle<double>
                {
                public:
                    explicit dCircle();          //!< setup
                    virtual ~dCircle() throw();  //!< cleanup

                    void         add(const double X, const double Y); //!< stored as double
                    virtual void free() throw();                      //!< free all

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(dCircle);
                    virtual bool solve_(point2d<double> &center, double &radius);
                };

            }

        }

    }

}


#endif
