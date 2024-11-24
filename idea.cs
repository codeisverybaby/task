using System;
using System.Collections.Generic;
using System.Net.Http;
using System.Text.Json;
using System.Threading.Tasks;

class Program
{
    // A dictionary to store the saved data (ID -> List of Commands)
    static Dictionary<string, List<string>> clients = new Dictionary<string, List<string>>();

    static async Task Main(string[] args)
    {
        using (HttpClient client = new HttpClient())
        {
            client.BaseAddress = new Uri("https://example.com/"); // Replace with your server URL

            Console.WriteLine("Starting to send GET requests every 10 seconds...");

            while (true)
            {
                await SendGetRequest(client);

                // Display all saved data
                ShowAllData();

                await Task.Delay(10000); // Wait 10 seconds
            }
        }
    }

    static async Task SendGetRequest(HttpClient client)
    {
        try
        {
            // Send the GET request
            HttpResponseMessage response = await client.GetAsync("api/data"); // Replace with your endpoint
            response.EnsureSuccessStatusCode();

            // Read the response as a string
            string responseBody = await response.Content.ReadAsStringAsync();

            // Parse the JSON response
            var data = JsonSerializer.Deserialize<ResponseData>(responseBody);

            if (data?.Data != null)
            {
                Console.WriteLine($"ID: {data.Data.Id}, Commands: {string.Join(", ", data.Data.Commands)}");

                // Save to the dictionary
                SaveData(data.Data);
            }
            else
            {
                Console.WriteLine("Invalid response format.");
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"GET Request failed: {ex.Message}");
        }
    }

    static void SaveData(DataInfo data)
    {
        if (!clients.ContainsKey(data.Id))
        {
            clients[data.Id] = new List<string>(data.Commands);
            Console.WriteLine($"Data saved: ID = {data.Id}, Commands = {string.Join(", ", data.Commands)}");
        }
        else
        {
            // Append new commands to the existing list if they are not already present
            foreach (var command in data.Commands)
            {
                if (!clients[data.Id].Contains(command))
                {
                    clients[data.Id].Add(command);
                }
            }
            Console.WriteLine($"Updated commands for ID = {data.Id}: {string.Join(", ", clients[data.Id])}");
        }
    }

    static void UseSavedData()
    {
        Console.WriteLine("\nUsing Saved Data:");
        foreach (var entry in clients)
        {
            Console.WriteLine($"ID: {entry.Key}, Commands: {string.Join(", ", entry.Value)}");
        }
    }

    // Function to display all IDs and commands
    static void ShowAllData()
    {
        Console.WriteLine("\nList of IDs and Commands:");
        if (clients.Count == 0)
        {
            Console.WriteLine("No data available.");
            return;
        }

        foreach (var entry in clients)
        {
            Console.WriteLine($"ID: {entry.Key}, Commands: {string.Join(", ", entry.Value)}");
        }
    }
}

// Define classes for deserializing the response JSON
public class ResponseData
{
    public DataInfo Data { get; set; }
}

public class DataInfo
{
    public string Id { get; set; }
    public List<string> Commands { get; set; }
}
