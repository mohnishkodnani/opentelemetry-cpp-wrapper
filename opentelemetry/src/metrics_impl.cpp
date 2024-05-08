#include "metrics_impl.hpp"
#include <opentelemetry/metrics/sync_instruments.h>
namespace mk::metrics {

    void otel_counter::inc(uint64_t value, const mk::metrics::dimensions &labels) {
        counter_->Add(value, labels);
        // Increment the counter
    }

    void otel_histogram::observe(double value, const mk::metrics::dimensions &labels) {
        histogram_->Record(value, labels, opentelemetry::context::Context{});
        // Observe the histogram
    }

    void otel_timer::start() {
        // Start the timer
        start_time_ = std::chrono::steady_clock::now();
    }
    void otel_timer::stop(const mk::metrics::dimensions &labels) {
        // Stop the timer
        auto end_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time_);
        histogram_->Record(duration.count(), labels, opentelemetry::context::Context{});
    }
}
// Write unit tests that validate functions in this class.
// Path: opentelemetry/tests/metrics_impl_test.cpp
// #include "metrics_impl.hpp"
// #include <gtest/gtest.h>
//
// TEST(otel_counter, inc) {
//     // Create a counter object
//     auto counter = std::make_unique<otel_counter>(nullptr);
//     // Call the inc method with value 10 and empty labels
//     counter->inc(10, {});
//     // Validate the counter value
//     EXPECT_EQ(10, counter->counter_->GetHandle()->GetBoundValue().Get());
// }
