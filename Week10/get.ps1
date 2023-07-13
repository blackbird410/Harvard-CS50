
# Check if file containing links are provided

$file = "links.txt"

# Check if the file exit in the current folder
if (!(Test-Path -Path $file -PathType Leaf))
{
	echo "File $file not found. Please try again!"
	exit 5 	
}

$fileCount = 0

# Open the file and parse the links
foreach($link in Get-Content $file)
{
	# Split the link text to get the name for the downloaded file
	$linkParts = $link.Split("/")
	$fileName = $linkParts[-1]
	$destination = $fileName.Split(".")[0]

	# Download the zip file
	Invoke-WebRequest -Uri $link -Outfile $fileName

	# Check if the folder already exist 
	if (!(Test-Path -Path $destination -PathType Container))
	{
		# If not extract and create the folder
		Expand-Archive $fileName -DestinationPath $destination
	}

	# Remove the zip file
	Remove-Item $fileName

	$fileCount++ 
}

echo "`n"

# Make a report of downloaded files
echo "Downloaded files : $fileCount`nProcess terminated..."
exit 5

