TG=attend
CXX=c++
CFLAGS=-Wfatal-errors -Wall -Wextra -no-pie
LDFLAGS=-lFOX-1.6 -lsqlite3 -lX11 -no-pie
RM=@rm -f


# determine OS
ifeq ($(OS),Windows_NT)

INCDIR=/mingw64/include/fox-1.6
OSDIR=-D AT_WINDOWS

else

INCDIR=/usr/include/fox-1.6
OSDIR=-U AT_WINDOWS

endif




all: inc.o
	$(CXX) inc.o $(TG).cc -o $(TG) $(CFLAGS) $(OSDIR) $(LDFLAGS) -I $(INCDIR)
inc.o:
	nasm inc.asm -f elf64
clean:
	$(RM) $(TG) *.o
