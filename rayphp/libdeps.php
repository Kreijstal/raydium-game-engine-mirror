<?

// supported files : (depends_xxx style)

function depends_tri($filename)
{
//$f=@file($filename);
$fp=fopen($filename,"rt");
if($fp==false)
    {
    echo "Cannot open $filename";
    }

fgets($fp, 4096); // skip first line (version number)

while (!feof($fp)) 
    {
    $line = trim(fgets($fp, 4096));
    $line=explode(" ",$line);
    $tex=trim($line[count($line)-1]);
    if(substr($tex,0,4)!="rgb(")
	{
	$tex=trim($tex);
	$texs=explode(";",$tex);
	if(strlen($texs[0])) $ret[]=$texs[0];
	if(strlen($texs[1])) 
	    {
	    if(strpos($texs[1],"|")!==false)
		{
		$lm=explode("|",$texs[1]);
		$ret[]=$lm[2];
		}
	    else
		$ret[]=$texs[1];
	    }
	$ret=array_values(array_unique($ret));
	}
    }
fclose($fp);
$ret[]=$filename;

return $ret;
}

function depends_prt($filename)
{
$f=@file($filename);
for($i=0;$i<count($f);$i++)
    {
    $line=trim($f[$i]);
    if($line[0]=='/' && $line[1]=='/') continue;
    
    if(strpos($line,"texture") === false) continue;

    echo $line;
    $t=explode('=',$line);
    $t=trim($t[1]);
    $t=str_replace('"',"",$t);
    $t=str_replace(';',"",$t);
    $ret[]=$t;
    }

$ret[]=$filename;
return $ret;
}


// -----------------------------------------------

function depends_find($filename)
{
$tbl=explode(".",$filename);
$ext=trim($tbl[count($tbl)-1]);
if($ext=="tri") return depends_tri($filename);
if($ext=="prt") return depends_prt($filename);

// else ..
$ret[]=$filename;
return $ret;
}


?>
