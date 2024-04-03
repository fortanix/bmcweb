#include "redfish.hpp"

#include "account_service.hpp"
#include "aggregation_service.hpp"
#include "app.hpp"
#include "bios.hpp"
#include "cable.hpp"
#include "certificate_service.hpp"
#include "chassis.hpp"
#include "environment_metrics.hpp"
#include "ethernet.hpp"
#include "event_service.hpp"
#include "eventservice_sse.hpp"
#include "fabric_adapters.hpp"
#include "fan.hpp"
#include "hypervisor_system.hpp"
#include "log_services.hpp"
#include "manager_diagnostic_data.hpp"
#include "managers.hpp"
#include "memory.hpp"
#include "message_registries.hpp"
#include "metric_report.hpp"
#include "metric_report_definition.hpp"
#include "network_protocol.hpp"
#include "pcie.hpp"
#include "pcie_slots.hpp"
#include "power.hpp"
#include "power_subsystem.hpp"
#include "power_supply.hpp"
#include "processor.hpp"
#include "redfish_sessions.hpp"
#include "redfish_v1.hpp"
#include "roles.hpp"
#include "sensors.hpp"
#include "service_root.hpp"
#include "storage.hpp"
#include "systems.hpp"
#include "task.hpp"
#include "telemetry_service.hpp"
#include "thermal.hpp"
#include "thermal_metrics.hpp"
#include "thermal_subsystem.hpp"
#include "trigger.hpp"
#include "update_service.hpp"
#include "virtual_media.hpp"

