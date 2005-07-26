<?
// lists files from the first valid repository

// params: $filter (string, input), $size (integer, input)
//         $list (string, output), $status (integer, output)

// This script must be placed in "rayphp" sub-directory.

require("libfile.php");

function listrepos($filter,$size,$repos)
{
$filter=rawurlencode($filter);

$req="$repos?file=$filter&type=listRepos";
$d=@file($req);
if($d===false) 
    return false;

echo "Listing from '$repos'";
return implode("",$d);
}

$status=0; // sets status to "error", by default
$repos=read_repositories_file("rayphp/repositories.list");
$list="";

for($i=0;$i<count($repos);$i++)
  {
  $r=trim($repos[$i]);
    if(valid_entry($r))
	if( ($data=listrepos($filter,$size,$r)) )
	    {
	    if(strlen($data)+1>=$size)
		{
		echo "$r : Not enought memory";
		}
	    else
		{
		$list="$data";
		$status=1;
		break;
		}
	    }
  }
	
if($status==0)
    die("No valid repository found for listing, abording.");

?>
