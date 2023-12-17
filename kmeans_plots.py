import pandas as pd
import matplotlib.pyplot as plt
import sys
import os

# Load the data
txt_file_path = sys.argv[1]
output_dir = './result_data/kmeans/plots'

base_name = os.path.basename(txt_file_path)
output_file, _ = os.path.splitext(base_name)

# Read data from the text file
try:
    df = pd.read_csv(txt_file_path, sep=',')
    df['k'] = df['k'].astype(int)  # Convert 'k' column to integer
    print("Data read successfully. Here's a preview:")
    print(df.head())  # Print the first few rows of the DataFrame
except FileNotFoundError:
    print(f"File not found: {txt_file_path}")
    exit()
except Exception as e:
    print(f"An error occurred: {e}")
    exit()

# Plotting the data
plt.figure(figsize=(10, 6))

metrics = ["Silhouette Score", "WCSS", "BCSS"]
for i, metric in enumerate(metrics, 1):
    plt.subplot(2, 3, i)
    plt.plot(df["k"], df[metric], marker='o')
    plt.title(metric)
    plt.xlabel('k')
    plt.ylabel(metric)

    # Setting x-axis to show integer values
    plt.xticks(df["k"].unique())  # This ensures only the unique 'k' values are used as x-ticks

    plt.tight_layout()

# Save the figure before showing it
output_path = os.path.join(output_dir, output_file + '.png')
plt.savefig(output_path)

plt.show()
