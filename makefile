TG=attend
CXX=c++
CFLAGS=-Wfatal-errors -Wall -Wextra -I /usr/include/fox-1.6
LDFLAGS=-lFOX-1.6 -lsqlite3
RM=@rm -f

# determine OS
ifeq ($(OS),Windows_NT)
	DIR=C:\Program Files\attend
else
	DIR=/usr/bin
endif

all:
	$(CXX) $(TG).cc -o $(TG) $(CFLAGS) $(LDFLAGS)
clean:
	$(RM) $(TG) *.o
