cc=gcc
flags= -g -W -Wall -ansi -pedantic

exec = ep
objs = ep.o

all: $(exec)
	
ep: $(objs)
	$(cc) -o $@ $^
	@echo "\nCompilou!"

%.o: %.c 
	$(cc) -c $^ $(debugflags) $(flags)

clean:
	-rm -f $(exec) $(objs) *~ core*