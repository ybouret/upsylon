
#include "y/mkl/fitting/built-in/circles.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/sequence/arrays.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            namespace  built_in
            {
                __circle::  __circle() throw() {}
                __circle:: ~__circle() throw() {}

            }


            namespace  built_in
            {

                iCircle:: ~iCircle() throw()
                {
                }

                iCircle:: iCircle() : circle_type(), z()
                {
                }

                void iCircle:: free() throw()
                {
                    free_xy();
                    z.free();
                }

                void iCircle:: release() throw()
                {
                    release_xy();
                    z.release();
                }


                void iCircle:: add(const unit_t X, const unit_t Y)
                {
                    const apq _x = X;
                    const apq _y = Y;
                    const apq _z = apq::square_of(_x) + apq::square_of(_y);
                    try
                    {
                        x.push_back(_x);
                        y.push_back(_y);
                        z.push_back(_z);
                    }
                    catch(...)
                    {
                        free();
                        throw;
                    }
                }

                bool iCircle:: solve_(point2d<double> &center, double &radius)
                {
                    assert(x.size()==y.size());
                    assert(x.size()==z.size());
                    const size_t n = x.size();
                    

                    size_t count = n;
                    apq &sum_zx = rhs[1]; assert(0==sum_zx);
                    apq &sum_zy = rhs[2]; assert(0==sum_zy);
                    apq &sum_z  = rhs[3]; assert(0==sum_z);

                    apq &sum_xx = mu[1][1]; assert(0==sum_xx);
                    apq &sum_xy = mu[1][2]; assert(0==sum_xy);
                    apq &sum_x  = mu[1][3]; assert(0==sum_x);

                    apq &sum_yy = mu[2][2]; assert(0==sum_yy);
                    apq &sum_y  = mu[2][3]; assert(0==sum_y);

                    mu[3][3] = n;

                    for( list<apq>::iterator i=x.begin(), j=y.begin(), k=z.begin(); count>0; --count, ++i, ++j, ++k)
                    {
                        const apq &_x = *i;
                        const apq &_y = *j;
                        const apq &_z = *k;

                        sum_zx += _z * _x;
                        sum_zy += _z * _y;
                        sum_z  += _z;

                        sum_xx += _x * _x;
                        sum_xy += _x * _y;
                        sum_x  += _x;

                        sum_yy += _y * _y;
                        sum_y  += _y;
                    }

                    regularize();


                    if(LU::build(mu))
                    {
                        LU::solve(mu,rhs);
                        rhs[1]/=2;
                        rhs[2]/=2;
                        const double ha = rhs[1].to_double();
                        const double hb = rhs[2].to_double();
                        const double c  = rhs[3].to_double();
                        center.x = ha;
                        center.y = hb;
                        radius   = sqrt( max_of<double>(0,c+square_of(ha)+square_of(hb)));
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }


            }


            namespace  built_in
            {
                dCircle:: ~dCircle() throw()
                {
                }

                dCircle:: dCircle() : circle_type()
                {
                }

                void dCircle:: free() throw()
                {
                    free_xy();
                }

                void dCircle:: release() throw()
                {
                    release_xy();
                }

                void dCircle:: add(const double X, const double Y)
                {
                    try
                    {
                        x.push_back(X);
                        y.push_back(Y);
                    }
                    catch(...)
                    {
                        free();
                        throw;
                    }
                }

                bool dCircle:: solve_(point2d<double> &center, double &radius)
                {
                    assert(x.size()==y.size());
                    const size_t n = x.size();
                    
                    if(n>0)
                    {
                        arrays<double> arr(4,n);
                        lightweight_array<double>
                        &X = arr.next(),
                        &Y = arr.next(),
                        &Z = arr.next(),
                        &tmp = arr.next();

                        // load X/Y
                        {
                            size_t count = n;
                            for(list<double>::iterator i=x.begin(), j=y.begin(); count>0; --count, ++i, ++j)
                            {
                                X[count] = *i;
                                Y[count] = *j;
                            }
                        }

                        //std::cerr << "x=" << X << std::endl;
                        //std::cerr << "y=" << Y << std::endl;



                        // compute bar
                        tao::set(tmp,X); const double xbar = sorted_sum_by_abs(tmp)/n;
                        tao::set(tmp,Y); const double ybar = sorted_sum_by_abs(tmp)/n;

                        // update X/Y/Z
                        for(size_t i=n;i>0;--i)
                        {
                            Z[i]  = square_of(X[i] -= xbar) + square_of(Y[i] -= ybar);
                        }

                        // compute rhs
                        for(size_t i=n;i>0;--i) tmp[i] = Z[i] * X[i];
                        rhs[1] = sorted_sum_by_abs(tmp);

                        for(size_t i=n;i>0;--i) tmp[i] = Z[i] * Y[i];
                        rhs[2] = sorted_sum_by_abs(tmp);

                        for(size_t i=n;i>0;--i) tmp[i] = Z[i];
                        rhs[3] = sorted_sum_by_abs(tmp);

                        // compute mu
                        for(size_t i=n;i>0;--i) tmp[i] = square_of(X[i]);
                        mu[1][1] = sorted_sum_by_abs(tmp);

                        for(size_t i=n;i>0;--i) tmp[i] = X[i]*Y[i];
                        mu[1][2] = sorted_sum_by_abs(tmp);

                        for(size_t i=n;i>0;--i) tmp[i] = X[i];
                        mu[1][3] = sorted_sum_by_abs(tmp);


                        for(size_t i=n;i>0;--i) tmp[i] = square_of(Y[i]);
                        mu[2][2] = sorted_sum_by_abs(tmp);

                        for(size_t i=n;i>0;--i) tmp[i] = Y[i];
                        mu[2][3] = sorted_sum_by_abs(tmp);

                        mu[3][3] = double(n);

                        regularize();



                        if(LU::build(mu))
                        {
                            LU::solve(mu,rhs);
                            const double ha = rhs[1]/2;
                            const double hb = rhs[2]/2;
                            center.x = ha + xbar;
                            center.y = hb + ybar;
                            radius   = sqrt( max_of<double>(0,rhs[3] + square_of(ha) + square_of(hb)));
                            return true;
                        }
                        else
                        {
                            return false;
                        }
                        
                    }
                    else
                    {
                        return false;
                    }

                }


            }

            namespace built_in
            {

                const char * const circle:: name[3] =
                {
                    "center.x",
                    "center.y",
                    "radius"
                };

                circle:: ~circle() throw()
                {
                }

                circle:: circle(const size_t n) :
                data( sample_type::create("circle",n) ),
                dptr(NULL),
                iptr(NULL)
                {
                    **data << name[0] << name[1] << name[2];
                }

                double circle:: get2D(const vertex &v, const accessible<double> &aorg, const variables &vars)
                {
                    const double xc = vars(aorg,name[0]);
                    const double yc = vars(aorg,name[1]);
                    const double rr = vars(aorg,name[2]);
                    return sqrt( square_of(v.x-xc) + square_of(v.y-yc) ) - rr;
                }

                double circle:: onStart(const vertex v, const accessible<double> &aorg, const variables &vars)
                {
                    return get2D(v,aorg,vars);
                }

                double circle:: onReach(const vertex v, const accessible<double> &aorg, const variables &vars)
                {
                    return get2D(v,aorg,vars);
                }

                void  circle:: compute(addressable<double>        &dF,
                                       const vertex                v,
                                       const accessible<double>   &aorg,
                                       const variables            &vars,
                                       const accessible<bool>     &used)
                {
                    static const double tiny = numeric<double>::tiny;

                    const variable &xc = vars[name[0]];
                    const variable &yc = vars[name[1]];
                    const variable &rr = vars[name[2]];

                    const vertex center(xc(aorg),yc(aorg));
                    const vertex delta = center - v;
                    const double den   = sqrt( delta.norm2() ) + tiny;

                    if( xc(used) ) xc(dF) = delta.x/den;
                    if( yc(used) ) yc(dF) = delta.y/den;
                    if( rr(used) ) rr(dF) = -1;


                }

                const variables & circle:: operator*() const throw()
                {
                    return data->vars;
                }

                bool circle:: operator()(ls_type                  &ls,
                                         const accessible<double> &X,
                                         const accessible<double> &Y,
                                         addressable<double>      &aorg,
                                         const accessible<bool>   &used,
                                         addressable<double>      &aerr,
                                         const method              how)
                {
                    assert(X.size()==Y.size());

                    // initialize
                    data->free();
                    const size_t n       = X.size();
                    vertex       center;
                    double       radius  = 0;
                    bool         success = false;


                    // starting point: algebraic circle
                    switch(how)
                    {
                        case with_ints:
                            if(iptr.is_empty()) iptr = new iCircle();
                            iptr->free();
                            for(size_t i=n;i>0;--i) iptr->add( unit_t(X[i]), unit_t(Y[i]));
                            success = iptr->solve(center,radius);
                            break;

                        case with_reals: if(dptr.is_empty()) dptr = new dCircle();
                            for(size_t i=n;i>0;--i) iptr->add(X[i],Y[i]);
                            success = iptr->solve(center,radius);
                            break;
                    }
                    if(!success)
                    {
                        return false;
                    }
                    std::cerr << "circle: center=" << center << ", radius=" << radius << std::endl;

                    // prepare sample
                    for(size_t i=n;i>0;--i)
                    {
                        const vertex v(X[i],Y[i]);
                        data->add(v,0);
                    }

                    // prepare aorg
                    const variables &vars = **this;
                    const variable  &xc   = vars[name[0]];
                    const variable  &yc   = vars[name[1]];
                    const variable  &rr   = vars[name[2]];

                    if(xc(used)) xc(aorg) = center.x;
                    if(yc(used)) yc(aorg) = center.y;
                    if(rr(used)) rr(aorg) = radius;

                    __circle::sequential_type &F = *this;
                    __circle::v_gradient_type &G = *this;
                    return ls.fit(*data, F, G, aorg, used, aerr);

                }

                void circle:: free() throw()
                {
                    data->free();
                    if(iptr.is_valid()) iptr->free();
                    if(dptr.is_valid()) dptr->free();
                }

            }

        }

    }
}

