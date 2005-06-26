<?

function read_repositories_file($repos)
{
$ret=@file($repos);
if(count($ret)==0)
    die("Cannot open $repos");

// may make $ret array "uniq"

return $ret;
}


function filename_cut($filename,&$file,&$path)
{
$t=explode("/",$filename);
$file=$t[count($t)-1];
$t[count($t)-1]="";
$path=implode("/",$t);
}

function valid_entry($r)
{
if($r[0]!='#' && strlen(trim($r))>0)
    return true;
return false;
}

function gzdecode($in)
{
$tmp="tmp.tmp";
$fp=fopen($tmp,"wb");
if(!$fp) return false;
fwrite($fp,$in);
fclose($fp);

$fp=gzopen($tmp,"rb");
if(!$fp) return false;
while(!gzeof($fp))
    {
    $data.=gzread($fp,128);
    }
gzclose($fp);
unlink($tmp);
return $data;
}

?>