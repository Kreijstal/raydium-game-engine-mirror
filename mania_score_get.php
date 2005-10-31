<?
// this file is used by ManiaDrive to get
// best track time from CQFD Corp website.

$trackE=rawurlencode($track);

$url="http://maniadrive.cqfd-corp.org/score/?get=$trackE";
$res=file($url);
$res=explode('|',$res[0]);

$str=$res[0];
$str=explode(":",$str);
$score=$str[0]*3600 + $str[1]*60 + $str[2] + $str[3]/1000;
$player="{$res[1]}";
echo "$track: best score is '$score', by $player";
?>
