network.exe: train_test.o neural_network.o
	g++ -o network.exe train_test.o neural_network.o

train_test.o: train_test.cpp neural_network.h
	g++ -c train_test.cpp

neural_network.o: neural_network.cpp neural_network.h
	g++ -c neural_network.cpp

debug:
	g++ -g -o train_testDebug.exe train_test.cpp neural_network.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups
