#include "cross_validation.h"

// Perform k-fold cross-validation on a dataset using a specified model.
void crossValidation(ShapeData *data, int dataSize, int kFolds, ModelFunction modelFunc) {
    // Calculate the size of each fold
    int foldSize = dataSize / kFolds;

    // Iterate through each fold
    for (int i = 0; i < kFolds; i++) {
        // Calculate the start and end indices for the test set in this fold
        int testStart = i * foldSize;
        int testEnd = testStart + foldSize;

        // Create a structure to hold the training and test sets
        SplitData split;
        split.trainingSet = malloc((dataSize - foldSize) * sizeof(ShapeData));
        split.testSet = malloc(foldSize * sizeof(ShapeData));
        split.trainingSize = 0;
        split.testSize = 0;

        // Populate training and test sets
        for (int j = 0; j < dataSize; j++) {
            if (j >= testStart && j < testEnd) {
                // Add the data point to the test set
                split.testSet[split.testSize++] = data[j];
            } else {
                // Add the data point to the training set
                split.trainingSet[split.trainingSize++] = data[j];
            }
        }

        // Call the model function to evaluate the model on the current fold
        modelFunc(split);

        // Free memory allocated for the training and test sets
        free(split.trainingSet);
        free(split.testSet);
    }
}