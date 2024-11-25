# Load the shared library
dyn.load("rclusterbox.so")

# Define the R wrapper functions
dbscan <- function(points, eps, minPts) {
  .Call("_dbscan", points, eps, minPts)
}

kmeans <- function(points, numClusters, maxIter) {
  .Call("_kmeans", points, numClusters, maxIter)
}

# Test DBSCAN
test_points <- matrix(c(1, 2, 3, 4, 5, 6, 7, 8, 9, 10), ncol = 2, byrow = TRUE)
eps <- 1.5
minPts <- 2
cat("Testing DBSCAN:\n")
dbscan_result <- dbscan(test_points, eps, minPts)
print(dbscan_result)

# Test K-Means
numClusters <- 2
maxIter <- 10
cat("Testing K-Means:\n")
kmeans_result <- kmeans(test_points, numClusters, maxIter)
print(kmeans_result)

