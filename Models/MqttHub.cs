using Microsoft.AspNetCore.SignalR;

namespace MqttViewer.Models
{
    public class MqttHub : Hub
    {
        public async Task SendMessage(string message)
        {
            await Clients.All.SendAsync("ReceiveMessage", message);
        }
    }
}
