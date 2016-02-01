cc=gcc
flags= -g -Wall

exec = ep
objs = utils.o linkedList.o node.o pixel.o ponto.o estatistics.o ep.o
headers = utils.h linkedList.h globals.h pixel.h ponto.h node.h estatistics.h

ep: $(objs)
	$(cc) -o $@ $^ -fopenmp -lm -lallegro -lallegro_primitives
	-rm -f *.gch *.o
	@echo "\nCompilou!"

ep.o: ep.c $(headers)
	$(cc) -c $^ $(flags) -fopenmp -lm

estatistics.o: estatistics.c estatistics.h globals.h
	$(cc) -c $^ $(flags) -fopenmp -lm

node.o: node.c utils.h node.h linkedList.h globals.h
	$(cc) -c $^ $(flags) -lm

linkedList.o: linkedList.c utils.h linkedList.h
	$(cc) -c $^ $(flags)

pixel.o: pixel.c pixel.h utils.h globals.h 
	$(cc) -c $^ $(flags) -fopenmp -lm

ponto.o: ponto.c utils.h ponto.h globals.h
	$(cc) -c $^ $(flags) -lm

utils.o: utils.c utils.h
	$(cc) -c $^ $(flags)

clean:
	-rm -f $(exec) $(objs) *~ core* *.gch