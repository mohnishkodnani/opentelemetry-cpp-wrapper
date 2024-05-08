#define CATCH_CONFIG_MAIN

#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include "metrics_impl.hpp"
#include "otel_registry.hpp"

TEST_CASE("Metric API", "[metrics]") {
    mk::metrics::otel_registry registry;

    SECTION("Test counter metric inc() functionality") {
        auto counter = registry.create_counter("my_counter", "my counter to test");
        counter->inc();
        counter->inc();
        auto metrics_str = registry.export_metrics();
        REQUIRE_THAT(metrics_str, 
            Catch::Matchers::ContainsSubstring("# HELP my_counter_total my counter to test\n# TYPE my_counter_total counter\nmy_counter_total 2"));
    }

    SECTION("Test histogram metric observe() functionality") {
        auto histogram = registry.create_histogram("my_histogram", "my histogram to test");
        histogram->observe(10);
        histogram->observe(20);
        auto metrics_str = registry.export_metrics();
        REQUIRE_THAT(metrics_str, 
            Catch::Matchers::ContainsSubstring("# HELP my_histogram my histogram to test\n# TYPE my_histogram histogram\nmy_histogram_bucket{le=\"5\"} 0\nmy_histogram_bucket{le=\"10\"} 1\nmy_histogram_bucket{le=\"20\"} 2\nmy_histogram_bucket{le=\"50\"} 2\nmy_histogram_bucket{le=\"100\"} 2\nmy_histogram_bucket{le=\"+Inf\"} 2\nmy_histogram_sum 30\nmy_histogram_count 2"));
    }

    SECTION("Test timer metric observe() functionality") {
        auto timer = registry.create_timer("my_timer", "my timer to test");
        timer->observe(5);
        timer->observe(10);
        auto metrics_str = registry.export_metrics();
        REQUIRE_THAT(metrics_str, 
            Catch::Matchers::ContainsSubstring("# HELP my_timer my timer to test\n# TYPE my_timer summary\nmy_timer{quantile=\"0.5\"} 7.5\nmy_timer{quantile=\"0.9\"} 10\nmy_timer{quantile=\"0.99\"} 10\nmy_timer_sum 15\nmy_timer_count 2"));
    }
}