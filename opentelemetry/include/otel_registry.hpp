#pragma once

#include "metrics_registry.hpp"
#include "opentelemetry/exporters/prometheus/exporter_factory.h"
#include <opentelemetry/exporters/ostream/metric_exporter_factory.h>
#include "opentelemetry/exporters/prometheus/exporter_options.h"
#include "opentelemetry/sdk/metrics/export/periodic_exporting_metric_reader_options.h"
#include "opentelemetry/sdk/metrics/export/periodic_exporting_metric_reader_factory.h"
#include "opentelemetry/metrics/provider.h"
#include <opentelemetry/sdk/metrics/push_metric_exporter.h>
#include <chrono>
#include <memory>
#include <opentelemetry/sdk/metrics/meter.h>
#include <opentelemetry/sdk/metrics/meter_provider.h>
#include <opentelemetry/sdk/metrics/meter_provider_factory.h>
#include "custom_prometheus_exporter.hpp"
#include <opentelemetry/sdk/metrics/meter_context_factory.h>

namespace mk::metrics {
    class otel_registry : public metrics_registry {
        public:

        otel_registry() {
           cexporter_ = std::make_shared<custom_prometheus_exporter>();
            // auto ostr_exp = opentelemetry::exporter::metrics::OStreamMetricExporterFactory::Create();
            // opentelemetry::sdk::metrics::PeriodicExportingMetricReaderOptions reader_options;
            // reader_options.export_interval_millis = std::chrono::milliseconds(1000);
            // reader_options.export_timeout_millis = std::chrono::milliseconds(500);
            // auto reader = opentelemetry::sdk::metrics::PeriodicExportingMetricReaderFactory::Create(std::move(ostr_exp), reader_options);
             auto context = opentelemetry::sdk::metrics::MeterContextFactory::Create();
            //  context->AddMetricReader(std::move(reader));
             context->AddMetricReader(cexporter_);
            auto u_provider =
                opentelemetry::sdk::metrics::MeterProviderFactory::Create(std::move(context));
            auto meter_provider =
                static_cast<opentelemetry::sdk::metrics::MeterProvider *>(
                    u_provider.get());
            meter_ = static_cast<opentelemetry::sdk::metrics::Meter *>(u_provider->GetMeter(name, version, schema).get());
           opentelemetry::metrics::Provider::SetMeterProvider(std::move(u_provider));

        }
        std::unique_ptr<counter>
        create_counter(const std::string &name, const std::string &help) override;
        std::unique_ptr<histogram>
        create_histogram(const std::string &name, const std::string &help, const std::string& unit,
                            const buckets_info &buckets) override;
          std::unique_ptr<timer>
          create_timer(const std::string &name, const std::string &help, const std::string& unit,
                       const buckets_info &buckets) override;
          std::string export_metrics() override;

        ~otel_registry() {
            opentelemetry::metrics::Provider::SetMeterProvider(nullptr);
        }
        private:
         constexpr static const char* name = "cassini_metrics";
         constexpr static const char* version = "1.2.0";
         constexpr static const char* schema = "https://opentelemetry.io/schemas/1.2.0";
         std::shared_ptr<custom_prometheus_exporter> cexporter_;
         opentelemetry::sdk::metrics::Meter* meter_;
    };
}