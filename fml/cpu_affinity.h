// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <optional>
#include <string>
#include <vector>

namespace fml {

/// The CPU Affinity provides a hint to the operating system on which cores a
/// particular thread should be scheduled on. The operating system may or may
/// not honor these requests.
enum class CpuAffinity {
  /// @brief Request CPU affinity for the performance cores.
  ///
  ///        Generally speaking, only the UI and Raster thread should
  ///        use this option.
  kPerformance,

  /// @brief Request CPU affinity for the efficiency cores.
  kEfficiency,

  /// @brief Request affinity for all non-performance cores.
  kNotPerformance,
};

struct CpuIndexAndSpeed {
  // The index of the given CPU.
  size_t index;
  // CPU speed in kHZ
  int64_t speed;
};

/// @brief A class that computes the correct CPU indices for a requested CPU
///        affinity.
///
/// @note  This is visible for testing.
class CPUSpeedTracker {
 public:
  explicit CPUSpeedTracker(std::vector<CpuIndexAndSpeed> data);

  /// @brief The class is valid if it has more than one CPU index and a distinct
  ///        set of efficiency or performance CPUs.
  ///
  ///        If all CPUs are the same speed this returns false, and all requests
  ///        to set affinity are ignored.
  bool IsValid() const;

  /// @brief Return the set of CPU indices for the requested CPU affinity.
  ///
  ///        If the tracker is valid, this will always return a non-empty set.
  const std::vector<size_t>& GetIndices(CpuAffinity affinity) const;

 private:
  bool valid_ = false;
  std::vector<CpuIndexAndSpeed> cpu_speeds_;
  std::vector<size_t> efficiency_;
  std::vector<size_t> performance_;
  std::vector<size_t> not_performance_;
};

/// @note Visible for testing.
std::optional<int64_t> ReadIntFromFile(const std::string& path);

}  // namespace fml
