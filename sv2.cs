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

        try
        {
            // Start the listener
            listener.Start();
            Console.WriteLine("Server started. Listening on http://localhost:8080/");

            // Keep the server running indefinitely
            while (true)
            {
                try
                {
                    // Wait for an incoming request
                    HttpListenerContext context = await listener.GetContextAsync();
                    HttpListenerRequest request = context.Request;

                    // Prepare the response object
                    HttpListenerResponse response = context.Response;
                    string responseString;
                    byte[] buffer;

                    // Check the request method
                    if (request.HttpMethod == "POST")
                    {
                        // Handle POST request
                        using StreamReader reader = new StreamReader(request.InputStream, request.ContentEncoding);
                        string message = await reader.ReadToEndAsync();
                        Console.WriteLine($"Received POST message: {message}");

                        // Write the message to a file
                        string filePath = "received_message.txt";  // Change the file path as needed
                        bool fileWritten = await WriteToFileAsync(filePath, message);

                        if (fileWritten)
                        {
                            responseString = "Message received and written to file successfully!";
                        }
                        else
                        {
                            responseString = "Failed to write message to file.";
                        }

                        buffer = Encoding.UTF8.GetBytes(responseString);
                        response.ContentType = "text/plain";
                    }
                    else if (request.HttpMethod == "GET")
                    {
                        // Handle GET request
                        Console.WriteLine("Received a GET request.");

                        // Get the file path from the query parameter (optional)
                        string filePath = request.QueryString["file"];
                        if (string.IsNullOrEmpty(filePath))
                        {
                            filePath = "received_message.txt"; // Default file if no file query parameter is given
                        }

                        // Read the file content
                        string fileContent = await ReadFileAsync(filePath);

                        // Prepare the response for GET request
                        if (fileContent != null)
                        {
                            responseString = fileContent;
                            buffer = Encoding.UTF8.GetBytes(responseString);
                            response.ContentType = "text/plain";
                        }
                        else
                        {
                            responseString = "File not found or error reading file.";
                            buffer = Encoding.UTF8.GetBytes(responseString);
                            response.ContentType = "text/plain";
                            response.StatusCode = (int)HttpStatusCode.NotFound;
                        }
                    }
                    else
                    {
                        // Handle other HTTP methods
                        Console.WriteLine("Received an unsupported HTTP request.");

                        // Prepare response for unsupported methods
                        responseString = "HTTP method not supported.";
                        buffer = Encoding.UTF8.GetBytes(responseString);
                        response.ContentType = "text/plain";
                        response.StatusCode = (int)HttpStatusCode.MethodNotAllowed;
                    }

                    // Set response headers and content
                    response.ContentLength64 = buffer.Length;

                    // Send the response back to the client
                    await response.OutputStream.WriteAsync(buffer, 0, buffer.Length);
                    response.OutputStream.Close();
                }
                catch (Exception ex)
                {
                    // Log any errors during request processing
                    Console.WriteLine($"Error handling request: {ex.Message}");
                }
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error starting the server: {ex.Message}");
        }
        finally
        {
            listener.Stop();
            Console.WriteLine("Server stopped.");
        }
    }

    // Helper function to write content to a file asynchronously
    static async Task<bool> WriteToFileAsync(string filePath, string content)
    {
        try
        {
            // Ensure the directory exists
            string directory = Path.GetDirectoryName(filePath);
            if (!Directory.Exists(directory))
            {
                Directory.CreateDirectory(directory);
            }

            // Write the content to the file asynchronously
            await File.WriteAllTextAsync(filePath, content);
            return true;
        }
        catch (Exception ex)
        {
            // Log any file write errors
            Console.WriteLine($"Error writing to file: {ex.Message}");
            return false;
        }
    }

    // Helper function to read content from a file asynchronously
    static async Task<string> ReadFileAsync(string filePath)
    {
        try
        {
            // Check if the file exists
            if (File.Exists(filePath))
            {
                // Read the content of the file asynchronously
                return await File.ReadAllTextAsync(filePath);
            }
            else
            {
                return null; // File does not exist
            }
        }
        catch (Exception ex)
        {
            // Log any file read errors
            Console.WriteLine($"Error reading from file: {ex.Message}");
            return null;
        }
    }
}
