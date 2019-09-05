TG=attend
CXX=c++
CFLAGS=-Wfatal-errors -Wall -Wextra
LDFLAGS=-lFOX-1.6 -lsqlite3
RM=@rm -f


# determine OS
ifeq ($(OS),Windows_NT)

INCDIR=/mingw64/include/fox-1.6
OSDIR=-D AT_WINDOWS

else

INCDIR=/usr/include/fox-1.6
OSDIR=-U AT_WINDOWS

endif

all:
	$(CXX) $(TG).cc -o $(TG) $(CFLAGS) $(OSDIR) $(LDFLAGS) -I $(INCDIR)
clean:
	$(RM) $(TG) *.o
