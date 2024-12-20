using MQTTnet.Client;
using MQTTnet;
using System;
using System.Text;
using System.Threading.Tasks;
using Microsoft.AspNetCore.SignalR;

namespace MqttViewer.Models
{
    public class MqttService
    {

#if DEBUG
        public readonly static string _host = "mqtt.dataext.com.br";

#else
                private readonly static string _host = "127.0.0.1";
#endif
        private readonly int _port = 1883;
        private readonly string _topic = "test/ifsc";
        private readonly IHubContext<MqttHub> _hubContext;

        private IMqttClient _mqttClient;

        public MqttService(IHubContext<MqttHub> hubContext)
        {
            _hubContext = hubContext;

            var factory = new MqttFactory();
            _mqttClient = factory.CreateMqttClient();
        }

        public async Task StartListeningAsync()
        {
            var options = new MqttClientOptionsBuilder()
                .WithTcpServer(_host, _port)
                .Build();

            _mqttClient.ApplicationMessageReceivedAsync += async e =>
            {
                var message = Encoding.UTF8.GetString(e.ApplicationMessage.Payload);
                // Envia mensagem para todos os clientes conectados via SignalR
                await _hubContext.Clients.All.SendAsync("ReceiveMessage", message);
            };

            await _mqttClient.ConnectAsync(options);
            await _mqttClient.SubscribeAsync(new MqttTopicFilterBuilder()
                .WithTopic(_topic)
                .Build());
        }

        public static async Task<bool> EnviarMensagemAsync(string mensagem)
        {
            try
            {
                // Configurar o cliente MQTT
                var factory = new MqttFactory();
                var client = factory.CreateMqttClient();

                // Configuração do cliente MQTT
                var options = new MqttClientOptionsBuilder()
                    .WithTcpServer(_host, 1883)  // Broker e porta
                    .Build();

                // Conectar ao broker MQTT
                await client.ConnectAsync(options, CancellationToken.None);

                // Mensagem a ser enviada
                var message = new MqttApplicationMessageBuilder()
                    .WithTopic("test/ifsc")  // Tópico
                    .WithPayload(Encoding.UTF8.GetBytes(mensagem))  // Mensagem
                    .WithQualityOfServiceLevel(MQTTnet.Protocol.MqttQualityOfServiceLevel.AtMostOnce)  // QoS 0 (enviar uma vez, sem garantia)
                    .Build();

                // Publicar a mensagem
                await client.PublishAsync(message, CancellationToken.None);

                // Desconectar do broker
                await client.DisconnectAsync();

                return true; // Sucesso
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Erro: {ex.Message}");
                return false; // Falha
            }
        }
    }
}
