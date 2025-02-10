# Define task name and script path
$taskName = "AutoRunPowerShell"
$scriptPath = "C:\path\to\test.ps1"  # Change this to your actual script path

# Define the command to execute the PowerShell script silently
$taskAction = New-ScheduledTaskAction -Execute "powershell.exe" -Argument "-ExecutionPolicy Bypass -WindowStyle Hidden -File `"$scriptPath`""

# Define trigger (run at user logon)
$taskTrigger = New-ScheduledTaskTrigger -AtLogOn

# Define task settings (no admin required, runs in background)
$taskSettings = New-ScheduledTaskSettingsSet -AllowStartIfOnBatteries -DontStopIfGoingOnBatteries

# Register the task (for the current user, without admin rights)
Register-ScheduledTask -TaskName $taskName -Action $taskAction -Trigger $taskTrigger -Settings $taskSettings -Description "Runs test.ps1 at user login" -User $env:USERNAME

Write-Host "Task Scheduler entry created! test.ps1 will run at every login."
