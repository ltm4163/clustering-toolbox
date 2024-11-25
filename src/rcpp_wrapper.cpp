#include <Rcpp.h>
#include "dbscan.h"
#include "kmeans.h"

// Helper function to extract cluster labels from a vector of Points
std::vector<int> extract_labels(const std::vector<Point>& points) {
    std::vector<int> labels;
    labels.reserve(points.size());
    for (const auto& point : points) {
        labels.push_back(point.cluster);
    }
    return labels;
}

// [[Rcpp::export]]
Rcpp::IntegerVector dbscan_interface(Rcpp::NumericMatrix points, double eps, int minPts) {
    std::vector<Point> cpp_points;

    for (int i = 0; i < points.nrow(); ++i) {
        cpp_points.emplace_back(points(i, 0), points(i, 1));
    }

    std::vector<Point> clustered_points = dbscan(cpp_points, eps, minPts);
    std::vector<int> labels = extract_labels(clustered_points);
    return Rcpp::wrap(labels);
}

// [[Rcpp::export]]
Rcpp::IntegerVector kmeans_interface(Rcpp::NumericMatrix points, int numClusters, int maxIter) {
    std::vector<Point> cpp_points;

    for (int i = 0; i < points.nrow(); ++i) {
        cpp_points.emplace_back(points(i, 0), points(i, 1));
    }

    std::vector<Point> clustered_points = kmeans(cpp_points, numClusters, maxIter);
    std::vector<int> labels = extract_labels(clustered_points);
    return Rcpp::wrap(labels);
}

// Expose the functions to R
RcppExport SEXP _dbscan(SEXP points, SEXP eps, SEXP minPts) {
    return Rcpp::wrap(dbscan_interface(points, Rcpp::as<double>(eps), Rcpp::as<int>(minPts)));
}

RcppExport SEXP _kmeans(SEXP points, SEXP numClusters, SEXP maxIter) {
    return Rcpp::wrap(kmeans_interface(points, Rcpp::as<int>(numClusters), Rcpp::as<int>(maxIter)));
}

