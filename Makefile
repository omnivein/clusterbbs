CFLAGS= -Wall
OBJ=main.o clusterbs.o pseudor.o clustering.o

all: main.exe

%.o: %.cxx %.hxx
	@clang++ -c -o $@ $< $(CFLAGS)

main.exe: $(OBJ)
	@clang++ -o $@ $^ $(CFLAGS)

clean:
	@rm -rf $(OBJ) main.exe