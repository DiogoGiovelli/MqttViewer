using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;
using MQTTnet.Client;
using MqttViewer.Models;

namespace MqttViewer.Controllers
{
    public class HomeController : Controller
    {
        private readonly ILogger<HomeController> _logger;
        private readonly MqttService _mqttService;

        public HomeController(ILogger<HomeController> logger, MqttService mqttService)
        {
            _logger = logger;

        }

        public IActionResult Index()
        {
            return View();
        }

        [HttpPost]
        public async Task<IActionResult> EnviarMqtt(string message)
        {
            if (!string.IsNullOrEmpty(message))
            {
                bool result = await MqttService.EnviarMensagemAsync(message);

                if (result)
                {
                    ViewData["MessageStatus"] = "Mensagem enviada com sucesso!";
                }
                else
                {
                    ViewData["MessageStatus"] = "Erro ao enviar a mensagem.";
                }
            }
            else
            {
                ViewData["MessageStatus"] = "A mensagem não pode ser vazia.";
            }

            return View("Index"); // Redireciona de volta para a página inicial
        }

        public async Task<IActionResult> DadosBrutos()
        {
            return View();
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }
    }
}
