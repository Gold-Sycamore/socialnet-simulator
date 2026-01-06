Param(
  [string]$Src = "main.cpp",
  [string]$InputFile = ""
)

$Out = "main.exe"

if (-not (Test-Path $Src)) {
  Write-Error "Source not found: $Src"
  exit 1
}

# Pick a compiler
$compiler = if (Get-Command g++ -ErrorAction SilentlyContinue) { "g++" }
            elseif (Get-Command clang++ -ErrorAction SilentlyContinue) { "clang++" }
            else {
              Write-Error "Neither g++ nor clang++ found in PATH. Install MinGW-w64 or LLVM."
              exit 1
            }

Write-Host "[build] $compiler -std=c++17 -O2 -Wall `"$Src`" -o `"$Out`""
& $compiler -std=c++17 -O2 -Wall "$Src" -o "$Out"
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

Write-Host "[run] $Out"
if ($InputFile -and (Test-Path $InputFile)) {
  # Feed the file to stdin (safer than inline redirection inside scripts)
  Get-Content -Raw "$InputFile" | & ".\${Out}"
} else {
  & ".\${Out}"
}
