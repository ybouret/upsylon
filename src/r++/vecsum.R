dyn.load("vecsum.so");
a = rnorm(10);
.Call("vecsum",a);

