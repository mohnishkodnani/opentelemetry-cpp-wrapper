#include "otel_registry.hpp"
#include <iostream>

int main(int argc, char** argv) {

  mk::metrics::otel_registry registry;
  auto buckets = mk::metrics::buckets_info{0, 5, 10, 20, 50, 100};
  auto histogram = registry.create_histogram(
      "my_histogram", "my histogram to test", "", buckets);
  histogram->observe(10);
  histogram->observe(20);
  auto metrics_str = registry.export_metrics();
  std::cout << metrics_str << std::endl;
  return 0;
}