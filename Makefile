CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lpthread -lm

OBJ = main.o data_reader.o normalization.o data_split.o standardization.o knn.o

all: main

main: $(OBJ)
	$(CC) -o main $(OBJ) $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

data_reader.o: data_reader.c data_reader.h
	$(CC) $(CFLAGS) -c data_reader.c

normalization.o: normalization.c normalization.h
	$(CC) $(CFLAGS) -c normalization.c

data_split.o: data_split.c data_split.h
	$(CC) $(CFLAGS) -c data_split.c

standardization.o: standardization.c standardization.h
	$(CC) $(CFLAGS) -c standardization.c

knn.o: knn.c knn.h
	$(CC) $(CFLAGS) -c knn.c

clean:
	rm -f *.o main
