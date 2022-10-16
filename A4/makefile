BINS = diffusion
CFLAGS = -O0 -lOpenCL -g

.PHONY : all
all : $(BINS)

diffusion : diffusion.c
	gcc $(CFLAGS) -o $@ $<

.PHONY : clean
clean :
	rm -rf $(BINS)