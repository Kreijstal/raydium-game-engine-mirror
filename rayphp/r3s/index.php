<?
// Raydium Server Side repository script
// name this file "index.php", and place data in $data_dir directory.
require("config.inc.php");

function GorP($var)
{
global $_GET,$_POST;

if(isset($_POST[$var]))
    return $_POST[$var];

if(isset($_GET[$var]))
    return $_GET[$var];

return "";
}


function decompress_file($gz,$final)
{
$fp=gzopen($gz,"r");
if(!$fp) die("FAILED: Cannot open gz file");
while(!gzeof($fp))
    {
    $data.=gzread($fp,128);
    }
gzclose($fp);

$fp=fopen($final,"wb");
if(!$fp)
    {
    echo "FAILED: Cannot create file '$final' on this server";
    return false;
    }
fwrite($fp,$data);
fclose($fp);
unlink($gz);
return true;
}

function home()
{
global $data_dir,$upload_accept,$QUERY_STRING;

echo "<h2>R3S Raydium data repository</h2>\n";
echo "<h3>Available files:</h3>\n";
echo "<table border=0 cellpadding=2>";

 if ($dh = @opendir($data_dir)) 
 {
    while (($file = readdir($dh)) !== false) 
    {
	if($file[0]==".") continue;
	$size=filesize($data_dir.$file);
	$total_size+=$size;
	echo "<tr><td><b> $file </b></td><td align=right>$size byte(s)</td><td>&nbsp;</td><td>".date("Y-m-d H:i:s",filemtime($data_dir.$file))."</td></tr>";
    }
  closedir($dh);
 }
echo "<tr><td>&nbsp;</td></tr>";
echo "<tr><td><b>Total size</b></td><td align=right>".sprintf("%.2f",$total_size/1024/1024)." MB</td></tr";
echo "</table><br>";
echo "You can add this URL in your <i>rayphp/repositories.*</i> files.<br>";
if($upload_accept) $up="supports"; else $up="does not supports";
echo "This server $up data uploading.<br><br>";
echo "Yes, we needs a CSS file ;)<br>";
echo "Note: files are not sorted.<br>";
}



function main($file,$type,$username,$password,$data)
{
global $data_dir,$upload,$upload_accept,$brute_force_delay;
global $HTTP_POST_FILES;

if($type=="")
    {
    home();
    return;
    }



$file=rawurldecode($file);
$file=str_replace("/","",$file);
$file=$data_dir.$file;

if($type=="listRepos")
    {
    if($file==$data_dir)
	$file="$data_dir/*";
    foreach((array)glob($file) as $file)
	{
	if($file[0]!='.')
	    echo trim(str_replace("/","",substr($file,strlen($data_dir))))."\n";
	}
    return;
    }

// For all next operations, consider $file as mandatory ...
if($file=="") return;

if($type=="putGzip")
{
if(!$upload_accept)
    {
    echo "FAILED: Upload is not activated on this server !";
    return;
    }

sleep($brute_force_delay);

$username=rawurldecode($username);
$password=rawurldecode($password);

if($username!=$upload["user"] || $password!=$upload["pass"])
    {
    echo "FAILED: invalid user/pass ($username/$password)";
    return;
    }
$filegz=$file.".tmp.gz";
if(file_exists($HTTP_POST_FILES["data"]["tmp_name"]))
 {
 move_uploaded_file($HTTP_POST_FILES["data"]["tmp_name"], $filegz);
 }
else die("FAILED: Cannot find data in this request");

if(!decompress_file($filegz,$file)) return;
chmod($file,0664);

echo "+ SUCCESS: file uploaded";
return;
}

if(!file_exists($file)) die("FAILED: file not found");

if($type=="getGzip")
{
$fp=fopen($file,"rb");
if(!$fp) die("FAILED: file not found");
$data=fread($fp,filesize($file));
fclose($fp);

$tmp=tempnam("./","delme");
$fp=gzopen($tmp,"wb");
if(!$fp) return;
gzwrite($fp,$data);
gzclose($fp);
//$dat=gzencode($data);
//echo $dat;
//echo date("s");
readfile($tmp);
unlink($tmp);
//echo $tmp;
}

if($type=="getDate")
{
echo filemtime($file);
}
} // end main()

main(GorP("file"),GorP("type"),GorP("username"),GorP("password"),GorP("data"));
?>
