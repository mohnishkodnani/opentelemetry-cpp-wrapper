#include "otel_registry.hpp"
#include "metrics_impl.hpp"
#include <__memory/unique_ptr.h>
#include <memory>
#include <opentelemetry/metrics/provider.h>
#include <opentelemetry/sdk/metrics/aggregation/aggregation_config.h>
#include <opentelemetry/sdk/metrics/instruments.h>
#include <opentelemetry/sdk/metrics/meter.h>
#include <opentelemetry/sdk/metrics/meter_provider.h>
#include <opentelemetry/sdk/metrics/aggregation/histogram_aggregation.h>
#include <opentelemetry/sdk/metrics/view/instrument_selector.h>
#include <opentelemetry/sdk/metrics/view/meter_selector.h>
#include <opentelemetry/sdk/metrics/view/view.h>

namespace mk::metrics {

    std::unique_ptr<counter> otel_registry::create_counter(const std::string &name, const std::string &help) {
        auto counter = meter_->CreateUInt64Counter(name, help);
        return std::make_unique<otel_counter>(std::move(counter));
    }

    std::unique_ptr<histogram> otel_registry::create_histogram(const std::string &name, const std::string &help, const std::string& unit, const buckets_info &buckets) {
        // Add custom buckets to the aggregation config.
        auto config = std::make_shared<opentelemetry::sdk::metrics::HistogramAggregationConfig>(); 
        config->boundaries_ = std::move(buckets);
        // Create a view to attach the custom buckets to the histogram.
        auto view = std::make_unique<opentelemetry::sdk::metrics::View>(name + "_view", help, unit, opentelemetry::sdk::metrics::AggregationType::kHistogram, config);
        // Instrument selector to find the instrument to apply the view.
        auto instrument_selector = std::make_unique<opentelemetry::sdk::metrics::InstrumentSelector>(opentelemetry::sdk::metrics::InstrumentType::kHistogram, name, unit);
        // Fetch the global MeterProvider
        auto meter_provider = std::static_pointer_cast<opentelemetry::sdk::metrics::MeterProvider>(opentelemetry::metrics::Provider::GetMeterProvider());
        // Get the meter Selector
        auto meter_selector = std::make_unique<opentelemetry::sdk::metrics::MeterSelector>(name, version, schema);
        // Add the view to the provider.
        meter_provider->AddView(std::move(instrument_selector), std::move(meter_selector), std::move(view));
        // Create the histogram instrument.
        auto meter = meter_provider->GetMeter(otel_registry::name, otel_registry::version, otel_registry::schema);
        auto hist = meter->CreateDoubleHistogram(name, help, unit);
        return std::make_unique<otel_histogram>(std::move(hist));
    }

    std::unique_ptr<timer> otel_registry::create_timer(const std::string &name, const std::string &help, const std::string& unit, const buckets_info &buckets) {
        // Add custom buckets to the aggregation config.
        auto config = std::make_shared<opentelemetry::sdk::metrics::HistogramAggregationConfig>(); 
        config->boundaries_ = buckets;
        // Create a view to attach the custom buckets to the histogram.
        auto view = std::make_unique<opentelemetry::sdk::metrics::View>(name + "_view", help, unit, opentelemetry::sdk::metrics::AggregationType::kHistogram, config);
        // Instrument selector to find the instrument to apply the view.
        auto instrument_selector = std::make_unique<opentelemetry::sdk::metrics::InstrumentSelector>(opentelemetry::sdk::metrics::InstrumentType::kHistogram, name, unit);
        // Fetch the global MeterProvider
        auto meter_provider = std::static_pointer_cast<opentelemetry::sdk::metrics::MeterProvider>(opentelemetry::metrics::Provider::GetMeterProvider());
        // Get the meter Selector
        auto meter_selector = std::make_unique<opentelemetry::sdk::metrics::MeterSelector>(name, version, schema);
        // Add the view to the provider.
        meter_provider->AddView(std::move(instrument_selector), std::move(meter_selector), std::move(view));
        // Create the histogram instrument.
        auto hist = meter_->CreateDoubleHistogram(name, help, unit);
        return std::make_unique<otel_timer>(std::move(hist));
    }

    std::string otel_registry::export_metrics() {
        return cexporter_->Export();
    }
}