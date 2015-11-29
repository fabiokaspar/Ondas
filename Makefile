cc=gcc
flags= -g -Wall
#flags= -g -W -Wall -ansi -pedantic

exec = ep
objs = ep.o utils.o linkedList.o
	
ep: $(objs) -lm
	$(cc) -o $@ $^
	@echo "\nCompilou!"

utils.o: utils.c utils.h 
	$(cc) -c $^ $(flags)
	
%.o: %.c utils.h linkedList.h
	$(cc) -c $^ $(flags)

clean:
	-rm -f $(exec) $(objs) *~ core* *.gch