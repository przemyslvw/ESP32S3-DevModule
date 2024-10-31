REM Title: File Organizer
REM Description: Sorts files by type into categorized folders (Images, Videos, Music, Documents)
REM Author: przemyslvw
REM Target: Windows (Powershell 2.0 or above)

DELAY 3000
GUI r
DELAY 750
STRING powershell
ENTER
DELAY 750
STRING $sourceFolder = "C:\Path\To\Your\Folder"
ENTER
STRING $imageExtensions = @("*.jpg", "*.jpeg", "*.png", "*.gif", "*.bmp", "*.tiff")
ENTER
STRING $videoExtensions = @("*.mp4", "*.avi", "*.mov", "*.wmv", "*.mkv")
ENTER
STRING $musicExtensions = @("*.mp3", "*.wav", "*.flac", "*.aac", "*.ogg")
ENTER
STRING $documentExtensions = @("*.pdf", "*.docx", "*.doc", "*.xlsx", "*.xls", "*.pptx", "*.txt")

ENTER
STRING $imageFolder = Join-Path -Path $sourceFolder -ChildPath "Images"
ENTER
STRING $videoFolder = Join-Path -Path $sourceFolder -ChildPath "Videos"
ENTER
STRING $musicFolder = Join-Path -Path $sourceFolder -ChildPath "Music"
ENTER
STRING $documentFolder = Join-Path -Path $sourceFolder -ChildPath "Documents"

ENTER
STRING New-Item -ItemType Directory -Force -Path $imageFolder, $videoFolder, $musicFolder, $documentFolder
ENTER

STRING foreach ($ext in $imageExtensions) {Move-Item -Path (Get-ChildItem -Path $sourceFolder -Recurse -Include $ext) -Destination $imageFolder -Force -ErrorAction SilentlyContinue}
ENTER
STRING foreach ($ext in $videoExtensions) {Move-Item -Path (Get-ChildItem -Path $sourceFolder -Recurse -Include $ext) -Destination $videoFolder -Force -ErrorAction SilentlyContinue}
ENTER
STRING foreach ($ext in $musicExtensions) {Move-Item -Path (Get-ChildItem -Path $sourceFolder -Recurse -Include $ext) -Destination $musicFolder -Force -ErrorAction SilentlyContinue}
ENTER
STRING foreach ($ext in $documentExtensions) {Move-Item -Path (Get-ChildItem -Path $sourceFolder -Recurse -Include $ext) -Destination $documentFolder -Force -ErrorAction SilentlyContinue}
ENTER

STRING Write-Host "Files sorted successfully into folders: Images, Videos, Music, Documents." -ForegroundColor Green
ENTER
DELAY 3000
ALT F4