
#include "y/mkl/fitting/built-in/circle.hpp"
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
                        &X   = arr.next(),
                        &Y   = arr.next(),
                        &Z   = arr.next(),
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


        }
    }
}

