#include "data_split.h"

// Internal function to shuffle an array of ShapeData.
// This function randomizes the order of elements in the array.
static void shuffleData(ShapeData *shapes, int size) {
    srand(time(NULL)); // Initialize random number generator.
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        ShapeData temp = shapes[i];
        shapes[i] = shapes[j];
        shapes[j] = temp;
    }
}

// Function to split the data into training and test sets.
// This function first shuffles the data to ensure a random distribution.
SplitData splitData(ShapeData *shapes, int totalSize, float trainingFraction) {
    shuffleData(shapes, totalSize); // Ensure random distribution.

    SplitData split = {NULL, 0, NULL, 0};

    int trainingSize = (int)(totalSize * trainingFraction);
    int testSize = totalSize - trainingSize;

    // Allocate memory for the training set.
    split.trainingSet = (ShapeData *)malloc(trainingSize * sizeof(ShapeData));
    if (split.trainingSet == NULL) {
        perror("Memory allocation failed for trainingSet array");
        return split; // Return with NULL pointers on allocation failure.
    }

    // Allocate memory for the test set.
    split.testSet = (ShapeData *)malloc(testSize * sizeof(ShapeData));
    if (split.testSet == NULL) {
        free(split.trainingSet); // Free previously allocated memory.
        split.trainingSet = NULL;
        perror("Memory allocation failed tstSet array");
        return split; // Return with NULL pointers on allocation failure.
    }

    // Copy data to training and test sets.
    for (int i = 0; i < trainingSize; i++) {
        split.trainingSet[i] = shapes[i];
    }
    for (int i = 0; i < testSize; i++) {
        split.testSet[i] = shapes[i + trainingSize];
    }

    split.trainingSize = trainingSize;
    split.testSize = testSize;

    return split;
}
