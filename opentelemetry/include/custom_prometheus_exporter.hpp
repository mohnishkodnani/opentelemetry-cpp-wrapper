#pragma once

#include <iostream>
#include <opentelemetry/sdk/metrics/export/metric_producer.h>
#include <opentelemetry/sdk/metrics/instruments.h>
#include <opentelemetry/sdk/metrics/metric_reader.h>
#include <opentelemetry/exporters/prometheus/exporter_utils.h>
#include <ostream>
#include <prometheus/text_serializer.h>
#include <sstream>

namespace mk::metrics {
    /**
     * @class custom_prometheus_exporter
     * @brief A custom Prometheus exporter for OpenTelemetry metrics.
     *
     * This class extends the `opentelemetry::sdk::metrics::MetricReader` class and provides
     * functionality to export metrics in Prometheus format.
     */
    class custom_prometheus_exporter : public opentelemetry::sdk::metrics::MetricReader {        
        public:
            custom_prometheus_exporter(): collection_lock_(), serializer_() {};
            /**
             * Exports the data to a string representation of all metrics in prometheus format.
             *
             * @return A string representation of the exported data in prometheus format.
             */
            std::string Export() {
              std::ostringstream oss;
              if(!IsShutdown()) {
                Collect([&oss,
                         this](opentelemetry::sdk::metrics::ResourceMetrics
                                   &resource_metrics_) {
                  std::lock_guard<std::mutex> lock(collection_lock_);
                  for (const opentelemetry::sdk::metrics::ScopeMetrics &smd : resource_metrics_.scope_metric_data_) {
                    for (const opentelemetry::sdk::metrics::MetricData &md : smd.metric_data_) {
                      for (const opentelemetry::sdk::metrics::PointDataAttributes &dp : md.point_data_attr_) {
                        if(md.instrument_descriptor.type_ == opentelemetry::v1::sdk::metrics::InstrumentType::kHistogram) {
                          auto d = opentelemetry::nostd::get<
                              opentelemetry::sdk::metrics::HistogramPointData>(
                              dp.point_data);
                          std::ostringstream o;
                          for (double b : d.boundaries_) {
                            o << b << ",";
                          }
                          std::cout << o.str() << std::endl;
                        }
                      }
                    }
                  }
                  auto prometheus_metric_data = opentelemetry::exporter::
                      metrics::PrometheusExporterUtils::TranslateToPrometheus(
                          resource_metrics_, true, true);
                  serializer_.Serialize(oss, prometheus_metric_data);
                  return true;
                });
              }
              return oss.str();
            }
            opentelemetry::sdk::metrics::AggregationTemporality GetAggregationTemporality(opentelemetry::sdk::metrics::InstrumentType it) const noexcept override {
                return opentelemetry::sdk::metrics::AggregationTemporality::kCumulative;
            }
            /**
             * \brief Forces a flush of the exporter with an optional timeout.
             *
             * This function is called to force a flush of the exporter. It can be used to ensure that all collected metrics
             * are sent to the Prometheus server. An optional timeout can be provided to limit the duration of the flush operation.
             *
             * \param timeout The timeout duration for the flush operation. By default, it is set to `std::chrono::microseconds::max`,
             *                indicating no timeout.
             * \return `true` if the flush operation was successful, `false` otherwise.
             */
            bool OnForceFlush(std::chrono::microseconds timeout = (std::chrono::microseconds::max)()) noexcept override {
                return true;
            }
            /**
             * @brief Handles the shutdown of the exporter.
             *
             * This function is called when the exporter is being shut down. It allows for a timeout
             * to be specified, after which the shutdown process will be forcefully terminated.
             *
             * @param timeout The timeout duration for the shutdown process. Defaults to the maximum
             *                possible duration if not specified.
             * @return `true` if the shutdown process completed successfully, `false` otherwise.
             */
            bool OnShutDown(std::chrono::microseconds timeout = (std::chrono::microseconds::max)()) noexcept override {
                return true;
            }
          private:
            mutable std::mutex collection_lock_;
            prometheus::TextSerializer serializer_;
    };
}