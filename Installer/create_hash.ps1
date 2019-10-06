param([string] $output = "sha256.txt")
$hash=Get-FileHash $args[0] 
$hash | Format-List
$hash.hash > $output

