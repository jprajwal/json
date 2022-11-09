CC = clang++

BLDCMD = $(CC) --std=c++17

test: json.o string.o object.o
	$(BLDCMD) json.o string.o object.o test.cpp -o test

json.o: string.o object.o
	$(BLDCMD) -c json.cpp

string.o:
	$(BLDCMD) -c string.cpp

object.o:
	$(BLDCMD) -c object.cpp

clean:
	rm *.o