TG=attend
CXX=c++
CFLAGS=-Wfatal-errors -Wall -Wextra -no-pie
LDFLAGS=-static -lFOX-1.6 -lsqlite3
#LDFLAGS=-static libFOX-1.6.a -lsqlite3 -no-pie -static-libstdc++
RM=@rm -f


# determine OS
ifeq ($(OS),Windows_NT)

INCDIR=/mingw64/include/fox-1.6
OSDIR=-D AT_WINDOWS
WINLIBS=-luser32 -lkernel32 -ladvapi32 -lgdi32 -lws2_32 -ljpeg -lpng -ltiff -lz -llzma -lzstd -static-libstdc++

else

INCDIR=/usr/include/fox-1.6
OSDIR=-U AT_WINDOWS
WINLIBS=

endif




all: inc.o
	$(CXX) inc.o $(TG).cc -o $(TG) $(CFLAGS) $(OSDIR) $(LDFLAGS) $(WINLIBS) -I $(INCDIR)
inc.o:
	nasm inc.asm -f elf64
clean:
	$(RM) $(TG) *.o
