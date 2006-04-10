<?php
// This file is used by ManiaDrive to get
// score from CQFD Corp website.

require_once('./nusoap/nusoap.php');

// Webservice WSDL URL
$url = "http://maniadrive.raydium.org/ecrire/tools/maniadrive/WSManiaDrive.php?wsdl";

// set the proxy according to the configuration database
$proxy = str_pad('', 128);
raydium_parser_db_get("Generic-Proxy", $proxy, "");
$proxy=parse_url($proxy);

// Create the client instance
$client = new soapclient($url, true, $proxy["host"], $proxy["port"]);

// Check for an error
$err = $client->getError();
if ($err) {
    // Log the error 
    raydium_log('Constructor error: '.$err);
    exit();
} 

// Parameters
$parameters = array(
  'track' => $track
); 


// Call the SOAP method
$result = $client->call('ScoreGetTop', $parameters);

// Display the result
//print_r($result);


// Check for a fault
if ($client->fault) {
    raydium_log('Fault: '.$result);
} else {
    // Check for errors
    $err = $client->getError();
    if ($err) {
       raydium_log('Error: '.$err);
    } else {
        // Display the result
        raydium_log("TOP : ".$result["name"]." => " . $result["score"]);
        
        $str=explode(":",$result["score"]);
	$score=$str[0]*3600 + $str[1]*60 + $str[2] + $str[3]/1000;
	
	$player = "{$result["name"]}";
    }
}


?>

