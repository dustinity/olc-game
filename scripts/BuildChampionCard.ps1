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

$bp = "/Game/UI/ChampionSelection/Widgets/WBP_ChampionCard.WBP_ChampionCard"
$rootRef = "$bp:WidgetTree.RootBorder"
$contentRef = "$bp:WidgetTree.CardContent"

function AddW {
    param([string]$JsonArgs)
    $r = Invoke-UeMcpTool -ToolsetName "UMGToolSet.UMGToolSet" -ToolName "AddWidget" -Arguments $JsonArgs
    $fixed = ($r -replace '\bTrue\b','true' -replace '\bFalse\b','false') | ConvertFrom-Json
    return $fixed.widget.refPath
}

function SetP {
    param([string]$Ref, [string]$Values)
    Write-Host "  SET $(Split-Path $Ref -Leaf) ..." -NoNewline
    Invoke-UeMcpTool -ToolsetName "editor_toolset.toolsets.object.ObjectTools" -ToolName "set_properties" `
        -Arguments "{\"instance\":{\"refPath\":\"$Ref\"},`"values`":$Values}" | Out-Null
    Write-Host " OK"
}

# 1. Add ChampionPortrait Image to CardContent
Write-Host "ADD ChampionPortrait (Image)" -NoNewline
$img = AddW '{"widgetBlueprint":{"refPath":"'$bp'"},"widgetClass":{"refPath":"/Script/UMG.Image"},"widgetDisplayName":"ChampionPortrait","parentWidget":{"refPath":"'$contentRef'"}}'
Write-Host " -> $img"

# 2. Add TextContent VerticalBox to CardContent
Write-Host "ADD TextContent (VerticalBox)" -NoNewline
$vbox = AddW '{"widgetBlueprint":{"refPath":"'$bp'"},"widgetClass":{"refPath":"/Script/UMG.VerticalBox"},"widgetDisplayName":"TextContent","parentWidget":{"refPath":"'$contentRef'"}}'
Write-Host " -> $vbox"

# 3. Add ChampionNameText to TextContent
Write-Host "ADD ChampionNameText (TextBlock)" -NoNewline
$name = AddW '{"widgetBlueprint":{"refPath":"'$bp'"},"widgetClass":{"refPath":"/Script/UMG.TextBlock"},"widgetDisplayName":"ChampionNameText","parentWidget":{"refPath":"'$vbox'"}}'
Write-Host " -> $name"

# 4. Add ChampionRoleText to TextContent
Write-Host "ADD ChampionRoleText (TextBlock)" -NoNewline
$role = AddW '{"widgetBlueprint":{"refPath":"'$bp'"},"widgetClass":{"refPath":"/Script/UMG.TextBlock"},"widgetDisplayName":"ChampionRoleText","parentWidget":{"refPath":"'$vbox'"}}'
Write-Host " -> $role"

# 5. Configure properties
SetP -Ref $rootRef -Values '{"background":{"drawAs":"Image","resourceObject":{"refPath":"/Game/UI/ChampionSelection/Textures/T_CS_Button_Champion_Inactive.T_CS_Button_Champion_Inactive"},"imageSize":{"x":480,"y":96}}}'

SetP -Ref $img -Values '{"source":{"resourceObject":{"refPath":"/Game/UI/ChampionSelection/Textures/T_CS_Placeholder_Portrait.T_CS_Placeholder_Portrait"}}}'

SetP -Ref $name -Values '{"text":{"textData":{"literal":"CHAMPION NAME"}},"font":{"typeface":{"size":20}}}'

SetP -Ref $role -Values '{"text":{"textData":{"literal":"Role"}},"font":{"typeface":{"size":14}}}'

# 6. Compile
Write-Host "Compile ..." -NoNewline
Invoke-UeMcpTool -ToolsetName "UMGToolSet.UMGToolSet" -ToolName "CompileWidgetBlueprint" `
    -Arguments '{"widgetBlueprint":{"refPath":"'$bp'"}}' | Out-Null
Write-Host " OK"

# 7. Save
Write-Host "Save ..." -NoNewline
Invoke-UeMcpTool -ToolsetName "editor_toolset.toolsets.asset.AssetTools" -ToolName "save_assets" `
    -Arguments '{"asset_paths":["'$bp'"]}' | Out-Null
Write-Host " OK"

Write-Host ""
Write-Host "WBP_ChampionCard built!"
