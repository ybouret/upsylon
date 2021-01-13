
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


                        return true;
                    }
                    else
                    {
                        return false;
                    }

                }


            }
        }

    }
}

