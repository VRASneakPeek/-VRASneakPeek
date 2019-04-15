using Microsoft.Azure.Devices;

namespace VRA.SneakPeek2019.Factories
{
    /// <summary>
    /// Factory for service clients.
    /// </summary>
    internal static class ServiceClientFactory
    {
        private static string _connectionString = "HostName=VRATelemetryIoTHubDev.azure-devices.net;SharedAccessKeyName=iothubowner;" +
                "SharedAccessKey=suxuL4aegXhGE+yB/hf1c8zqisz4XdIN0YlAnJA3Xbc=";

        /// <summary>
        /// Creates the service client for communication with the IoT Hub.
        /// </summary>
        /// <returns>The service client.</returns>
        public static ServiceClient CreateServiceClient()
        {
            return ServiceClient.CreateFromConnectionString(_connectionString);
        }
    }
}
