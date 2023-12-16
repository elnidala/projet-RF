#ifndef KMEANS_H
#define KMEANS_H

// Define a small threshold for convergence check
#define CONVERGENCE_THRESHOLD 0.001

#include "data_reader.h"
#include "knn.h"

#include <stdbool.h>
#include <float.h>


/**
 * Structure to represent a cluster. 
 */
typedef struct {
    ShapeData *centroid;   /**< The centroid of the cluster. */
    int size;              /**< The number of elements in the cluster. */
    ShapeData *points;     /**< Array of points belonging to this cluster. */
    int clusterClass;      /**< Added field to store the class of the cluster. */
} Cluster;

/**
 * Performs k-means clustering on the given dataset.
 * @param trainingSet Array of ShapeData to cluster.
 * @param trainingSize Number of elements in data.
 * @param k Number of clusters.
 * @param featureCount Number of features in each ShapeData item.
 * @param maxIterations Maximum number of iterations for the k-means algorithm.
 * @return Array of Cluster structures representing the k clusters.
 */
Cluster* kmeans(ShapeData *trainingSet, int trainingSize, int k, int p, int featureCount, int maxIterations);

#endif // KMEANS_H
