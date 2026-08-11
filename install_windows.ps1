$root = $PSScriptRoot
$bin = Join-Path $root "bin"

# Cria o diretório bin
if (-not (Test-Path $bin)) {
    New-Item -ItemType Directory -Path $bin | Out-Null
}

# Compila o executável
gcc -o (Join-Path $bin "ruffman.exe") `
    (Join-Path $root "src\main.c") `
    (Join-Path $root "src\ruffman.c") `
    (Join-Path $root "src\utils.c") `
    (Join-Path $root "src\operations.c") `
    -I(Join-Path $root "src\includes")

if ($LASTEXITCODE -ne 0) {
    Write-Host "Erro ao compilar."
    exit 1
}

# Obtém o PATH do usuário
$path = [Environment]::GetEnvironmentVariable("Path", "User")
$entries = $path -split ';' | Where-Object { $_ }

# Adiciona bin ao PATH
if ($entries -notcontains $bin) {
    $entries += $bin

    [Environment]::SetEnvironmentVariable(
        "Path",
        ($entries -join ';'),
        "User"
    )
}

# Cria o desinstalador
$uninstaller = Join-Path $root "uninstall_windows.ps1"

@"
`$bin = "$bin"

`$path = [Environment]::GetEnvironmentVariable("Path", "User")

`$entries = `$path -split ';' | Where-Object {
    `$_ -and `$_ -ne `$bin
}

[Environment]::SetEnvironmentVariable(
    "Path",
    (`$entries -join ';'),
    "User"
)

if (Test-Path `$bin) {
    Remove-Item `$bin -Recurse -Force
}

Write-Host "Ruffman desinstalado com sucesso."
"@ | Set-Content $uninstaller

Write-Host "Ruffman instalado com sucesso."