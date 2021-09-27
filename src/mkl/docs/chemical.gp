max(a,b) = a>b ? a:b;
min(a,b) = a>b ? b:a;
H(u)     = u<=0 ? 0 : 1;
psi(x)   = 0.5*( max(-x,0) ) ** 2;
dpsi(x)  = min(x,0); 

plot [-2:2] psi(x) w p, 0.5*x**2, x, dpsi(x), H(-x)

