#pragma once

#include "metrics_registry.hpp"
#include <cstdint>
#include <memory>
#include <opentelemetry/metrics/meter.h>

namespace mk::metrics {

    class otel_counter : public counter {
        public:
            otel_counter(std::unique_ptr<opentelemetry::metrics::Counter<uint64_t>> counter): counter_(std::move(counter)) {}
            void inc(uint64_t value, const mk::metrics::dimensions& labels) override;
            ~otel_counter() = default;
        private: 
            std::unique_ptr<opentelemetry::metrics::Counter<uint64_t>> counter_;
    };

    class otel_histogram : public histogram {
        public:
          otel_histogram(std::unique_ptr<opentelemetry::metrics::Histogram<double>> histogram)
              : histogram_(std::move(histogram)) {}
          void observe(double value,
                       const mk::metrics::dimensions &labels) override;
            ~otel_histogram() = default;
        private:
            std::unique_ptr<opentelemetry::metrics::Histogram<double>> histogram_;
    };

    class otel_timer : public timer {
        public:
          otel_timer(std::unique_ptr<opentelemetry::metrics::Histogram<double>> histogram)
              : histogram_(std::move(histogram)) {}
            void start() override;
            void stop(const mk::metrics::dimensions& labels) override;
            ~otel_timer() = default;
        private:
            std::chrono::time_point<std::chrono::steady_clock> start_time_;
            std::unique_ptr<opentelemetry::metrics::Histogram<double>> histogram_;
    };
}