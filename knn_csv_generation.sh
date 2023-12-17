#!/bin/bash

# Default values
start_k_default=1
end_k_default=20
increment_default=1
p_value_default=2
input_directory_default="./assets/E34/E34"
output_directory_default="./result_data/knn/csv_results"
extension_default=".E34"

# Parse command line arguments
while getopts s:e:n:p:i:o:x:h flag
do
    case "${flag}" in
        s) start_k=${OPTARG};;
        e) end_k=${OPTARG};;
        n) increment=${OPTARG};;
        p) p_value=${OPTARG};;
        i) input_directory=${OPTARG};;
        o) output_directory=${OPTARG};;
        x) extension=${OPTARG};;
        h) echo "Usage: $0 [-s start_k] [-e end_k] [-n increment] [-p p_value] [-i input_directory] [-o output_directory] [-x extension]"
           exit;;
    esac
done

# Set values from arguments or default if not provided
start_k=${start_k:-$start_k_default}
end_k=${end_k:-$end_k_default}
increment=${increment:-$increment_default}
p_value=${p_value:-$p_value_default}
input_directory=${input_directory:-$input_directory_default}
output_directory=${output_directory:-$output_directory_default}
extension=${extension:-$extension_default}

# Create a string that includes parameters used for this execution
params_string="${extension}_p${p_value}_k${start_k}to${end_k}_inc${increment}"

# Create a timestamp for the output file name
timestamp=$(date +"%Y%m%d_%H%M%S")

# Output file with detailed name for uniqueness
output_file="$output_directory/${params_string}_${timestamp}.csv"


# Check if output directory exists, if not, create it
mkdir -p "$output_directory"

# Create or clear the output file
echo "k,p,Overall Precision,Overall Recall,Overall Specificity,Overall F1 Score,Overall FPR,Overall Accuracy" > "$output_file"

# Loop over the range of k values
for (( k = $start_k; k <= $end_k; k += $increment )); do
    echo "Running with k = $k"
    # Run command and store the output
    output=$(./main -d "$input_directory" -e "$extension" -f 0.8 -m knn -p $p_value -k $k -l none)

    # Extract the first occurrence of overall metrics and format them
    overall_metrics=$(echo "$output" | grep -P "Overall Precision = \K[^\n]+" -A 6 | head -n 7 | awk -F" = " '{print $2}' | paste -sd, - | sed 's/,$//')

    # Save to file
    echo "$k,$p_value,$overall_metrics" >> "$output_file"
done


echo "Execution complete. Extracted metrics saved in $output_file"
