<?
// ManiaDrive, CQFD Corp.
// Generates story track list for ingame menu

$ret="";
$scores=array();


function get_data($mni)
{
$a=file($mni);
for($i=0;$i<count($a);$i++)
    {
    $l=$a[$i];
    if($l[0]=='d') // is Data
	{
	$l=substr($l,2);
	$l=explode(';',$l);
	return $l;
	}
    }
return array("$mni","unknown","0","0");
}


function get_best($mni)
{
global $scores;
$best=999999;

foreach($scores as $s)
    {
    $a=explode(";",$s);
    if($a[0]==$mni &&  $a[1]<$best)
	{
	$best=trim($a[1]);
	}
    }
return $best;
}

///////////// main

$story=file("mania_drive.story");

if(file_exists("mania_drive.state"))
    $scores=file("mania_drive.state");

$ret="";
$col='2';
foreach($story as $s)
    {
    $d=get_data($s);
    $b=get_best($s);

    $ret.="$s;^$col".$d[0]."  \n";
    if($b>$d[2])
	$col='4';
    }

?>
