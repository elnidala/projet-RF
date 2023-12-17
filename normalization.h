
/**
 * @file normalization.h
 * @brief Header file for normalizing feature values in ShapeData.
 */

#ifndef NORMALIZATION_H
#define NORMALIZATION_H

#include "data_reader.h"  // Include to use the ShapeData structure
#include <pthread.h>      // Include pthread library for multi-threading

// Structure to hold arguments for the thread function findMinMaxThread.
typedef struct {
    ShapeData *data;      /**< Pointer to the array of ShapeData. */
    int startIdx;         /**< Starting index of the data segment for this thread. */
    int endIdx;           /**< Ending index of the data segment. */
    double *min;          /**< Array to store minimum values found by this thread. */
    double *max;          /**< Array to store maximum values found by this thread. */
    int featureCount;     /**< Number of features in each ShapeData item. */
} ThreadArgs;

/**
 * @brief Normalizes the feature values in an array of ShapeData.
 *
 * Normalization scales the data to a specific range, typically [0, 1].
 *
 * @param data Pointer to the array of ShapeData.
 * @param dataSize Total number of ShapeData items.
 * @param featureCount Number of features in each ShapeData item.
 */
void normalizeData(ShapeData *data, int dataSize, int featureCount);

#endif // NORMALIZATION_H
