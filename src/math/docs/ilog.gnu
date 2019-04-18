v(i) = i*log(2);
a(i) = 1.0/2**i;
b(i) = (5.0-6.0*log(2))/2**( 2*i+1 );
c(i) = (3.0-4.0*log(2))/2**( 3*i+1 );

L(x,i) = v(i) + a(i) * (x-(2**i)) - b(i) * (x-2**i)**2 + c(i) * (x-2**i)**3;

l(x,i,A) = v(i) + (x-2**i)/(2**(i+1)-2**i) * ( v(i+1) - v(i) ) + A*(2**(i+1)-x)*(x-2**i);
