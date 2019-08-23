TG=attend
CXX=c++
CFLAGS=-Wfatal-errors -Wall -Wextra
LDFLAGS=-lFOX-1.6 -lsqlite3
RM=@rm -f


# determine OS
ifeq ($(OS),Windows_NT)

INCDIR=C:\msys2\mingw64\include\fox-1.6

else

INCDIR=/usr/include/fox-1.6

endif

all:
	$(CXX) $(TG).cc -o $(TG) $(CFLAGS) $(LDFLAGS) -I $(INCDIR)
clean:
	$(RM) $(TG) *.o
