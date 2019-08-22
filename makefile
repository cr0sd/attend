TG=f
CXX=c++
CFLAGS=-Wfatal-errors -Wall -Wextra -I /usr/include/fox-1.6
LDFLAGS=-lFOX-1.6 -lsqlite3
RM=@rm -f 
all:
	$(CXX) $(TG).cc -o $(TG) $(CFLAGS) $(LDFLAGS)
clean:
	$(RM) $(TG) *.o
