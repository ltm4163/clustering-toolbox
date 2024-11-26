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

// Helper function to convert Rcpp NumericMatrix to C++ Points
std::vector<Point> convert_to_points(const Rcpp::NumericMatrix& data) {
    std::vector<Point> points;
    for (int i = 0; i < data.nrow(); ++i) {
        std::vector<double> coords(data.ncol());
        for (int j = 0; j < data.ncol(); ++j) {
            coords[j] = data(i, j);
        }
        points.emplace_back(coords);
    }
    return points;
}

// [[Rcpp::export]]
Rcpp::IntegerVector dbscan_interface(Rcpp::NumericMatrix data, double eps, int minPts) {
    std::vector<Point> points = convert_to_points(data);
    std::vector<Point> clustered_points = dbscan(points, eps, minPts);
    std::vector<int> labels = extract_labels(clustered_points);
    return Rcpp::wrap(labels);
}

// [[Rcpp::export]]
Rcpp::IntegerVector kmeans_interface(Rcpp::NumericMatrix data, int k, int maxIterations) {
    std::vector<Point> points = convert_to_points(data);
    std::vector<Point> clustered_points = kmeans(points, k, maxIterations);
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

