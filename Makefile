play.exe: checkers.o game.o
	g++ -o play.exe checkers.o game.o

checkers.o: checkers.cpp game.h
	g++ -c checkers.cpp

game.o: game.cpp game.h
	g++ -c game.cpp

debug:
	g++ -g -o checkersDebug.exe checkers.cpp game.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups
