train_network.exe: train.o neural_network.o
	g++ -o train_network.exe train.o neural_network.o

train.o: train.cpp neural_network.h
	g++ -c train.cpp

neural_network.o: neural_network.cpp neural_network.h
	g++ -c neural_network.cpp

debug:
	g++ -g -o trainDebug.exe train.cpp neural_network.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups
