#ifndef BAYESMIX_UTILS_CLUSTER_UTILS_HPP_
#define BAYESMIX_UTILS_CLUSTER_UTILS_HPP_

#include <Eigen/Dense>

namespace bayesmix {
//! Estimates the clustering structure of the data via LS minimization
Eigen::VectorXd cluster_estimate(const Eigen::MatrixXd &alloc_chain);
}  // namespace bayesmix

#endif  // BAYESMIX_UTILS_CLUSTER_UTILS_HPP_
