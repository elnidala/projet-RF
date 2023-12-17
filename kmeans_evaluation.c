#include "kmeans_evaluation.h"

double silhouetteScore(Cluster *clusters, int k, int featureCount) {
    double totalSilhouetteScore = 0.0;
    int totalPoints = 0;

    // Loop through each cluster
    for (int c = 0; c < k; c++) {
        // Loop through each point in the cluster
        for (int i = 0; i < clusters[c].size; i++) {
            double a = 0.0; // Average distance to points in the same cluster
            double b = DBL_MAX; // Minimum average distance to points in other clusters

            // Calculate 'a' value - the mean distance to other data points in the same cluster
            for (int j = 0; j < clusters[c].size; j++) {
                if (i != j) {
                    a += minkowskiDistance(clusters[c].points[i], clusters[c].points[j], featureCount, 2);
                }
            }
            a /= clusters[c].size - 1;

            // Calculate 'b' value - the smallest mean distance to all points in any other cluster
            for (int otherCluster = 0; otherCluster < k; otherCluster++) {
                if (otherCluster != c) {
                    double otherAvgDist = 0.0;
                    for (int j = 0; j < clusters[otherCluster].size; j++) {
                        otherAvgDist += minkowskiDistance(clusters[c].points[i], clusters[otherCluster].points[j], featureCount, 2);
                    }
                    otherAvgDist /= clusters[otherCluster].size;
                    b = fmin(b, otherAvgDist);
                }
            }

            // Update the total silhouette score
            if (clusters[c].size > 1) {
                totalSilhouetteScore += (b - a) / fmax(a, b);
                totalPoints++;
            }
        }
    }

    // Return the average silhouette score
    return totalPoints > 0 ? totalSilhouetteScore / totalPoints : 0;
}

double withinClusterSumOfSquares(Cluster *clusters, int k, int featureCount) {
    double totalWCSS = 0.0;

    // Loop through each cluster
    for (int i = 0; i < k; i++) {
        // Loop through each point in the cluster
        for (int j = 0; j < clusters[i].size; j++) {
            double distance = minkowskiDistance(*clusters[i].centroid, clusters[i].points[j], featureCount, 2);
            totalWCSS += distance * distance;
        }
    }

    // Return the total WCSS
    return totalWCSS;
}

double betweenClusterSumOfSquares(Cluster *clusters, int k, int featureCount, ShapeData *globalCentroid, int dataSize) {
    double totalBCSS = 0.0;

    // Loop through each cluster
    for (int i = 0; i < k; i++) {
        double distance = minkowskiDistance(*globalCentroid, *clusters[i].centroid, featureCount, 2);
        totalBCSS += clusters[i].size * distance * distance;
    }

    // Normalize the BCSS by the total number of data points
    totalBCSS /= dataSize;

    // Return the total BCSS
    return totalBCSS;
}

ShapeData calculateGlobalCentroid(ShapeData *shapes, int count, int featureCount) {
    ShapeData centroid;
    centroid.features = calloc(featureCount, sizeof(double));

    // Check for memory allocation failure
    if (!centroid.features) {
        fprintf(stderr, "Memory allocation failed for global centroid features.\n");
        exit(EXIT_FAILURE);
    }

    // Sum up all the features for all points
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < featureCount; j++) {
            centroid.features[j] += shapes[i].features[j];
        }
    }

    // Calculate the average for each feature
    for (int j = 0; j < featureCount; j++) {
        centroid.features[j] /= count;
    }

    // Return the calculated global centroid
    return centroid;
}