namespace redfish
{

RedfishService::RedfishService(App& app)
{
    requestAccountServiceRoutes(app);
#ifdef BMCWEB_ENABLE_REDFISH_AGGREGATION
    requestRoutesAggregationService(app);
    requestRoutesAggregationSourceCollection(app);
    requestRoutesAggregationSource(app);
#endif
    requestRoutesRoles(app);
    requestRoutesRoleCollection(app);
    requestRoutesServiceRoot(app);
    requestRoutesNetworkProtocol(app);
    requestRoutesSession(app);
    requestEthernetInterfacesRoutes(app);
#ifdef BMCWEB_ALLOW_DEPRECATED_POWER_THERMAL
    requestRoutesThermal(app);
    requestRoutesPower(app);
#endif
#ifdef BMCWEB_NEW_POWERSUBSYSTEM_THERMALSUBSYSTEM
    requestRoutesEnvironmentMetrics(app);
    requestRoutesPowerSubsystem(app);
    requestRoutesPowerSupply(app);
    requestRoutesPowerSupplyCollection(app);
    requestRoutesThermalMetrics(app);
    requestRoutesThermalSubsystem(app);
    requestRoutesFan(app);
    requestRoutesFanCollection(app);
#endif
    requestRoutesManagerCollection(app);
    requestRoutesManager(app);
    requestRoutesManagerResetAction(app);
    requestRoutesManagerResetActionInfo(app);
    requestRoutesManagerResetToDefaultsAction(app);
    requestRoutesManagerDiagnosticData(app);
    requestRoutesChassisCollection(app);
    requestRoutesChassis(app);
    requestRoutesChassisResetAction(app);
    requestRoutesChassisResetActionInfo(app);
    requestRoutesChassisDrive(app);
    requestRoutesChassisDriveName(app);
    requestRoutesUpdateService(app);
    requestRoutesStorageCollection(app);
    requestRoutesStorage(app);
    requestRoutesStorageControllerCollection(app);
    requestRoutesStorageController(app);
    requestRoutesDrive(app);
    requestRoutesCable(app);
    requestRoutesCableCollection(app);

    requestRoutesSystemLogServiceCollection(app);
    requestRoutesEventLogService(app);
    requestRoutesPostCodesEntryAdditionalData(app);

    requestRoutesPostCodesLogService(app);
    requestRoutesPostCodesClear(app);
    requestRoutesPostCodesEntry(app);
    requestRoutesPostCodesEntryCollection(app);

#ifdef BMCWEB_ENABLE_REDFISH_DUMP_LOG
    requestRoutesSystemDumpService(app);
    requestRoutesSystemDumpEntryCollection(app);
    requestRoutesSystemDumpEntry(app);
    requestRoutesSystemDumpCreate(app);
    requestRoutesSystemDumpClear(app);

    requestRoutesBMCDumpService(app);
    requestRoutesBMCDumpEntryCollection(app);
    requestRoutesBMCDumpEntry(app);
    requestRoutesBMCDumpEntryDownload(app);
    requestRoutesBMCDumpCreate(app);
    requestRoutesBMCDumpClear(app);

    requestRoutesFaultLogDumpService(app);
    requestRoutesFaultLogDumpEntryCollection(app);
    requestRoutesFaultLogDumpEntry(app);
    requestRoutesFaultLogDumpClear(app);
#endif

#ifndef BMCWEB_ENABLE_REDFISH_DBUS_LOG_ENTRIES
    requestRoutesJournalEventLogEntryCollection(app);
    requestRoutesJournalEventLogEntry(app);
    requestRoutesJournalEventLogClear(app);
#endif

    requestRoutesBMCLogServiceCollection(app);
#ifdef BMCWEB_ENABLE_REDFISH_BMC_JOURNAL
    requestRoutesBMCJournalLogService(app);
    requestRoutesBMCJournalLogEntryCollection(app);
    requestRoutesBMCJournalLogEntry(app);
#endif

#ifdef BMCWEB_ENABLE_REDFISH_CPU_LOG
    requestRoutesCrashdumpService(app);
    requestRoutesCrashdumpEntryCollection(app);
    requestRoutesCrashdumpEntry(app);
    requestRoutesCrashdumpFile(app);
    requestRoutesCrashdumpClear(app);
    requestRoutesCrashdumpCollect(app);
#endif // BMCWEB_ENABLE_REDFISH_CPU_LOG

    requestRoutesProcessorCollection(app);
    requestRoutesProcessor(app);
    requestRoutesOperatingConfigCollection(app);
    requestRoutesOperatingConfig(app);
    requestRoutesMemoryCollection(app);
    requestRoutesMemory(app);

    requestRoutesSystems(app);

    requestRoutesBiosService(app);
    requestRoutesBiosReset(app);

#ifdef BMCWEB_ENABLE_VM_NBDPROXY
    requestNBDVirtualMediaRoutes(app);
#endif // BMCWEB_ENABLE_VM_NBDPROXY

#ifdef BMCWEB_ENABLE_REDFISH_DBUS_LOG_ENTRIES
    requestRoutesDBusLogServiceActionsClear(app);
    requestRoutesDBusEventLogEntryCollection(app);
    requestRoutesDBusEventLogEntry(app);
    requestRoutesDBusEventLogEntryDownload(app);
#endif

#ifdef BMCWEB_ENABLE_REDFISH_HOST_LOGGER
    requestRoutesSystemHostLogger(app);
    requestRoutesSystemHostLoggerCollection(app);
    requestRoutesSystemHostLoggerLogEntry(app);
#endif

    requestRoutesMessageRegistryFileCollection(app);
    requestRoutesMessageRegistryFile(app);
    requestRoutesMessageRegistry(app);

    requestRoutesCertificateService(app);
    requestRoutesHTTPSCertificate(app);
    requestRoutesLDAPCertificate(app);
    requestRoutesTrustStoreCertificate(app);

    requestRoutesSystemPCIeFunctionCollection(app);
    requestRoutesSystemPCIeFunction(app);
    requestRoutesSystemPCIeDeviceCollection(app);
    requestRoutesSystemPCIeDevice(app);

    requestRoutesSensorCollection(app);
    requestRoutesSensor(app);

    requestRoutesTaskMonitor(app);
    requestRoutesTaskService(app);
    requestRoutesTaskCollection(app);
    requestRoutesTask(app);
    requestRoutesEventService(app);
    requestRoutesEventServiceSse(app);
    requestRoutesEventDestinationCollection(app);
    requestRoutesEventDestination(app);
    requestRoutesFabricAdapters(app);
    requestRoutesFabricAdapterCollection(app);
    requestRoutesSubmitTestEvent(app);

    requestRoutesHypervisorSystems(app);

    requestRoutesTelemetryService(app);
    requestRoutesMetricReportDefinitionCollection(app);
    requestRoutesMetricReportDefinition(app);
    requestRoutesMetricReportCollection(app);
    requestRoutesMetricReport(app);
    requestRoutesTriggerCollection(app);
    requestRoutesTrigger(app);

    // Note, this must be the last route registered
    requestRoutesRedfish(app);
}

} // namespace redfish
