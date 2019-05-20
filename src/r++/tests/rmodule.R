dyn.load("rmodule.so");
a = rnorm(10);
.Call("vecsum",a);

