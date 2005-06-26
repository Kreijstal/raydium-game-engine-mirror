<?
// gets file from repositories listed in repositories.list

// params: $filename (string, input), $depends (integer, input)
// 	   $status (integer, output)

// This script must be placed in "rayphp" sub-directory.

require("libfile.php");
require("libdeps.php");

$status=0; // sets status to "error", by default

//$filename="buggy.tri"; // used when debugging outside of Raydium

// I use a "static" value for now, may randomly generate it in the future ...
$boundary="BREAKmessageHEREthisISaBOUNDARY";


// search first data line in $packer (HTTP server answer)
function http_search_data($packet,&$first)
{
$data_start=1;
$packet=explode("\n",$packet);

$first=trim($packet[0]);
for($i=0;$i<count($packet);$i++)
    if(trim($packet[$i])=="")
	break;

for($j=0;$j<$i;$j++)
    if(trim($packet[$j]) == "Transfer-Encoding: chunked")
	$data_start=2;

return trim($packet[$i+$data_start]);
}

function http_make_part($name,$value,$binary=false)
{
global $boundary;

if($binary)
    $filename="; filename=\"data.bin\"";
else $filename="";

$ret ="--$boundary\r\n";
$ret.="Content-Disposition: form-data; name=\"$name\"$filename\r\n";
if($binary)
$ret.="Content-Type: application/octet-stream\r\n";
$ret.="\r\n";
$ret.="$value";
$ret.="\r\n";

return $ret;
}

function ftp_upload($repos,$local,$distant)
{
/*
$ret=@copy($local,"$repos/$distant");
if($ret==false)
    {
    echo "Failed contacting $repos";
    return false;
    }
echo "$repos: SUCCESS";
return true;
*/
$url=parse_url($repos);

$conn_id = ftp_connect($url["host"],$url["port"]);
$login_result = ftp_login($conn_id, $url["user"], $url["pass"]);
ftp_pasv($conn_id, true);
//echo ".";
if (ftp_put($conn_id,$url["path"].$distant, $local, FTP_ASCII))
 {
 echo "$repos: SUCCESS";
 return true;
 }
else
 {
 echo "Failed contacting $repos";
 return false;
 }
ftp_close($conn_id);
}

function http_upload($repos,$local,$distant)
{
global $boundary,$depends;
$data=@file($local);
$data=implode("",$data);
$data=gzencode($data);

$url=parse_url($repos);
$distant=rawurlencode($distant);
$user=rawurlencode($url["user"]);
$pass=rawurlencode($url["pass"]);

$path=$url["path"];
if($url["port"]=="") $url["port"]=80;
$req="POST $path HTTP/1.1\r\n";

$fp = @fsockopen($url["host"],$url["port"]);
if(!$fp) 
    {
    echo "Failed contacting $repos";
    return false;
    }
$ret="";
fputs($fp,$req);
fputs($fp,"Host: ".$url["host"]."\r\n");
fputs($fp,"User-Agent: Raydium PHP Upload Script\r\n");

fputs($fp,"Content-Type: multipart/form-data; boundary=$boundary\r\n");

$packet =http_make_part("file",$distant);
$packet.=http_make_part("type","putGzip");
$packet.=http_make_part("data",$data,true);
$packet.=http_make_part("username",$user);
$packet.=http_make_part("password",$pass);

$packet.="--$boundary--\r\n";

$len=strlen($packet);
fputs($fp,"Content-Length: $len\r\n");
fputs($fp,"Connection: Close\r\n");
fputs($fp,"\r\n");
fputs($fp,$packet);

while(!feof($fp)) 
    {
    $ret.=fgets($fp,128);
    }
fclose($fp);

$ret=http_search_data($ret,$first);
echo "$repos: $ret";
if($ret[0]=='+') return true;
echo "HTTP reply: $first";
return false;
}


$repos=read_repositories_file("rayphp/repositories.upload");

if($depends)
$deps=depends_find($filename);
else $deps[]=$filename;

for($j=0;$j<count($deps);$j++)
{
filename_cut($deps[$j],$file,$path);

if(!file_exists($path.$file) || !is_readable($path.$file))
    {
    echo "Cannot upload '$path$file': file not exists or invalid rights";
    continue;
    }

echo "Using repositories to upload '$file' file:";

 for($i=0;$i<count($repos);$i++)
  {
  $r=trim($repos[$i]);
    if(valid_entry($r))
	{
	// http or ftp ?
	$type=parse_url($r);
	$type=$type["scheme"];
	
	if(($type=="ftp" ||
	    $type=="ftps" )
	    && ftp_upload($r,$file.$path,$file))
	    {
	    touch($path.$file);
	    $status++;
	    break;
	    }
	
	if(($type=="http" ||
	    $type=="https" )
	&& http_upload($r,$file.$path,$file))
	    {
	    touch($path.$file);
	    $status++;
	    break;
	    }	
	}
  }
}

if($status==count($deps))
    {
    echo "All files uploaded ($status)";
    return;
    }    

if($status==0)
    {
    echo "No file uploaded";
    return;
    }    

echo "Only $status/".count($deps)." file(s) uploaded";
?>
