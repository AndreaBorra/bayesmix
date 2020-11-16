#include "dirichlet_mixing.hpp"

#include <google/protobuf/stubs/casts.h>

#include <stan/math/prim/prob.hpp>

#include "../../proto/cpp/mixings.pb.h"
#include "../utils/rng.hpp"

void DirichletMixing::update_hypers(
    const std::vector<std::shared_ptr<BaseHierarchy>> &unique_values,
    unsigned int n) {
  if (state.has_fixed_value()) {
    return;
  } else if (state.has_gamma_prior()) {
    auto rng = bayesmix::Rng::Instance().get();

    // Recover parameters
    unsigned int k = unique_values.size();
    double alpha = state.totalmass.alpha();
    double beta = state.totalmass.beta();

    double phi = stan::math::gamma_rng(totalmass + 1, n, rng);
    double odds = (alpha + k - 1) / (n * (beta - log(phi)));
    double prob = odds / (1 + odds);
    double p = stan::math::uniform_rng(0.0, 1.0, rng);
    if (p <= prob) {
      totalmass = stan::math::gamma_rng(alpha + k, beta - log(phi), rng);
    } else {
      totalmass = stan::math::gamma_rng(alpha + k - 1, beta - log(phi), rng);
    }
  } else {
    std::invalid_argument("Error: no possible valid update");
  }
}

void DirichletMixing::set_state(const google::protobuf::Message &curr) {
  DPState *currcast = google::protobuf::internal::down_cast<DPState *>(curr);
  state = *currcast;
  if (state.has_fixed_value()) {
    totalmass = state.totalmass.value();
  } else if (state.has_gamma_prior()) {
    totalmass = state.totalmass.alpha() / state.totalmass.beta();
  } else {
    std::invalid_argument("Error: argument proto is not appropriate");
  }
}