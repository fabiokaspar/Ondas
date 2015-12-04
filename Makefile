cc=gcc
flags= -g -Wall
#flags= -g -W -Wall -ansi -pedantic

exec = ep
objs = ep.o utils.o linkedList.o lago.o pixel.o ponto.o 
headers = utils.h linkedList.h globals.h pixel.h ponto.h lago.h

ep: $(objs) -lm
	$(cc) -o $@ $^ -fopenmp
	@echo "\nCompilou!"

%.o: %.c $(headers)
	$(cc) -c $^ $(flags)

clean:
	-rm -f $(exec) $(objs) *~ core* *.gch