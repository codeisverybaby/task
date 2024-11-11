using System;
using System.IO;
using System.Net;
using System.Text;
using System.Threading.Tasks;

class Program
{
    static async Task Main(string[] args)
    {
        // Create an instance of HttpListener
        HttpListener listener = new HttpListener();
        
        // Specify the URL and port (localhost and port 8080 in this example)
        listener.Prefixes.Add("http://localhost:8080/");
        
        // Start the listener
        listener.Start();
        Console.WriteLine("Server started. Listening on http://localhost:8080/");
        
        // Keep the server running indefinitely
        while (true)
        {
            // Wait for an incoming request
            HttpListenerContext context = await listener.GetContextAsync();
            HttpListenerRequest request = context.Request;
            
            // Check if the request is a POST request
            if (request.HttpMethod == "POST")
            {
                // Read the content of the POST request
                using StreamReader reader = new StreamReader(request.InputStream, request.ContentEncoding);
                string message = await reader.ReadToEndAsync();
                Console.WriteLine($"Received message: {message}");
                
                // Prepare a response
                HttpListenerResponse response = context.Response;
                string responseString = "Message received successfully!";
                byte[] buffer = Encoding.UTF8.GetBytes(responseString);
                
                // Set response headers and content
                response.ContentLength64 = buffer.Length;
                response.ContentType = "text/plain";
                
                // Send the response back to the client
                await response.OutputStream.WriteAsync(buffer, 0, buffer.Length);
                response.OutputStream.Close();
            }
            else
            {
                Console.WriteLine("Received a non-POST request.");
            }
        }
    }
}
