#pragma once

#include <string>
#include <map>

namespace mk::metrics {
    using dimensions = std::map<std::string, std::string>;
    using buckets_info =  std::vector<double>;
    enum class metric_runtime {
        prometheus,
        opentelemetry
    };
    class counter;
    class histogram;
    class timer;
    class metrics_registry {
        public:
            virtual std::unique_ptr<counter> create_counter(const std::string& name, const std::string& help) = 0;
            virtual std::unique_ptr<histogram> create_histogram(const std::string& name, const std::string& help, const std::string& unit, const buckets_info& buckets) = 0;
            virtual std::unique_ptr<timer> create_timer(const std::string& name, const std::string& help, const std::string& unit, const buckets_info& buckets) = 0;
            virtual std::string export_metrics() = 0;
    };

    class counter {
        public:
            virtual void inc(uint64_t value = 1, const mk::metrics::dimensions& labels = {}) = 0;
            virtual ~counter() = default;
    };
    class histogram {
        public:
            virtual void observe(double value, const mk::metrics::dimensions& labels = {}) = 0;
            virtual ~histogram() = default;
    };
    class timer {
        public:
            virtual void start() = 0;
            virtual void stop(const mk::metrics::dimensions& labels = {}) = 0;
            virtual ~timer() = default;
    };
}