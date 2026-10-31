# Campus Group Finder - Lightweight PowerShell Web Server
# Zero dependencies needed: Uses standard Windows .NET HttpListener

$Port = 8080
$Folder = Split-Path -Parent $MyInvocation.MyCommand.Path

$Listener = New-Object System.Net.HttpListener
$Listener.Prefixes.Add("http://localhost:$Port/")

Write-Host "==========================================================" -ForegroundColor Cyan
Write-Host " 🎓 Campus Group Finder Web Server running!" -ForegroundColor Green
Write-Host " Open your browser at: http://localhost:$Port/" -ForegroundColor Yellow
Write-Host " Press Ctrl+C in this terminal window to stop the server." -ForegroundColor Gray
Write-Host "==========================================================" -ForegroundColor Cyan

try {
    $Listener.Start()
    # Automatically open the browser
    Start-Process "http://localhost:$Port/"

    while ($Listener.IsListening) {
        $Context = $Listener.GetContext()
        $Request = $Context.Request
        $Response = $Context.Response

        $UrlPath = $Request.Url.LocalPath.TrimStart('/')
        if ([string]::IsNullOrEmpty($UrlPath)) {
            $UrlPath = "index.html"
        }

        $FilePath = Join-Path $Folder $UrlPath

        if (Test-Path $FilePath -PathType Leaf) {
            $Ext = [System.IO.Path]::GetExtension($FilePath).ToLower()
            $ContentType = switch ($Ext) {
                ".html" { "text/html; charset=utf-8" }
                ".css"  { "text/css; charset=utf-8" }
                ".js"   { "application/javascript; charset=utf-8" }
                ".json" { "application/json; charset=utf-8" }
                ".png"  { "image/png" }
                ".svg"  { "image/svg+xml" }
                default { "application/octet-stream" }
            }

            $Bytes = [System.IO.File]::ReadAllBytes($FilePath)
            $Response.ContentType = $ContentType
            $Response.ContentLength64 = $Bytes.Length
            $Response.OutputStream.Write($Bytes, 0, $Bytes.Length)
        } else {
            $Response.StatusCode = 404
            $ErrorMsg = [System.Text.Encoding]::UTF8.GetBytes("404 Not Found")
            $Response.OutputStream.Write($ErrorMsg, 0, $ErrorMsg.Length)
        }

        $Response.OutputStream.Close()
    }
}
finally {
    $Listener.Stop()
}
