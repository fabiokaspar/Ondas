cc=gcc
flags= -g -Wall
#flags= -g -W -Wall -ansi -pedantic

exec = ep
objs = ep.o utils.o
	
ep: $(objs) -lm
	$(cc) -o $@ $^
	@echo "\nCompilou!"

%.o: %.c utils.h
	$(cc) -c $^ $(flags)

clean:
	-rm -f $(exec) $(objs) *~ core*