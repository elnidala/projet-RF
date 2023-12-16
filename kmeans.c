#include "kmeans.h"

// Private helper functions declarations
static ShapeData* copyShapeData(const ShapeData *src, int featureCount);
static bool isIndexSelected(const int *selectedIndices, int k, int index);
static void initializeCentroids(Cluster *clusters, const ShapeData *trainingSet, int k, int featureCount, int trainingSize);
static void assignPointsToClusters(Cluster *clusters, const ShapeData *trainingSet, int trainingSize, int k, int featureCount, int p);
static void updateCentroids(Cluster *clusters, int k, int featureCount);
static void freeClusterPoints(Cluster *clusters, int k);
static bool isCentroidSimilar(ShapeData *centroid1, ShapeData *centroid2, int featureCount);
static bool areCentroidsConverged(Cluster *clusters, Cluster *prevClusters, int k, int featureCount);


/**
 * @brief Copies ShapeData structure.
 *
 * Allocates memory and creates a copy of a given ShapeData structure.
 * This is useful for creating independent copies of centroids.
 * 
 * @param src Source ShapeData to copy.
 * @param featureCount Number of features in ShapeData.
 * @return ShapeData* Pointer to the newly allocated copy of ShapeData.
 */
static ShapeData* copyShapeData(const ShapeData *src, int featureCount) {
    // Allocate memory for the ShapeData structure
    ShapeData *dest = malloc(sizeof(ShapeData));
    if (!dest) {
        return NULL; // Return NULL if memory allocation fails
    }

    // Allocate memory for the features array within ShapeData
    dest->features = malloc(featureCount * sizeof(double));
    if (!dest->features) {
        free(dest); // Free the ShapeData structure if feature allocation fails
        return NULL;
    }

    // Copy the feature values from the source to the newly allocated destination
    memcpy(dest->features, src->features, featureCount * sizeof(double));
    return dest;
}

/**
 * @brief Checks for index in selected indices array.
 *
 * Used in centroid initialization to avoid duplicate selections.
 * 
 * @param selectedIndices Array of selected indices.
 * @param k Number of selected indices.
 * @param index Index to check.
 * @return true if the index is in the list, false otherwise.
 */
static bool isIndexSelected(const int *selectedIndices, int k, int index) {
    for (int i = 0; i < k; i++) {
        if (selectedIndices[i] == index) {
            return true; // Return true if index is found in the array
        }
    }
    return false; // Return false if index is not found
}

/**
 * @brief Initializes centroids for k-means clustering.
 *
 * Randomly selects unique data points from the training set to serve as initial centroids.
 * Ensures that each centroid is unique to provide a diverse starting point for clustering.
 * 
 * @param clusters Array of clusters to initialize.
 * @param trainingSet Array of training data.
 * @param k Number of clusters.
 * @param featureCount Number of features in each ShapeData item.
 * @param trainingSize Size of the training set.
 */
static void initializeCentroids(Cluster *clusters, const ShapeData *trainingSet, int k, int featureCount, int trainingSize) {
    // Allocate an array to keep track of selected indices for centroids
    int *selectedIndices = malloc(k * sizeof(int));
    if (!selectedIndices) {
        return; // Return if memory allocation fails
    }

    // Initialize all indices in the array to -1
    for (int i = 0; i < k; i++) {
        selectedIndices[i] = -1;
    }

    // Randomly select unique indices for initial centroids
    for (int correctIndex = 0; correctIndex < k; ) {
        int index = rand() % trainingSize;
        if (!isIndexSelected(selectedIndices, k, index)) {
            selectedIndices[correctIndex++] = index;
            clusters[correctIndex - 1].centroid = copyShapeData(&trainingSet[index], featureCount);
        }
    }

    free(selectedIndices); // Free the memory used for tracking selected indices
}

/**
 * @brief Assigns each data point to the nearest cluster.
 *
 * Computes the distance between each data point and each centroid,
 * assigning the point to the cluster with the closest centroid.
 * 
 * @param clusters Array of clusters.
 * @param trainingSet Array of training data.
 * @param trainingSize Size of the training set.
 * @param k Number of clusters.
 * @param featureCount Number of features in each ShapeData item.
 */
static void assignPointsToClusters(Cluster *clusters, const ShapeData *trainingSet, int trainingSize, int k, int featureCount, int p) {
    for (int i = 0; i < trainingSize; i++) {
        double minDistance = DBL_MAX;
        int closestCluster = 0;

        // Determine the closest cluster for each point
        for (int j = 0; j < k; j++) {
            double distance = minkowskiDistance(*(clusters[j].centroid), trainingSet[i], featureCount, p); 
            if (distance < minDistance) {
                minDistance = distance;
                closestCluster = j;
            }
        }

        // Assign the point to the closest cluster
        clusters[closestCluster].points = realloc(clusters[closestCluster].points, (clusters[closestCluster].size + 1) * sizeof(ShapeData));
        if (clusters[closestCluster].points) {
            clusters[closestCluster].points[clusters[closestCluster].size] = trainingSet[i];
            clusters[closestCluster].size++;
        }
    }
}


/**
 * @brief Updates centroids of each cluster.
 *
 * Recalculates the centroid of each cluster to be the mean of all points assigned to it.
 * This step is essential for the iterative improvement of cluster assignments.
 * 
 * @param clusters Array of clusters.
 * @param k Number of clusters.
 * @param featureCount Number of features in each ShapeData item.
 */
