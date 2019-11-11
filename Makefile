TG=attend
CXX=c++
CFLAGS=-Wfatal-errors -Wall -Wextra -no-pie
LDFLAGS=-lFOX-1.6 -lsqlite3
#LDFLAGS=-static libFOX-1.6.a -lsqlite3 -no-pie -static-libstdc++
RM=@rm -f

# determine OS
ifeq ($(OS),Windows_NT)
# MSYS DIR
INCDIR=/mingw64/include/fox-1.6
OSDIR=-D AT_WINDOWS
WINLIBS=rc.o -luser32 -lkernel32 -ladvapi32 -lgdi32 -lws2_32 \
	-ljpeg -lpng -ltiff -lz -llzma -lzstd -static-libstdc++ \
	-mwindows
WINDRES=windres rc.rc -o rc.o
STATIC=-static

else
INCDIR=/usr/include/fox-1.6
OSDIR=-U AT_WINDOWS
WINLIBS=
WINDRES=
STATIC=
endif

all: inc.o attend
	# Finished!
attend:
	$(WINDRES)
	$(CXX) inc.o $(TG).cc -o $(TG) $(CFLAGS) $(OSDIR) \
		 $(STATIC) $(LDFLAGS) $(WINLIBS) -I $(INCDIR)
inc.o:
	nasm inc.asm -f elf64
rc.o:
	windres rc.rc -o rc.o
clean:
	$(RM) $(TG) *.o
.PHONY: all clean
