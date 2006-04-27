<?
// this file is used by ManiaDrive to get
// track list from CQFD Corp website.
require("rayphp/libfile.php");

$list="";
$nl="";


$ok=(integer)http_test(); // R3S function
if(!$ok)
    {
    // network unavailable, read from disk
    $list=file_get_contents("mania_tracklist.txt")."";
    //var_dump($list);
    die("");
    }

$url="http://maniadrive.cqfd-corp.org/score/list.php";
$file=file($url);

foreach($file as $t)
    {
    $list.=$nl.trim($t);
    $nl="\n";
    }

// save list to disk
$fp=fopen("mania_tracklist.txt","wb");
fwrite($fp,$list);
fclose($fp);

//var_dump($list);
?>
