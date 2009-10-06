<?
// Add a file to the specified zip package

// params: $file (string, input), $zip_file (string, input), $status (integer, output)

$status=0;

//echo getcwd();

$zip = new ZipArchive;

$res=$zip->open($zip_file,ZipArchive::CREATE);
if($res!==TRUE)
	die("FAILED: Unable to create $zip_file from RayPHP");

$res=$zip->addFile($file_full_path,basename($file_full_path));

if(!$res)
	die("FAILED: Unable to add '$file_full_path' to '$zip_file'");

$zip->close();

$status=1;
echo "'$file_full_path' added to '$zip_file'";

?>