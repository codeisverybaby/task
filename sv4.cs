using System;
using System.Net;
using System.Net.Http;
using System.Text;
using System.Text.Json;
using System.Threading;
using System.Threading.Tasks;

class Program
{
    private static readonly HttpClient _httpClient = new HttpClient();
    private static readonly HttpListener _httpListener = new HttpListener();

    static async Task Main(string[] args)
    {
        // Start HTTP Listener to handle client requests
        _httpListener.Prefixes.Add("http://localhost:5000/"); // Listen on port 5000
        _httpListener.Start();
        Console.WriteLine("Server started. Listening on http://localhost:5000/");

        // Run the client request handler and API polling concurrently
        var listenerTask = Task.Run(HandleClientRequests);
        var pollingTask = Task.Run(() => PollApiAndHandleCommands());

        await Task.WhenAll(listenerTask, pollingTask);
    }

    private static async Task HandleClientRequests()
    {
        while (true)
        {
            try
            {
                // Wait for an incoming request
                var context = await _httpListener.GetContextAsync();
                var request = context.Request;
                var response = context.Response;

                if (request.HttpMethod == "POST")
                {
                    using (var reader = new System.IO.StreamReader(request.InputStream, request.ContentEncoding))
                    {
                        var body = await reader.ReadToEndAsync();
                        Console.WriteLine($"Received request from client: {body}");
                    }

                    // Respond to the client
                    string responseString = "Request received successfully";
                    byte[] buffer = Encoding.UTF8.GetBytes(responseString);
                    response.ContentLength64 = buffer.Length;
                    await response.OutputStream.WriteAsync(buffer, 0, buffer.Length);
                }
                response.Close();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error in handling client request: {ex.Message}");
            }
        }
    }

    private static async Task PollApiAndHandleCommands()
    {
        while (true)
        {
            try
            {
                var apiResponse = await _httpClient.GetAsync("http://www.dnd5eapi.co/api/monsters/giant-spider");
                if (apiResponse.IsSuccessStatusCode)
                {
                    var content = await apiResponse.Content.ReadAsStringAsync();

                    var json = JsonSerializer.Deserialize<JsonElement>(content);
                    Console.WriteLine($"json received from API: {json}");

                    if (json.TryGetProperty("command", out var command))
                    {
                        // Simulate sending a response to a client
                        Console.WriteLine($"Command received from API: {command.GetString()}");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error polling API: {ex.Message}");
            }

            await Task.Delay(1000); // Wait 1 second before the next API call
        }
    }
}
