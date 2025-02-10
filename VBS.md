# Define registry path and value name
$regPath = "HKCU:\Software\Microsoft\Windows\CurrentVersion\Run"
$valueName = "AutoRunPowerShell"
$scriptPath = "C:\path\to\test.ps1"  # Change to your actual script path

# Define a VBS script to run PowerShell silently
$vbsPath = "C:\path\to\run_silent.vbs"
$vbsContent = '@echo off
Dim shell
Set shell = CreateObject("WScript.Shell")
shell.Run "powershell.exe -ExecutionPolicy Bypass -WindowStyle Hidden -File ""' + $scriptPath + '""", 0, False'

# Save the VBS script to disk
$vbsContent | Out-File -FilePath $vbsPath -Encoding ASCII -Force

# Set registry key to run the VBS script instead of PowerShell directly
$command = "wscript.exe `"$vbsPath`""

# Add registry key without output
New-ItemProperty -Path $regPath -Name $valueName -Value $command -PropertyType String -Force | Out-Null
