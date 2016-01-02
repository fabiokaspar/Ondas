cc=gcc
flags= -g -Wall

exec = ep
objs = utils.o linkedList.o lago.o pixel.o ponto.o estatistics.o ep.o
headers = utils.h linkedList.h globals.h pixel.h ponto.h lago.h estatistics.h

ep: $(objs)
	$(cc) -o $@ $^ -fopenmp -lm -lallegro -lallegro_primitives -lallegro_image
	-rm -f *.gch *.o
	@echo "\nCompilou!"

ep.o: ep.c $(headers)
	$(cc) -c $^ $(flags) -fopenmp -lm

estatistics.o: estatistics.c estatistics.h globals.h
	$(cc) -c $^ $(flags) -fopenmp -lm

lago.o: lago.c utils.h lago.h linkedList.h globals.h
	$(cc) -c $^ $(flags) -lm

linkedList.o: linkedList.c utils.h linkedList.h
	$(cc) -c $^ $(flags)

pixel.o: pixel.c pixel.h utils.h globals.h 
	$(cc) -c $^ $(flags) -lm

ponto.o: ponto.c utils.h ponto.h globals.h
	$(cc) -c $^ $(flags) -lm

utils.o: utils.c utils.h
	$(cc) -c $^ $(flags)

clean:
	-rm -f $(exec) $(objs) *~ core* *.gch