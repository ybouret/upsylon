max(a,b) = a>b ? a:b;
min(a,b) = a>b ? b:a;
H(u)     = u<=0 ? 0 : 1;
psi(x)   = 0.5*( max(-x,0) ) ** 2;
dpsi(x)  = min(x,0); 
