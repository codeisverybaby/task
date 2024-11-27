using System;
using System.IO;
using System.Net;
using System.Security.Cryptography;
using System.Text;

class AesHttpServer
{
    private static readonly string Key = "0123456789ABCDEF"; // 16 bytes key
    private static readonly string IV = "ABCDEF9876543210"; // 16 bytes IV

    static void Main(string[] args)
    {
        HttpListener listener = new HttpListener();
        listener.Prefixes.Add("http://localhost:12345/");
        listener.Start();
        Console.WriteLine("Server is listening on http://localhost:12345/");

        while (true)
        {
            HttpListenerContext context = listener.GetContext();
            Console.WriteLine("Client connected.");

            // Read the encrypted request
            using (StreamReader reader = new StreamReader(context.Request.InputStream, context.Request.ContentEncoding))
            {
                string encryptedMessage = reader.ReadToEnd();
                Console.WriteLine("Encrypted message received: " + encryptedMessage);

                // Decrypt the message
                string decryptedMessage = Decrypt(encryptedMessage);
                Console.WriteLine("Decrypted message: " + decryptedMessage);

                // Create an encrypted response
                string responseMessage = Encrypt("Acknowledged: " + decryptedMessage);
                byte[] responseBytes = Encoding.UTF8.GetBytes(responseMessage);

                // Send the encrypted response
                context.Response.ContentType = "text/plain";
                context.Response.ContentLength64 = responseBytes.Length;
                context.Response.OutputStream.Write(responseBytes, 0, responseBytes.Length);
                context.Response.OutputStream.Close();
            }
        }
    }

    public static string Encrypt(string plainText)
    {
        using (Aes aes = Aes.Create())
        {
            aes.Key = Encoding.UTF8.GetBytes(Key);
            aes.IV = Encoding.UTF8.GetBytes(IV);

            using (ICryptoTransform encryptor = aes.CreateEncryptor(aes.Key, aes.IV))
            {
                byte[] plainBytes = Encoding.UTF8.GetBytes(plainText);
                byte[] cipherBytes = encryptor.TransformFinalBlock(plainBytes, 0, plainBytes.Length);
                return Convert.ToBase64String(cipherBytes);
            }
        }
    }

    public static string Decrypt(string cipherText)
    {
        using (Aes aes = Aes.Create())
        {
            aes.Key = Encoding.UTF8.GetBytes(Key);
            aes.IV = Encoding.UTF8.GetBytes(IV);

            using (ICryptoTransform decryptor = aes.CreateDecryptor(aes.Key, aes.IV))
            {
                byte[] cipherBytes = Convert.FromBase64String(cipherText);
                byte[] plainBytes = decryptor.TransformFinalBlock(cipherBytes, 0, cipherBytes.Length);
                return Encoding.UTF8.GetString(plainBytes);
            }
        }
    }
}
