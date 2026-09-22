# Make an copy of all the config file before
param(
    [string]$Place,
    [string]$Mode
)

Write-Output "test output"


$ResourceFilePath=""

$ResourceFilePath = Join-Path $PSScriptRoot "\x64\$Place-$Mode\resources.cfg"
Write-Output $ResourceFilePath
Write-Output $Place

if(-not (Test-Path $ResourceFilePath)){
    Write-Warning "No resource file found"
    exit 0
}
else {
    $Content = Get-Content -Path $ResourceFilePath

    if($Place -eq "Home"){
        $Content = $Content -replace "\$\$\$", "..\..\lib"
    }
    elseif($Place -eq "Lab"){
        $Content = $Content -replace "\$\$\$", "C:\software\programacion"
    }

    Set-Content -Path $ResourceFilePath -Value $Content

    Write-host "Replaced with success!"
}
