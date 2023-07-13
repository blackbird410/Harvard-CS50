# Check the provided argument
if ($args.count -ne 1)
{
	echo "Wrong usage: ./dProjects.ps1 folder"
	exit 5
}

# Check the path existence 
if (!(Test-Path -Path $args[0]))
{
	echo "Directory $args[0] not found"
	exit 5
}

# Move to the directory
# cd $args[0]

# Run the script to download the files
$ScriptPath = Split-Path $MyInvocation.InvocationName
& "$ScriptPath\$($args[0])\get.ps1"

# Invoke-Expression -Command $ScriptPath 
