using System;
using System.Text;

public static class RC4
{
    // RC4 encryption/decryption method that works with byte[] input and output
    public static byte[] Process(string key, byte[] inputBytes)
    {
        byte[] keyBytes = Encoding.UTF8.GetBytes(key);
        byte[] keySchedule = new byte[256];

        // Initialize the key schedule
        for (int i = 0; i < 256; i++)
        {
            keySchedule[i] = (byte)i;
        }

        int j = 0;
        for (int i = 0; i < 256; i++)
        {
            j = (j + keySchedule[i] + keyBytes[i % keyBytes.Length]) % 256;
            byte temp = keySchedule[i];
            keySchedule[i] = keySchedule[j];
            keySchedule[j] = temp;
        }

        byte[] outputBytes = new byte[inputBytes.Length];
        int i1 = 0, j1 = 0;
        for (int k = 0; k < inputBytes.Length; k++)
        {
            i1 = (i1 + 1) % 256;
            j1 = (j1 + keySchedule[i1]) % 256;
            byte temp = keySchedule[i1];
            keySchedule[i1] = keySchedule[j1];
            keySchedule[j1] = temp;

            byte ksj = keySchedule[(keySchedule[i1] + keySchedule[j1]) % 256];
            outputBytes[k] = (byte)(inputBytes[k] ^ ksj);
        }

        return outputBytes;
    }
}

class Program
{
    static void Main()
    {
        string key = "secretkey";
        string plaintext = "Hello, World!";

        // Encrypt the plaintext
        byte[] encryptedBytes = RC4.Process(key, Encoding.UTF8.GetBytes(plaintext));
        string encryptedBase64 = Convert.ToBase64String(encryptedBytes);
        Console.WriteLine("Encrypted (Base64): " + encryptedBase64);

        // To decrypt, decode the Base64 string to byte[] and pass it to the RC4 process
        byte[] encryptedData = Convert.FromBase64String(encryptedBase64);
        byte[] decryptedBytes = RC4.Process(key, encryptedData); // Now passing byte[] to decrypt
        string decrypted = Encoding.UTF8.GetString(decryptedBytes);
        Console.WriteLine("Decrypted: " + decrypted);
    }
}
