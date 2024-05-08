#include "metrics_registry.hpp"
#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include "metrics_impl.hpp"
#include "otel_registry.hpp"

TEST_CASE("Metric API", "[metrics]") {
 //   SECTION("Test counter metric inc() functionality") {
 //     mk::metrics::otel_registry registry;
 //     auto counter =
 //         registry.create_counter("my_counter", "my counter to test");
 //     counter->inc();
 //     counter->inc();
 //     auto metrics_str = registry.export_metrics();
 //     REQUIRE_THAT(metrics_str,
 //                  Catch::Matchers::ContainsSubstring(
 //                      "# HELP my_counter_total my counter to test\n# TYPE "
 //                      "my_counter_total counter\nmy_counter_total 2"));
 //   }
    // Write unit tests for the counter , histogram and timer metrics 
    // using the above example as a reference
    SECTION("Test histogram metric observe() functionality") {
      mk::metrics::otel_registry registry;
      auto buckets = mk::metrics::buckets_info{0, 5, 10, 20, 50, 100};
      auto histogram = registry.create_histogram(
          "my_histogram", "my histogram to test", "", buckets);
      histogram->observe(10);
      histogram->observe(20);
      auto metrics_str = registry.export_metrics();
      REQUIRE_THAT(metrics_str,
                   Catch::Matchers::ContainsSubstring(
                       "# HELP my_histogram my histogram to test\n# TYPE "
                       "my_histogram histogram\nmy_histogram_bucket{le=\"5\"} "
                       "0\nmy_histogram_bucket{le=\"10\"} "
                       "1\nmy_histogram_bucket{le=\"20\"} "
                       "2\nmy_histogram_bucket{le=\"50\"} "
                       "2\nmy_histogram_bucket{le=\"100\"} "
                       "2\nmy_histogram_bucket{le=\"+Inf\"} "
                       "2\nmy_histogram_sum 30\nmy_histogram_count 2"));
    }

//    SECTION("Test timers metric functionality") {
//      mk::metrics::otel_registry registry;
//      auto buckets = mk::metrics::buckets_info{0, 5, 10, 20, 50, 100};
//      auto timer =
//          registry.create_timer("my_timer", "my timer to test", "ms", buckets);
//      timer->start();
//      std::this_thread::sleep_for(std::chrono::milliseconds(100));
//      timer->stop();
//      auto metrics_str = registry.export_metrics();
//      REQUIRE_THAT(metrics_str,
//                   Catch::Matchers::ContainsSubstring(
//                       "# HELP  my histogram to test\n# TYPE my_histogram "
//                       "histogram\nmy_histogram_bucket{le=\"5\"} "
//                       "0\nmy_histogram_bucket{le=\"10\"} "
//                       "1\nmy_histogram_bucket{le=\"20\"} "
//                       "2\nmy_histogram_bucket{le=\"50\"} "
//                       "2\nmy_histogram_bucket{le=\"100\"} "
//                       "2\nmy_histogram_bucket{le=\"+Inf\"} "
//                       "2\nmy_histogram_sum 30\nmy_histogram_count 2"));
//    }
}
