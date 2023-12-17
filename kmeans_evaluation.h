/**
 * @file kmeans_evaluation.h
 * Header file for K-means clustering evaluation functions.
 */

#ifndef KMEANS_EVALUATION_H
#define KMEANS_EVALUATION_H

#include "kmeans.h"
#include "knn.h"

/**
 * @brief Calculates the silhouette score for clustering.
 * 
 * @param clusters Pointer to an array of Cluster objects.
 * @param k Number of clusters.
 * @param featureCount Number of features in each data point.
 * @return double The average silhouette score of all clusters.
 */
double silhouetteScore(Cluster *clusters, int k, int featureCount);

/**
 * @brief Computes the within-cluster sum of squares.
 * 
 * @param clusters Pointer to an array of Cluster objects.
 * @param k Number of clusters.
 * @param featureCount Number of features in each data point.
 * @return double The total within-cluster sum of squares.
 */
double withinClusterSumOfSquares(Cluster *clusters, int k, int featureCount);

/**
 * @brief Calculates the between-cluster sum of squares.
 * 
 * @param clusters Pointer to an array of Cluster objects.
 * @param k Number of clusters.
 * @param featureCount Number of features in each data point.
 * @param globalCentroid Pointer to the global centroid.
 * @param dataSize Total number of data points.
 * @return double The total between-cluster sum of squares.
 */
double betweenClusterSumOfSquares(Cluster *clusters, int k, int featureCount, ShapeData *globalCentroid, int dataSize);

/**
 * @brief Calculates the global centroid of all data points.
 * 
 * @param shapes Pointer to an array of ShapeData objects.
 * @param count Number of data points.
 * @param featureCount Number of features in each data point.
 * @return ShapeData The global centroid of all data points.
 */
ShapeData calculateGlobalCentroid(ShapeData *shapes, int count, int featureCount);

#endif // KMEANS_EVALUATION_H
