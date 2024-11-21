import clusterbox  # Import the Python wrapper for the C++/CUDA module
import numpy as np
import matplotlib.pyplot as plt
import os

# Ensure output directory exists
os.makedirs("out", exist_ok=True)

def generate_test_data():
    """Generate synthetic data for testing using numpy."""
    from sklearn.datasets import make_blobs, make_moons
    data1, _ = make_blobs(n_samples=300, centers=4, cluster_std=0.6, random_state=42)
    data2, _ = make_moons(n_samples=300, noise=0.05, random_state=42)
    return {"blobs": data1, "moons": data2}

def visualize_results(data, labels, title, filename):
    """Visualize clustering results."""
    plt.figure(figsize=(8, 6))
    plt.scatter(data[:, 0], data[:, 1], c=labels, cmap='viridis', s=50, alpha=0.8)
    plt.title(title)
    plt.savefig(f"out/{filename}")
    plt.close()

def test_dbscan(data, eps, minPts, dataset_name):
    """Test  DBSCAN."""
    print(f"Testing DBSCAN on {dataset_name} data...")
    # Convert data to list of tuples for compatibility with the C++ wrapper
    data_tuples = [tuple(point) for point in data]
    # Run DBSCAN
    results = clusterbox.dbscan(data_tuples, eps=eps, minPts=minPts)
    # Extract labels
    labels = np.array([label for _, _, label in results])
    print(labels)
    # Visualize results
    visualize_results(np.array(data), labels, f"DBSCAN (GPU) - {dataset_name}", f"dbscan_gpu_{dataset_name}.png")

def test_kmeans(data, k, maxIterations, dataset_name):
    """TestK-Means."""
    print(f"Testing K-Means on {dataset_name} data...")
    # Convert data to list of tuples for compatibility with the C++ wrapper
    data_tuples = [tuple(point) for point in data]
    # Run K-Means
    results = clusterbox.kmeans(data_tuples, k=k, maxIterations=maxIterations)
    # Extract labels
    labels = np.array([label for _, _, label in results])
    # Visualize results
    visualize_results(np.array(data), labels, f"K-Means (GPU) - {dataset_name}", f"kmeans_gpu_{dataset_name}.png")

if __name__ == "__main__":
    # Generate synthetic test data
    datasets = generate_test_data()

    # Test GPU DBSCAN
    test_dbscan(datasets["blobs"], eps=0.8, minPts=5, dataset_name="blobs")
    test_dbscan(datasets["moons"], eps=0.2, minPts=5, dataset_name="moons")

    # Test GPU K-Means
    test_kmeans(datasets["blobs"], k=4, maxIterations=100, dataset_name="blobs")
    test_kmeans(datasets["moons"], k=2, maxIterations=100, dataset_name="moons")

    # Illegal Input Handling
    test_dbscan(datasets["blobs"], eps=-1, minPts=5, dataset_name = "blobs")
    tetst_kmeans(datasets["blobs"], k=-1, maxIterations=100, dataset_name="blobs")
