
MY_CFLAGS  = -O3 -Wall -fopenmp -I/opt/local/include
MY_LDFLAGS = -lm 

all: pi pi-ng

	
pi.c    : pi-core.c pi.h
pi-ng.c : pi-core.c pi.h

pi: pi.c
	$(CC)  $(CFLAGS) $(MY_CFLAGS) $^ -o $@ $(LDFLAGS) $(MY_LDFLAGS) 
	
pi-ng: pi-ng.c
	$(CC)  $(CFLAGS) $(MY_CFLAGS) $^ -o $@ $(LDFLAGS) $(MY_LDFLAGS) 
	

clean:
	@rm -f *.o pi pi-ng pi.exe pi-ng.exe
	
arch: clean
	rm -f pi.zip; zip -v9 pi.zip toto
