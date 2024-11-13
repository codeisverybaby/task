using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using System.Threading.Tasks;

class Program
{
    // Dictionary to store pending clients by IP, each IP has a list of requests (IP:Port)
    static Dictionary<string, List<HttpListenerContext>> pendingClients = new Dictionary<string, List<HttpListenerContext>>();

    static async Task Main(string[] args)
    {
        HttpListener listener = new HttpListener();
        listener.Prefixes.Add("http://*:8080/");

        // Task to handle console input for selecting and responding to clients
        Task.Run(() => HandleConsoleInput());

        try
        {
            listener.Start();
            Console.WriteLine("Server started. Listening on http://*:8080/");

            while (true)
            {
                // Wait for an incoming request
                HttpListenerContext context = await listener.GetContextAsync();
                string clientIP = context.Request.RemoteEndPoint?.Address.ToString();
                int clientPort = context.Request.RemoteEndPoint?.Port ?? 0;

                Console.WriteLine($"Received request from {clientIP}:{clientPort}");

                // Add client to pending list without responding immediately
                if (!pendingClients.ContainsKey(clientIP))
                {
                    pendingClients[clientIP] = new List<HttpListenerContext>();
                }
                pendingClients[clientIP].Add(context);

                // Display the list of unique client IPs waiting for responses
                DisplayPendingClientIPs();
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error starting server: {ex.Message}");
        }
        finally
        {
            listener.Stop();
            Console.WriteLine("Server stopped.");
        }
    }

    // Method to display all unique client IPs with pending requests, with numbers
    static void DisplayPendingClientIPs()
    {
        Console.WriteLine("\nClients (IP only) awaiting response:");
        int i = 1;
        foreach (var clientIP in pendingClients.Keys)
        {
            Console.WriteLine($"{i}. {clientIP}");
            i++;
        }
        Console.WriteLine();
    }

    // Method to handle console input for responding to clients
    static void HandleConsoleInput()
    {
        while (true)
        {
            Console.Write("Enter client number to see all pending requests (or 'exit' to quit): ");
            string input = Console.ReadLine();

            if (input == "exit")
            {
                break;
            }

            // Parse the input to get the client number
            if (int.TryParse(input, out int clientNumber) && clientNumber > 0)
            {
                // Get the IP corresponding to the selected number
                string selectedClientIP = GetClientIPByNumber(clientNumber);

                if (selectedClientIP != null && pendingClients.TryGetValue(selectedClientIP, out List<HttpListenerContext> contexts))
                {
                    Console.WriteLine($"\nPending requests for IP {selectedClientIP}:");

                    // Show all IP:Port pairs for the selected IP
                    for (int i = 0; i < contexts.Count; i++)
                    {
                        var context = contexts[i];
                        string clientIdentifier = $"{selectedClientIP}:{context.Request.RemoteEndPoint?.Port}";
                        Console.WriteLine($"{i + 1}. {clientIdentifier}");
                    }

                    Console.Write("Enter the number of the client you want to respond to: ");
                    if (int.TryParse(Console.ReadLine(), out int clientIndex) &&
                        clientIndex > 0 && clientIndex <= contexts.Count)
                    {
                        HttpListenerContext selectedContext = contexts[clientIndex - 1];
                        Console.Write("Enter response body for this client: ");
                        string customResponse = Console.ReadLine();

                        // Send the custom response to the selected client
                        SendCustomResponse(selectedContext, customResponse);

                        // Remove client from the pending list after responding
                        contexts.RemoveAt(clientIndex - 1);
                        if (contexts.Count == 0)
                        {
                            pendingClients.Remove(selectedClientIP);
                        }

                        // Display updated list of pending client IPs
                        DisplayPendingClientIPs();
                    }
                    else
                    {
                        Console.WriteLine("Invalid selection.");
                    }
                }
                else
                {
                    Console.WriteLine("Client IP not found or already responded to all requests from this IP.");
                }
            }
            else
            {
                Console.WriteLine("Invalid input. Please enter a valid client number.");
            }
        }
    }

    // Method to get the client IP by its number
    static string GetClientIPByNumber(int clientNumber)
    {
        int i = 1;
        foreach (var clientIP in pendingClients.Keys)
        {
            if (i == clientNumber)
            {
                return clientIP;
            }
            i++;
        }
        return null;
    }

    // Method to send a custom response to a specific client
    static async void SendCustomResponse(HttpListenerContext context, string responseText)
    {
        HttpListenerResponse response = context.Response;
        byte[] buffer = Encoding.UTF8.GetBytes(responseText);

        response.ContentLength64 = buffer.Length;
        response.ContentType = "text/plain";

        await response.OutputStream.WriteAsync(buffer, 0, buffer.Length);
        response.OutputStream.Close();

        Console.WriteLine("Response sent to client.");
    }
}
