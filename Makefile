CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lpthread -lm

OBJ = main.o data_reader.o normalization.o data_split.o standardization.o knn.o kmeans.o confusion_matrix.o cross_validation.o

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

kmeans.o: kmeans.c kmeans.h
	$(CC) $(CFLAGS) -c kmeans.c

confusion_matrix.o: confusion_matrix.c confusion_matrix.h
	$(CC) $(CFLAGS) -c confusion_matrix.c

cross_validation.o: cross_validation.c cross_validation.h
	$(CC) $(CFLAGS) -c cross_validation.c

clean:
	rm -f *.o main
