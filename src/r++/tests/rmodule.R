dyn.load("rmodule.so");
a = rnorm(10);
print(a);
.Call("vecsum",a);
print(a);

