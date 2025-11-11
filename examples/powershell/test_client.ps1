# Simple PowerShell test client for Flight Sim Server

$client = New-Object System.Net.Sockets.TcpClient
$client.Connect("localhost", 9090)

$stream = $client.GetStream()
$reader = New-Object System.IO.StreamReader($stream)
$writer = New-Object System.IO.StreamWriter($stream)
$writer.AutoFlush = $true

Write-Host "Connected to server" -ForegroundColor Green

# Read welcome message
$welcome = $reader.ReadLine()
Write-Host "Server says: $welcome" -ForegroundColor Cyan

# Send control command
$command = '{"type":"control","throttle":0.8,"elevator":0.1,"aileron":0.0,"rudder":0.0}'
Write-Host "`nSending: $command" -ForegroundColor Yellow
$writer.WriteLine($command)

# Read responses for 5 seconds
$stopwatch = [System.Diagnostics.Stopwatch]::StartNew()
$count = 0
while ($stopwatch.Elapsed.TotalSeconds -lt 5 -and $count -lt 10) {
    if ($stream.DataAvailable) {
        $response = $reader.ReadLine()
        Write-Host "Received: $response" -ForegroundColor White
        $count++
    }
    Start-Sleep -Milliseconds 100
}

$client.Close()
Write-Host "`nDisconnected" -ForegroundColor Green