static void updateCentroids(Cluster *clusters, int k, int featureCount) {
    for (int i = 0; i < k; i++) {
        if (clusters[i].size == 0) continue;

        double *newCentroid = calloc(featureCount, sizeof(double));
        if (!newCentroid) {
            continue; // Skip centroid update if memory allocation fails
        }

        // Calculate the mean of all points in the cluster
        for (int j = 0; j < clusters[i].size; j++) {
            for (int f = 0; f < featureCount; f++) {
                newCentroid[f] += clusters[i].points[j].features[f];
            }
        }

        for (int f = 0; f < featureCount; f++) {
            clusters[i].centroid->features[f] = newCentroid[f] / clusters[i].size;
        }

        free(newCentroid); // Free the memory allocated for the new centroid
    }
}

/**
 * @brief Frees memory allocated for points in each cluster.
 *
 * This is a crucial step to prevent memory leaks during the reassignment of points
 * to clusters in each iteration of the k-means algorithm.
 * 
 * @param clusters Array of clusters.
 * @param k Number of clusters.
 */
static void freeClusterPoints(Cluster *clusters, int k) {
    for (int i = 0; i < k; i++) {
        free(clusters[i].points); // Free the memory allocated for points in each cluster
        clusters[i].points = NULL;
        clusters[i].size = 0;
    }
}

// Function to check if two centroids are similar
static bool isCentroidSimilar(ShapeData *centroid1, ShapeData *centroid2, int featureCount) {
    double distance = 0.0;
    for (int i = 0; i < featureCount; i++) {
        distance += pow(centroid1->features[i] - centroid2->features[i], 2);
    }
    return sqrt(distance) < CONVERGENCE_THRESHOLD;
}

// Function to check if all centroids are similar
static bool areCentroidsConverged(Cluster *clusters, Cluster *prevClusters, int k, int featureCount) {
    for (int i = 0; i < k; i++) {
        if (!isCentroidSimilar(clusters[i].centroid, prevClusters[i].centroid, featureCount)) {
            return false;
        }
    }
    return true;
}


/**
 * Iteratively performs clustering by assigning points to the nearest centroid
 * and updating centroids until the maximum number of iterations is reached.
 */
/**
 * Iteratively performs clustering by assigning points to the nearest centroid
 * and updating centroids until the maximum number of iterations is reached.
 */
Cluster* kmeans(ShapeData *trainingSet, int trainingSize, int k, int p, int featureCount, int maxIterations) {
    // Validate input parameters
    if (!trainingSet || trainingSize <= 0 || k <= 0 || featureCount <= 0 || maxIterations <= 0) {
        fprintf(stderr, "Invalid input parameters to kmeans function\n");
        return NULL;
    }

    // Allocate memory for clusters and prevClusters
    Cluster *clusters = calloc(k, sizeof(Cluster));
    Cluster *prevClusters = calloc(k, sizeof(Cluster));

    if (!clusters || !prevClusters) {
        fprintf(stderr, "Memory allocation failure for clusters\n");
        free(clusters); // Free in case one allocation succeeded but the other failed
        free(prevClusters);
        return NULL;
    }

    // Initialize each Cluster in clusters and prevClusters
    for (int i = 0; i < k; i++) {
        clusters[i].points = NULL;
        clusters[i].size = 0;
        clusters[i].centroid = calloc(1,sizeof(ShapeData)); // Allocate memory for centroid
        if (clusters[i].centroid) {
            clusters[i].centroid->features = calloc(featureCount, sizeof(double)); // Allocate memory for features
            if (!clusters[i].centroid->features) {
                printf("ERROR/n");
            }
        }

        prevClusters[i].points = NULL;
        prevClusters[i].size = 0;
        prevClusters[i].centroid = calloc(1, sizeof(ShapeData)); // Allocate memory for centroid
        if (prevClusters[i].centroid) {
            prevClusters[i].centroid->features = calloc(featureCount, sizeof(double)); // Allocate memory for features
            if (!prevClusters[i].centroid->features) {
                printf("ERROR/n");
            }
        }
    }

    initializeCentroids(clusters, trainingSet, k, featureCount, trainingSize);

    // Main k-means clustering loop
    bool converged = false;
    for (int iteration = 0; iteration < maxIterations && !converged; iteration++) {
        // Store previous centroids
        for (int i = 0; i < k; i++) {
            if (prevClusters[i].centroid) {
                free(prevClusters[i].centroid->features);
                free(prevClusters[i].centroid);
            }
            prevClusters[i].centroid = copyShapeData(clusters[i].centroid, featureCount);
        }

        freeClusterPoints(clusters, k);
        assignPointsToClusters(clusters, trainingSet, trainingSize, k, featureCount, p);
        updateCentroids(clusters, k, featureCount);

        // Check for convergence
        if (iteration > 0 && areCentroidsConverged(clusters, prevClusters, k, featureCount)) {
            converged = true;
        }
    }

    // Free previous clusters
    for (int i = 0; i < k; i++) {
        if (prevClusters[i].centroid) {
            free(prevClusters[i].centroid->features);
            free(prevClusters[i].centroid);
        }
    }
    free(prevClusters);

    // Return the result, but remember that the caller is responsible for freeing this memory
    return clusters;
}
