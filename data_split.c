#include "data_split.h"
#include <stdlib.h>
#include <time.h>

// Internal helper function to shuffle an array of ShapeData.
// Parameters:
//   shapes: Pointer to the array of ShapeData to be shuffled.
//   size: Number of elements in the shapes array.
void shuffleData(ShapeData *shapes, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1); // Generate a random index.
        ShapeData temp = shapes[i];
        shapes[i] = shapes[j];    // Swap elements at indices i and j.
        shapes[j] = temp;
    }
}

// Splits the data into training and test sets based on a specified fraction.
// The function shuffles the data before splitting.
SplitData splitData(ShapeData *shapes, int totalSize, float trainingFraction) {
    srand(time(NULL)); // Seed the random number generator.

    shuffleData(shapes, totalSize); // Shuffle the data to randomize the distribution.

    SplitData split;
    // Calculate sizes of training and test sets.
    split.trainingSize = (int)(totalSize * trainingFraction);
    split.testSize = totalSize - split.trainingSize;

    // Allocate memory for the training and test sets.
    split.trainingSet = (ShapeData *)malloc(split.trainingSize * sizeof(ShapeData));
    split.testSet = (ShapeData *)malloc(split.testSize * sizeof(ShapeData));

    // Copy the shuffled data to the training and test sets.
    for (int i = 0; i < split.trainingSize; i++) {
        split.trainingSet[i] = shapes[i];
    }
    for (int i = 0; i < split.testSize; i++) {
        split.testSet[i] = shapes[i + split.trainingSize];
    }

    return split; // Return the SplitData structure.
}
