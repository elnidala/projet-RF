CC = gcc
CFLAGS = -Wall -O2 -fopenmp
LDFLAGS = -lpthread -lm -fopenmp

# List of source files
SRCS = main.c data_reader.c normalization.c data_split.c standardization.c \
       knn.c kmeans.c confusion_matrix.c cross_validation.c kmeans_evaluation.c

# Corresponding object files
OBJS = $(SRCS:.c=.o)

# Target executable
TARGET = main

# Default target
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compiling source files
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Generate documentation
doc:
	doxygen Doxyfile

# Phony targets for non-file commands
.PHONY: all clean doc
