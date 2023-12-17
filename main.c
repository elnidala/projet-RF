#include "data_reader.h"
#include "data_split.h"
#include "normalization.h"
#include "standardization.h"
#include "knn.h"
#include "kmeans.h"
#include "confusion_matrix.h"
#include "cross_validation.h"
#include "kmeans_evaluation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/**
 * @struct CommandLineOptions
 * @brief Stores command line options.
 */
typedef struct {
    char *directory;            /**< Path to the directory containing data files. */
    char *extension;            /**< extension File extension of data files. */
    float trainingFraction;     /**< Fraction of data to be used for training. */
    char *method;               /**< Machine learning method to use ('knn' or 'kmeans'). */
    int p;                      /**< Distance metric parameter (used in k-NN and k-Means). */
    int k;                      /**< Number of neighbors/clusters. */
    char *preprocessing;        /**< Preprocessing method ('normalize' or 'standardize'). */
} CommandLineOptions;

// Function declarations
void runKnn(const CommandLineOptions *options);
void runKmeans(const CommandLineOptions *options);
void parseOptions(int argc, char *argv[], CommandLineOptions *options);
bool validateOptions(const CommandLineOptions *options);
void runModel(const CommandLineOptions *options);
void printUsage(const char *program_name);

int main(int argc, char *argv[]) {
    CommandLineOptions options = {0};

    // Parse command line options
    parseOptions(argc, argv, &options);

    // Validate the parsed options
    if (!validateOptions(&options)) {
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    // Run the specified model (kNN or kMeans)
    runModel(&options);

    return EXIT_SUCCESS;
}


/**
 * @brief Parses command line options and populates a CommandLineOptions struct.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @param options Pointer to CommandLineOptions to be filled.
 */
void parseOptions(int argc, char *argv[], CommandLineOptions *options) {
    int opt;
    while ((opt = getopt(argc, argv, "d:e:f:m:p:k:l:")) != -1) {
        switch (opt) {
            case 'd':
                options->directory = optarg;
                break;
            case 'e':
                options->extension = optarg;
                break;
            case 'f':
                options->trainingFraction = atof(optarg);
                break;
            case 'm':
                options->method = optarg;
                break;
            case 'p':
                options->p = atoi(optarg);
                break;
            case 'k':
                options->k = atoi(optarg);
                break;
            case 'l':
                if (optarg != NULL) {
                    options->preprocessing = optarg;
                } else {
                    options->preprocessing = "";
                }
                break;
            default:
                printUsage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }
}


/**
 * @brief Validates the parsed command line options.
 * @param options Parsed command line options.
 * @return bool True if options are valid, false otherwise.
 */
bool validateOptions(const CommandLineOptions *options) {
    if (!options->directory || !options->extension || options->trainingFraction <= 0.0 ||
        !options->method || options->p <= 0 || options->k <= 0 || !options->preprocessing) {
        return false;
    }
    return true;
}


/**
 * @brief Runs the specified machine learning model based on the options.
 * @param options Parsed and validated command line options.
 */
void runModel(const CommandLineOptions *options) {
    if (strcmp(options->method, "knn") == 0) {
        runKnn(options);
    } else if (strcmp(options->method, "kmeans") == 0) {
        runKmeans(options);
    } else {
        fprintf(stderr, "Method not supported\n");
        exit(EXIT_FAILURE);
    }
}


/**
 * @brief Prints the usage message for the program.
 * @param program_name Name of the program.
 */
void printUsage(const char *program_name) {
    fprintf(stderr, "Usage: %s -d <directory> -e <file_extension> -f <training_fraction> -m <method> -p <p-value> -k <k-value> -l <pre-processing>\n", program_name);
}


/**
 * @brief Runs the k-NN algorithm based on the provided command line options.
 * 
 * This function reads data files, pre-processes the data based on the specified method,
 * splits the data into training and test sets, computes distances for k-NN, applies k-NN classification,
 * and then calculates and prints the confusion matrix.
 *
 * @param options The CommandLineOptions containing the settings for the run.
 */
void runKnn(const CommandLineOptions *options) {
    // Read all files
    int count;
    ShapeData *shapes = readAllFiles(options->directory, options->extension, &count);

    if (!shapes) {
        fprintf(stderr, "Failed to read files\n");
        exit(EXIT_FAILURE);
    }

    // Normalize or standardize data if required
    if (strcmp(options->method, "normalize") == 0) {
        normalizeData(shapes, count, shapes->featureCount);
    } else if (strcmp(options->method, "standardize") == 0) {
        standardizeData(shapes, count, shapes->featureCount);
    }

    // Split data into training and test sets
    SplitData split = splitData(shapes, count, options->trainingFraction);

    // Precompute distances
    double **distances = precomputeDistances(split.trainingSet, split.trainingSize, split.testSet, split.testSize, shapes->featureCount, options->p);

    if (!distances) {
        fprintf(stderr, "Failed to precompute distances\n");
        exit(EXIT_FAILURE);
    }

    // Apply k-NN classification
    printf("Applying k-NN Classification (k = %d):\n", options->k);

    // Create a confusion matrix
    int classCount = 9;
    ConfusionMatrix cm = createConfusionMatrix(classCount);

    int *predictedClasses = malloc(split.testSize * sizeof(int)); // Store predicted classes

    for (int i = 0; i < split.testSize; i++) {
        int predictedClass = knnClassify(distances, i, split.trainingSet, split.trainingSize, options->k);
        int actualClass = split.testSet[i].class;
        updateConfusionMatrix(&cm, actualClass, predictedClass);
        printf("Test Sample %d predicted as class %d (Actual Class: %d)\n", i, predictedClass, split.testSet[i].class);
        //printMatrix(cm);
    }

    printDetailedConfusionMatrix(cm);

    // Free the allocated resources
    for (int i = 0; i < split.testSize; i++) {
        free(distances[i]);
    }
    free(distances);
    free(predictedClasses);
    freeConfusionMatrix(&cm);
    freeShapeData(shapes, count);
    free(split.trainingSet);
    free(split.testSet);
}



void knnModelFunction(SplitData split) {
    // Precompute distances for the current fold
    double **distances = precomputeDistances(split.trainingSet, split.trainingSize, split.testSet, split.testSize, split.trainingSet->featureCount, 2);

    if (!distances) {
        fprintf(stderr, "Failed to precompute distances\n");
        return;
    }

    int classCount = 9;
    ConfusionMatrix cm = createConfusionMatrix(classCount);

    for (int i = 0; i < split.testSize; i++) {
        // Use the existing knnClassify function that utilizes precomputed distances
        int predictedClass = knnClassify(distances, i, split.trainingSet, split.trainingSize, 5);

        updateConfusionMatrix(&cm, split.testSet[i].class, predictedClass);
    }

    printDetailedConfusionMatrix(cm);

    // Free the resources used by the confusion matrix and distances
    freeConfusionMatrix(&cm);
    for (int i = 0; i < split.testSize; i++) {
        free(distances[i]);
    }
    free(distances);
}

/**
 * @brief Runs the k-Means clustering algorithm based on the provided command line options.
 * 
 * This function reads data files, preprocesses the data, applies k-Means clustering,
 * prints the clustering results, evaluates the clustering performance using different metrics,
 * and frees allocated resources.
 *
 * @param options The CommandLineOptions containing the settings for the run.
 */
void runKmeans(const CommandLineOptions *options) {
    int count;
    ShapeData *shapes = readAllFiles(options->directory, options->extension, &count);
    if (!shapes) {
        fprintf(stderr, "Failed to read files\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(options->preprocessing, "normalize") == 0) {
        normalizeData(shapes, count, shapes->featureCount);
    } else if (strcmp(options->preprocessing, "standardize") == 0) {
        standardizeData(shapes, count, shapes->featureCount);
    }

    int maxIterations = 100; 
    Cluster *clusters = kmeans(shapes, count, options->k, options->p, shapes->featureCount, maxIterations);

    if (!clusters) {
        fprintf(stderr, "Failed to perform k-means clustering\n");
        exit(EXIT_FAILURE);
    }

    // Printing the classes of points in each cluster
    printf("k-Means Clustering Results (k = %d):\n", options->k);
    for (int i = 0; i < options->k; i++) {
        printf("Cluster %d:\n", clusters[i].clusterClass);
        printf("  Classes in Cluster:\n");
        for (int j = 0; j < clusters[i].size; j++) {
            printf("    Point %d: Class %d\n", j + 1, clusters[i].points[j].class);
        }
        printf("\n");
    }

     // Calculate the global centroid for BCSS
    ShapeData globalCentroid = calculateGlobalCentroid(shapes, count, shapes->featureCount);

    // Evaluate the clustering
    double silhouette = silhouetteScore(clusters, options->k, shapes->featureCount);
    double wcss = withinClusterSumOfSquares(clusters, options->k, shapes->featureCount);
    double bcss = betweenClusterSumOfSquares(clusters, options->k, shapes->featureCount, &globalCentroid, count);

    printf("Silhouette Score: %f\n", silhouette);
    printf("Within-Cluster Sum of Squares: %f\n", wcss);
    printf("Between-Cluster Sum of Squares: %f\n", bcss);

    // Free resources
    free(globalCentroid.features);

    // Free resources
    for (int i = 0; i < options->k; i++) {
        free(clusters[i].centroid->features);
        free(clusters[i].centroid);
        free(clusters[i].points);
    }
    free(clusters);
    freeShapeData(shapes, count);
}