# Resolve UeMcp.ps1 location: prefer env var override, then try relative path from olc-game to techstack.
$UeMcpPath = if ($env:TECHSTACK_ROOT) {
    Join-Path $env:TECHSTACK_ROOT "unreal-engine/scripts/UeMcp.ps1"
} elseif (Test-Path "$PSScriptRoot/../../../techstack/unreal-engine/scripts/UeMcp.ps1") {
    # Relative from olc-game/scripts/ up through Projects/ to agent-bob/ techstack
    Resolve-Path "$PSScriptRoot/../../../techstack/unreal-engine/scripts/UeMcp.ps1"
} else {
    throw "UeMcp.ps1 not found. Set `$env:TECHSTACK_ROOT to point to the techstack root (e.g., /home/chbe/agent-bob/techstack)."
}

. $UeMcpPath

$bp = '/Game/UI/ChampionSelection/Widgets/WBP_ChampionDossier.WBP_ChampionDossier'
$tex = '/Game/UI/ChampionSelection/Textures'
$root = $bp + ':WidgetTree.RootBorder'

function AW {
    param([string]$Class, [string]$Name, [string]$Parent)
    if ($Parent) {
        $j = '{"widgetBlueprint":{"refPath":"' + $bp + '"},"widgetClass":{"refPath":"/Script/UMG.' + $Class + '"},"widgetDisplayName":"' + $Name + '","parentWidget":{"refPath":"' + $Parent + '"}}'
    } else {
        $j = '{"widgetBlueprint":{"refPath":"' + $bp + '"},"widgetClass":{"refPath":"/Script/UMG.' + $Class + '"},"widgetDisplayName":"' + $Name + '"}'
    }
    $r = Invoke-UeMcpTool -ToolsetName 'UMGToolSet.UMGToolSet' -ToolName AddWidget -Arguments $j
    $fixed = ($r -replace '\bTrue\b','true' -replace '\bFalse\b','false') | ConvertFrom-Json
    return $fixed.returnValue.widget.refPath
}

function SetProps {
    param([string]$Ref, [string]$Values)
    Write-Host "  SET $(Split-Path $Ref -Leaf) ..." -NoNewline
    $j = '{"instance":{"refPath":"' + $Ref + '"},"values":' + $Values + '}'
    Invoke-UeMcpTool -ToolsetName 'editor_toolset.toolsets.object.ObjectTools' -ToolName set_properties -Arguments $j | Out-Null
    Write-Host " OK"
}

Write-Host ""
Write-Host "=== WBP_ChampionDossier ==="

# RootBorder already exists from previous call

# 2. Content VerticalBox inside RootBorder
Write-Host "ADD Content (VerticalBox)" -NoNewline
$content = AW -Class VerticalBox -Name Content -Parent $root
Write-Host " -> $content"

# Set RootBorder background to Dossier panel texture
SetProps -Ref $root -Values '{"background":{"drawAs":"Image","resourceObject":{"refPath":"' + $tex + '/T_CS_Panel_Dossier.T_CS_Panel_Dossier"}}}'

# 3. Portrait Image
Write-Host "ADD Portrait (Image)" -NoNewline
$portrait = AW -Class Image -Name Portrait -Parent $content
Write-Host " -> $portrait"
SP -Ref $portrait -Values '{"source":{"resourceObject":{"refPath":"' + $tex + '/T_CS_Placeholder_Portrait.T_CS_Placeholder_Portrait"}}}'

# 4. DisplayName TextBlock
Write-Host "ADD DisplayName (TextBlock)" -NoNewline
$displayName = AW -Class TextBlock -Name DisplayName -Parent $content
Write-Host " -> $displayName"
SP -Ref $displayName -Values '{"text":{"textData":{"literal":"Champion Name"}},"font":{"typeface":{"size":24}}}'

# 5. Role TextBlock
Write-Host "ADD Role (TextBlock)" -NoNewline
$role = AW -Class TextBlock -Name Role -Parent $content
Write-Host " -> $role"
SP -Ref $role -Values '{"text":{"textData":{"literal":"Role"}},"font":{"typeface":{"size":16}}}'

# 6. StatsContainer VerticalBox
Write-Host "ADD StatsContainer (VerticalBox)" -NoNewline
$stats = AW -Class VerticalBox -Name StatsContainer -Parent $content
Write-Host " -> $stats"

# 7. Rating labels + progress bars
foreach ($stat in @('Combat', 'Engineering', 'Mobility', 'Survival')) {
    Write-Host "ADD $($stat)Label (TextBlock)" -NoNewline
    $label = AW -Class TextBlock -Name ($stat + 'Label') -Parent $stats
    Write-Host " -> $label"
    SetProps -Ref $label -Values '{"text":{"textData":{"literal":"' + $stat + '"}},"font":{"typeface":{"size":12}}}'

    Write-Host "ADD $($stat)Bar (ProgressBar)" -NoNewline
    $bar = AW -Class ProgressBar -Name ($stat + 'Bar') -Parent $stats
    Write-Host " -> $bar"
}

# 8. AbilitiesContainer HorizontalBox
Write-Host "ADD AbilitiesContainer (HorizontalBox)" -NoNewline
$abilities = AW -Class HorizontalBox -Name AbilitiesContainer -Parent $content
Write-Host " -> $abilities"

# 9. Compile & Save
Write-Host "Compile ..." -NoNewline
Invoke-UeMcpTool -ToolsetName 'UMGToolSet.UMGToolSet' -ToolName CompileWidgetBlueprint `
    -Arguments ('{"widgetBlueprint":{"refPath":"' + $bp + '"}}') | Out-Null
Write-Host " OK"

Write-Host "Save ..." -NoNewline
$savePath = '/Game/UI/ChampionSelection/Widgets/WBP_ChampionDossier'
Invoke-UeMcpTool -ToolsetName 'editor_toolset.toolsets.asset.AssetTools' -ToolName save_assets `
    -Arguments ('{"asset_paths":["' + $savePath + '"]}') | Out-Null
Write-Host " OK"

Write-Host ""
Write-Host "=== WBP_ChampionDossier built! ==="
