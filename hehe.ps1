using System;
using System.Diagnostics;

class Program
{
    static void Main()
    {
        string script = @"
        $restOfScript = 'My content script'
        $encscr = [Convert]::ToBase64String([System.Text.Encoding]::UTF8.GetBytes($restOfScript))
        $encscr
        $Seed = 12345
        $MixedBase64 = [Text.Encoding]::ASCII.GetString(([Text.Encoding]::ASCII.GetBytes($encscr) | Sort-Object { Get-Random -SetSeed $Seed }))
        $MixedBase64
        ";

        RunPowerShellScript(script);
    }

    static void RunPowerShellScript(string script)
    {
        ProcessStartInfo psi = new ProcessStartInfo
        {
            FileName = "powershell",
            Arguments = $"-NoProfile -ExecutionPolicy Bypass -Command \"{script}\"",
            RedirectStandardOutput = true,
            UseShellExecute = false,
            CreateNoWindow = false
        };

        using (Process process = Process.Start(psi))
        {
            string output = process.StandardOutput.ReadToEnd();
            process.WaitForExit();
            Console.WriteLine(output);
        }
    }
}
