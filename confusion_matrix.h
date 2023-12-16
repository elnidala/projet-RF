#ifndef CONFUSION_MATRIX_H
#define CONFUSION_MATRIX_H

#include "data_reader.h" // Include for ShapeData structure definition.

typedef struct {
    int **matrix;     // 2D array representing the confusion matrix.
    int classCount;   // Number of classes.
} ConfusionMatrix;

ConfusionMatrix createConfusionMatrix(int classCount);
void updateConfusionMatrix(ConfusionMatrix *cm, int actual, int predicted);
void freeConfusionMatrix(ConfusionMatrix *cm);
void printDetailedConfusionMatrix(const ConfusionMatrix cm);
void saveDetailedConfusionMatrixToFile(const ConfusionMatrix cm, const char *filename, const char *title);

#endif // CONFUSION_MATRIX_H
