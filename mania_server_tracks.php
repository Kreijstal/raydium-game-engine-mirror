<?

// > $last
// < $track

//$tracks[]="finchtrack.mni";
$tracks[]="deathonion.mni";
$tracks[]="carot.mni";
$tracks[]="et_hop.mni";
$tracks[]="bennin.mni";
$tracks[]="simple.mni";
$tracks[]="simple8.mni";
$tracks[]="speedturn.mni";
$tracks[]="complex1.mni";
$tracks[]="bigjump.mni";

$last++;
if($last>=count($tracks))
    $last=0;

$track="".$tracks[$last];

?>
