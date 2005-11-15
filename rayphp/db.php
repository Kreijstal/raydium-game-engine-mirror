<?
// Raydium database management

// --- in:
// $order : "get" or "set"
// $key : record key
// $data : data to write for a "set" order

// --- out:
// $ret : 1 means success, 0 failed
// $data : value for a "get" order

// gets and sets values in Raydium's db
$db=dba_open("raydium.db","c","flatfile");

if(!$db)
    {
    echo "Cannot open Raydium's database !";
    return;
    }


if($order=="get")
    {
    $r=dba_fetch($key,$db);
    if($r==false)
	$ret=0;
    else
	{
	$data="$r";
	$ret=1;
	}
    }

if($order=="set")
    {
    $r=dba_replace($key,$data,$db);
    if($r==false)
	$ret=0;
    else
	$ret=1;
    }

dba_sync($db);
dba_close($db);
?>
