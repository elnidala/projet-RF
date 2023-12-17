
/**
 * @file cross_validation.h
 * @brief Header file for implementing k-fold cross-validation with model evaluation metrics.
 */

#ifndef CROSS_VALIDATION_H
#define CROSS_VALIDATION_H

#include "data_split.h"  // Includes ShapeData structure definition
#include "confusion_matrix.h" // Includes ConfusionMatrix definition

/**
 * @typedef ModelFunction
 * @brief Typedef for a function pointer representing a model's evaluation function.
 * @param split The data split containing training and test sets.
 * @return ConfusionMatrix representing the model's performance.
 */
typedef ConfusionMatrix (*ModelFunction)(SplitData split);

/**
 * @struct CrossValidationMetrics
 * @brief Structure to hold aggregated metrics from k-fold cross-validation.
 */
typedef struct {
    double totalAccuracy;  /**< Total accuracy across all folds. */
    double totalPrecision; /**< Total precision across all folds. */
    double totalRecall;    /**< Total recall across all folds. */
    double totalF1Score;   /**< Total F1 score across all folds. */
    int foldsProcessed;    /**< Number of folds processed. */
} CrossValidationMetrics;


/**
 * @brief Perform k-fold cross-validation on a dataset using a specified model.
 *
 * This function divides the dataset into k folds, trains the model on k-1 folds,
 * and evaluates its performance on the remaining fold. This process is repeated k times
 * to ensure each fold serves as the test set exactly once.
 *
 * @param data Pointer to the dataset.
 * @param dataSize Number of elements in the dataset.
 * @param kFolds Number of folds for cross-validation.
 * @param modelFunc Function pointer to the model's evaluation function.
 */
void crossValidation(ShapeData *data, int dataSize, int kFolds, ModelFunction modelFunc);

/**
 * @brief Model function for k-Nearest Neighbors (knn) algorithm.
 * @param split The data split containing training and test sets.
 */
void knnModelFunction(SplitData split);

#endif // CROSS_VALIDATION_H
