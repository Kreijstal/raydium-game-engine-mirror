<?
// gets file from repositories listed in repositories.list

// params: $filename (string, input), $force (integer, input),
// 	   $status (integer, output)

// This script must be placed in "rayphp" sub-directory.

require("libfile.php");

function getfile($file,$repos,$local)
{
global $force;

//echo "trying: $repos/$file : ";
$file=rawurlencode($file);

$req="$repos?file=$file&type=getDate";
$d=@file($req);
if($d!=false) $d=$d[0];

$ld=@filemtime($local);

if($ld>=$d && !$force && $d!=false && $ld!=false)
    {
    echo "Local file is the same or newer, abording. ($repos)";
    return false;
    }

$req="$repos?file=$file&type=getGzip";
$data=@file($req);
if($data==false)
    return false;

if(substr($data[0],0,6)=="FAILED")
    {
    echo $data[0]." ($repos)";
    return false;
    }

$data=implode("",$data);
echo "Found at '$repos'";
return $data;
}

$status=0; // sets status to "error", by default


filename_cut($filename,$file,$path);

echo "Using repositories to get '$file' file:";

$repos=read_repositories_file("rayphp/repositories.list");

for($i=0;$i<count($repos);$i++)
  {
  $r=trim($repos[$i]);
    if(valid_entry($r))
	if( ($data=getfile($file,$r,$path.$file)) )
	    {
	    $status=-1;
	    break;
	    }
  }
	
if($status==0)
    die("No valid repository found for this file, abording.");

$data=gzdecode($data);
$fp=@fopen($path.$file,"wb");
if(!$fp)
    die("Cannot create output file '$path$file', abording.");
fwrite($fp,$data);
fclose($fp);
$status=1;
?>