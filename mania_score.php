<?
// this file is used by ManiaDrive to upload
// scores to CQFD Corp website.
// See how it's easy to cheat ? ... No interest then ;)

$scoreE=rawurlencode($score);
$trackE=rawurlencode($track);
$nameE=rawurlencode($name);
$versionE=rawurlencode($version);

$url="http://maniadrive.cqfd-corp.org/score/?track=$trackE&score=$scoreE&name=$nameE&version=$versionE";
$res=file($url);

if($res[0]=="OK")
    {
    echo "Score successfuly posted to Mania website";
    //echo "Visit http://skydiver.cqfd-corp.org/ to compare ;)";
    }
else raydium_log("^cScore posting failed : ".$res[0]); // echo == raydium_log

?>
