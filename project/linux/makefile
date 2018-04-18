alltarget: client server


client: slib
	g++ ../../src/client/main.cpp \
		../../src/client/nfClient.cpp \
     	-std=c++11 -L. -ggdb -lslib -lpthread -ldl -o client

server: slib
	g++ ../../src/server/nfServer.cpp \
     	-std=c++11 -L. -ggdb -lslib -ldl -lpthread -ldl -o server

slib:
	gcc ../../../slib/src/slib.c -c -o slibc.o
	g++ ../../../slib/src/slib.cpp -c -std=c++11 -o slibcpp.o
	ar crv libslib.a slibcpp.o slibc.o

clean:
	rm -f server
	rm -f client
	rm -f *.a
	rm -f *.o
	rm -f *.so
	rm -f *.out
	#rm -f *.db
	rm -f *.log
	rm -rf *.dSYM
