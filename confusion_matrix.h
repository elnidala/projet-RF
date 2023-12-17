/**
 * @file confusion_matrix.h
 * @brief Header file for confusion matrix operations.
 */

#ifndef CONFUSION_MATRIX_H
#define CONFUSION_MATRIX_H

#include "data_reader.h" // Include for ShapeData structure definition.
#include <time.h>


/**
 * @struct ConfusionMatrix
 * @brief Represents a confusion matrix for classification results.
 */
typedef struct {
    int **matrix;     /**< 2D array representing the confusion matrix. */
    int classCount;   /**< Number of classes. */
} ConfusionMatrix;

/**
 * @struct ClassMetrics
 * @brief Metrics for evaluating classification performance for a single class.
 */
typedef struct {
    double precision;       /**< Precision of the class. */
    double recall;          /**< Recall of the class. */
    double f1Score;         /**< F1 Score of the class. */
    double accuracy;        /**< Accuracy of the class. */
} ClassMetrics;


/**
 * @struct ConfusionMatrixMetrics
 * @brief Container for metrics calculated from a confusion matrix.
 */
typedef struct {
    ClassMetrics *classMetrics;  /**< Array of metrics for each class. */
    ClassMetrics overallMetrics; /**< Overall metrics computed from the entire confusion matrix. */
    int classCount;              /**< Number of classes. */
} ConfusionMatrixMetrics;


/**
 * @brief Creates and initializes a confusion matrix.
 * 
 * @param classCount The number of classes.
 * @return ConfusionMatrix An initialized confusion matrix.
 */
ConfusionMatrix createConfusionMatrix(int classCount);

/**
 * @brief Updates the confusion matrix with new classification results.
 * 
 * @param cm Pointer to the confusion matrix.
 * @param actualClass The actual class of the sample.
 * @param predictedClass The predicted class of the sample.
 */
void updateConfusionMatrix(ConfusionMatrix *cm, int actual, int predicted);

/**
 * @brief Frees the memory allocated for the confusion matrix.
 * 
 * @param cm Pointer to the confusion matrix to be freed.
 */
void freeConfusionMatrix(ConfusionMatrix *cm);

/**
 * @brief Prints the confusion matrix and its statistics to standard output.
 * 
 * @param cm The confusion matrix to be printed.
 */
void printDetailedConfusionMatrix(const ConfusionMatrix cm);

void saveDetailedConfusionMatrixToFile(const ConfusionMatrix cm, const char *filename, const char *title);
ConfusionMatrixMetrics calculateStatistics(const ConfusionMatrix *cm);
void printStatistics2(const ConfusionMatrixMetrics *metrics);

#endif // CONFUSION_MATRIX_H
