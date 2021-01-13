//! \file

#ifndef Y_MKL_FITTING_CIRCLES_INCLUDED
#define Y_MKL_FITTING_CIRCLES_INCLUDED 1

#include "y/sequence/list.hpp"
#include "y/sequence/vector.hpp"
#include "y/yap/rational.hpp"
#include "y/mkl/fitting/v-gradient.hpp"
#include "y/mkl/fitting/sequential.hpp"
#include "y/mkl/fitting/sample.hpp"
#include "y/mkl/fitting/least-squares.hpp"
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
                //! common parts for circle fitting
                //
                //______________________________________________________________
                class __circle : public object
                {
                public:
                    //__________________________________________________________
                    //
                    // types and definitions
                    //__________________________________________________________
                    typedef point2d<double>           vertex;          //!< internal vertex
                    typedef sequential<vertex,double> sequential_type; //!< sequential interface
                    typedef v_gradient<vertex,double> v_gradient_type; //!< gradient interface
                    typedef sample<vertex,double>     sample_type;     //!< internal sample

                    //__________________________________________________________
                    //
                    // methods
                    //__________________________________________________________
                    virtual ~__circle() throw(); //!< cleanup

                protected:
                    explicit __circle() throw();

                    //! perform effective computation
                    virtual bool solve_(vertex &center, double &radius) = 0;

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(__circle);
                };

                //______________________________________________________________
                //
                //
                //! interface to circle computation for rational/double
                //
                //______________________________________________________________
                template <typename T>
                class _circle : public releasable, public __circle
                {
                public:
                    //__________________________________________________________
                    //
                    // types and definitions
                    //__________________________________________________________
                    typedef __circle::vertex vertex;           //!< alias
                    typedef _circle<T>       circle_type;      //!< alias

                    //__________________________________________________________
                    //
                    // methods
                    //__________________________________________________________
                    inline virtual ~_circle() throw() {}   //!< cleanup
                    virtual void    free()    throw() = 0; //!< free all

                    //! interface to compute center/radius as floating point
                    inline bool solve(vertex &center,
                                      double &radius)
                    {
                        // initialize
                        assert( x.size() == y.size() );
                        mu.ld(zero);
                        tao::ld(rhs,zero);
                        center.x = center.y = radius = 0;

                        // call specialized proc
                        return this->solve_(center,radius);
                    }

                protected:
                    //! setup
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

                    //! release x and y
                    void release_xy() throw()
                    {
                        x.release();
                        y.release();
                    }

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(_circle);

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
                    virtual void free()    throw();                   //!< free all
                    virtual void release() throw();                   //!< release all

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(iCircle);
                    virtual bool solve_(vertex &center, double &radius) ;
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
                    explicit     dCircle();                           //!< setup
                    virtual     ~dCircle() throw();                   //!< cleanup
                    void         add(const double X, const double Y); //!< stored as double
                    virtual void free() throw();                      //!< free all
                    virtual void release() throw();                   //!< release all

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(dCircle);
                    virtual bool solve_(vertex &center, double &radius);
                };


                //______________________________________________________________
                //
                //
                //! fit circle with algebraic starting point then distance
                //
                //______________________________________________________________
                class circle :
                public __circle::sequential_type,
                public __circle::v_gradient_type
                {
                public:
                    //__________________________________________________________
                    //
                    // types and definitions
                    //__________________________________________________________
                    typedef __circle::vertex             vertex;       //!< alias
                    typedef __circle::sample_type        sample_type;  //!< alias
                    static const char * const            name[3];      //!< center.x, center.y, radius
                    typedef least_squares<vertex,double> ls_type;      //!< to fit a circle

                    //! algebraic methods to use
                    enum method
                    {
                        with_ints, //!< using iCircle
                        with_reals //!< using dCircle
                    };

                    //__________________________________________________________
                    //
                    // methods
                    //__________________________________________________________
                    virtual ~circle() throw();         //!< cleanup
                    explicit circle(const size_t n=0); //!< setup

                    const variables  &operator *() const throw(); //!< data variables

                    //! full fit
                    bool operator()(ls_type                  &ls,
                                    const accessible<double> &X,
                                    const accessible<double> &Y,
                                    addressable<double>      &aorg,
                                    const accessible<bool>   &used,
                                    addressable<double>      &aerr,
                                    const method              how);

                    //! free data and auxiliary
                    void free() throw();

                private:
                    Y_DISABLE_COPY_AND_ASSIGN(circle);
                    sample_type::pointer data;
                    auto_ptr<dCircle>    dptr;
                    auto_ptr<iCircle>    iptr;

                    double         get2D(const vertex &v,const accessible<double> &aorg,const variables &vars);
                    virtual double onStart(const vertex,const accessible<double> &,const variables &);
                    virtual double onReach(const vertex,const accessible<double> &,const variables &);
                    virtual void   compute(addressable<double>       &dFdA,
                                           const vertex                v,
                                           const accessible<double>   &aorg,
                                           const variables            &vars,
                                           const accessible<bool>     &used);

                };

            }

        }

    }

}


#endif
