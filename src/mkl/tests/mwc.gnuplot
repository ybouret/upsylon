SCALING=1e-7
Kh=0.17*SCALING;
Kl=36*SCALING
c =Kh/Kl;
L0=5000;
Vmax=40;

num(alpha) = alpha * (1+alpha) + L0 * c * alpha * (1+c*alpha);
den(alpha) = L0 * (1+c*alpha)**2 + (1+alpha)**2;
rate(alpha) = Vmax * num(alpha) / den(alpha);

plot [0:1e-6] 'mwc.txt' w lp, rate(x/Kh)


