<?php

	// include Facebook and Facebook Ads library
	$fb_path = function_exists('libraries_get_path') ? libraries_get_path('facebook-php-sdk') : '/var/www/vhosts/boucado.space/httpdocs/sites/all/libraries/facebook-php-sdk';
	$fb_ads_path = function_exists('libraries_get_path') ? libraries_get_path('facebook-php-ads-sdk') : '/var/www/vhosts/boucado.space/httpdocs/sites/all/libraries/facebook-php-ads-sdk';

	require_once $fb_path . '/vendor/autoload.php';  
	require_once $fb_ads_path . '/vendor/autoload.php';

	// Add to header of your file
	use Facebook\Facebook;
	use Facebook\Exceptions\FacebookResponseException;
	use Facebook\Exceptions\FacebookSDKException;
	
	use FacebookAds\Api;
	
	// Accounts
	use FacebookAds\Object\AdUser;
	use FacebookAds\Object\Fields\AdUserFields;
	use FacebookAds\Object\AdAccount;
	use FacebookAds\Object\Fields\AdAccountFields;	

	// Campaigns
	use FacebookAds\Object\Campaign;
	use FacebookAds\Object\Fields\CampaignFields;
	use FacebookAds\Object\Values\AdObjectives;
	use FacebookAds\Object\Values\AdBuyingTypes;

	// Targeting
	use FacebookAds\Object\TargetingSearch;
	use FacebookAds\Object\Search\TargetingSearchTypes;
	
	
function _fbads_interestresults($string){
	
	$items = array_map('trim', explode(',', $string));
    $last_item = array_pop($items);
    $prefix = implode(', ', $items);
	
	$results = TargetingSearch::search(
	  $type = TargetingSearchTypes::INTEREST,
	  $class = null,
	  $query = $last_item,
	  $limit = array('limit' => 15)
	);
	
 //fbads_print_pre($results->getObjects());
	
	$matches = array();
	
	for ($i=0;$i < count($results);$i++) {
		$cat = "";
		$topic = "";
		if (isset($results->getObjects()[$i]->disambiguation_category)) {$cat =  " / " . $results->getObjects()[$i]->disambiguation_category;}
		if (isset($results->getObjects()[$i]->topic)) {$topic = " / " . $results->getObjects()[$i]->topic;}

		$newresult = $results->getObjects()[$i]->name . " (" . number_format($results->getObjects()[$i]->audience_size,0,',','.') . ") " . $cat . $topic;
		$key = !empty($prefix) ? $prefix . ', ' . $results->getObjects()[$i]->name : $results->getObjects()[$i]->name;
		$matches[$key] = check_plain($newresult);
	}

	drupal_json_output($matches);
}

function _fbads_interestsuggestionresults($string){
	$results = TargetingSearch::search(
	  $type = TargetingSearchTypes::INTEREST_SUGGESTION,
	  $class = null,
	  $query = null,
	  $params = array('interest_list' => array($string), 'limit' => 15)
	);
	
	$matches = array();
	
	for ($i=0;$i < count($results);$i++) {
		$matches[$results->getObjects()[$i]->name] = check_plain($results->getObjects()[$i]->name . " (" . number_format($results->getObjects()[$i]->audience_size,0,',','.') . ")");
	}

	drupal_json_output($matches);
}

function _fbads_interestvalidate($interestlist){ // <= array('japan','nix')
	$results = TargetingSearch::search(
	  $type = TargetingSearchTypes::INTEREST_VALIDATE,
	  $class = null,
	  $query = null,
	  $params = array('interest_list' => $interestlist)
	);
	
	return $results->getObjects();
}

function _fbads_countryresults($string){

	$term = trim(strip_tags($_GET['term']));
	
	$items = array_map('trim', explode(',', $term));
    $last_item = array_pop($items);
    $prefix = implode(', ', $items);
		
	$results = TargetingSearch::search(
	  $type = TargetingSearchTypes::GEOLOCATION,
	  $class = null,
	  $query = $last_item,
	  array('location_types' => array('country'),)
	);
	
	$matches = array();
	
	
	for ($i=0;$i < count($results);$i++) {
		$label= $results->getObjects()[$i]->name;
		$value = !empty($prefix) ? $prefix . ', ' . $results->getObjects()[$i]->name : $results->getObjects()[$i]->name;
		//$matches[$key] = check_plain($newresult);
		
		$row['name'] = $results->getObjects()[$i]->name;
		$row['code'] = $results->getObjects()[$i]->key;
		$row['value'] = $results->getObjects()[$i]->name;
		$row['label'] = $results->getObjects()[$i]->name;
		$matches[] = $row;
	}
	
	// fbads_print_pre($matches);
	
	drupal_json_output($matches);
}

function _fbads_regionsresults($string){
	$results = TargetingSearch::search(
	  $type = TargetingSearchTypes::GEOLOCATION,
	  $class = null,
	  $query = $string,
	  array('location_types' => array('region'),)
	);
	
	$matches = array();
	
	for ($i=0;$i < count($results);$i++) {
		$matches[$results->getObjects()[$i]->name] = check_plain($results->getObjects()[$i]->name . ", " . $results->getObjects()[$i]->country_code);
	}

	drupal_json_output($matches);
}

function _fbads_cityresults($string){
	$results = TargetingSearch::search(
	  $type = TargetingSearchTypes::GEOLOCATION,
	  $class = null,
	  $query = $string,
	  array('location_types' => array('city'),)
	);
	
	$matches = array();
	
	for ($i=0;$i < count($results);$i++) {
		$matches[$results->getObjects()[$i]->name] = check_plain($results->getObjects()[$i]->name);
	}

	drupal_json_output($matches);
}

function _fbads_zipresults($string){
	$results = TargetingSearch::search(
	  $type = TargetingSearchTypes::GEOLOCATION,
	  $class = null,
	  $query = $string,
	  array('location_types' => array('zip'),)
	);
	

	$matches = array();
	
	for ($i=0;$i < count($results);$i++) {
		if (isset($results->getObjects()[$i]->primary_city)) {
			$city = $results->getObjects()[$i]->primary_city;
		} else {
			$city = $results->getObjects()[$i]->region;
		}
		$matches[$results->getObjects()[$i]->key] = check_plain($results->getObjects()[$i]->country_code . "-" . $results->getObjects()[$i]->name . " " . $city);
	}

	drupal_json_output($matches);
}

function _fbads_localesresults($string){
	$results = TargetingSearch::search(
	  $type = TargetingSearchTypes::LOCALE,
	  $class = null,
	  $query = $string
	);
	
	$matches = array();
	
	for ($i=0;$i < count($results);$i++) {
		$matches[$results->getObjects()[$i]->name] = check_plain($results->getObjects()[$i]->name);
	}

	drupal_json_output($matches);
}

function _fbads_educationresults($string){

	$results = TargetingSearch::search(
	  $type = TargetingSearchTypes::EDUCATION,
	  $class = null,
	  $query = $string,
	  $limit = array('limit' => 15)
	);
	
	$matches = array();
	
	for ($i=0;$i < count($results);$i++) {
		$matches[$results->getObjects()[$i]->name] = check_plain($results->getObjects()[$i]->name);
	}

	drupal_json_output($matches);
}

function _fbads_majorresults($string){

	$results = TargetingSearch::search(
	  $type = TargetingSearchTypes::MAJOR,
	  $class = null,
	  $query = $string,
	  $limit = array('limit' => 15)
	);
	
	$matches = array();
	
	for ($i=0;$i < count($results);$i++) {
		$matches[$results->getObjects()[$i]->name] = check_plain($results->getObjects()[$i]->name);
	}

	drupal_json_output($matches);
}

function _fbads_employerresults($string){

	$results = TargetingSearch::search(
	  $type = TargetingSearchTypes::EMPLOYER,
	  $class = null,
	  $query = $string,
	  $limit = array('limit' => 15)
	);
	
	$matches = array();
	
	for ($i=0;$i < count($results);$i++) {
		$matches[$results->getObjects()[$i]->name] = check_plain($results->getObjects()[$i]->name);
	}

	drupal_json_output($matches);
}

function _fbads_positionresults($string){

	$results = TargetingSearch::search(
	  $type = TargetingSearchTypes::POSITION,
	  $class = null,
	  $query = $string,
	  $limit = array('limit' => 15)
	);
	
	$matches = array();
	
	for ($i=0;$i < count($results);$i++) {
		$matches[$results->getObjects()[$i]->name] = check_plain($results->getObjects()[$i]->name);
	}

	drupal_json_output($matches);
}

function _fbads_categorylist($category){
	$result = TargetingSearch::search(
  		TargetingSearchTypes::TARGETING_CATEGORY,
  		$category);

/*
  `life_events`,
  `politics`,
  `industries`,
  'markets',
  `income`,
  `net_worth`,
  `home_type`,
  `home_ownership`,
  `ethnic_affinity`,
  `generation`,
  `household_composition`,
  `moms`,
  `office_type`,
  `family_statuses`
  'user_device'
 */
 	
 	$options = array();
	
	foreach ($result->getObjects() as $key => $val) {
		if (isset($val->id)) {$options[$key]['id'] = $val->id;}
		$options[$key]['label'] = $val->name;
		if (isset($val->description)) {$options[$key]['size'] = $val->audience_size;}
		if (isset($val->description)) {$options[$key]['description'] = $val->description;}
		if (isset($val->platform)) {$options[$key]['platform'] = $val->platform;}
	}
 
	// fbads_print_pre($options);
	// fbads_print_pre($result->getObjects());
	
	return $options;	
}

function _fbads_interestlist($category){
	$result = TargetingSearch::search(
  		TargetingSearchTypes::TARGETING_CATEGORY,
  		$category);

/*
  `interests`,
  `bahaviors`,
 */

  $hierarchy = array(
     'win' => array('label' => 'Windows',
	       'children' => array(
	         'xp'    => array('label' => 'XP'),
	         'vista' => array('label' => 'Vista',
		           'children' => array(
		             'x86' => array('label' => '32-bits'),
		             'x64' => array('label' => '64-bits'),
		           ),
	         ),
	       ),
     ),
  );
  	
 	$options = array();
	
	foreach ($result->getObjects() as $key => $val) {
		//if (isset($val->id)) {$options[$key]['id'] = $val->id;}
		$count = count($val->path);
		if (!isset($options[$val->path[0]])) {
			$options[$val->path[0]] = array('label' => $val->path[0]);
		}
		if ($count == 2) {
			$options[$val->path[0]]['children'][$val->id."#".$val->path[1]] = array('label' => $val->path[1]);} 
		
		if ($count == 3) {
			if (!isset($options[$val->path[0]]['children'][$val->path[1]])){
				$options[$val->path[0]]['children'][$val->path[1]] = array('label' => $val->path[1]);
			}
			if (!isset($options[$val->path[0]]['children'][$val->path[1]]['children'][$val->path[2]])) {
				$options[$val->path[0]]['children'][$val->path[1]]['children'][$val->id."#".$val->path[2]] = array('label' => $val->path[2]);
			} else {
				$options[$val->path[0]]['children'][$val->path[1]]['children'][$val->path[2]]['children'][$val->id."#".$val->path[2]] = array('label' => $val->path[2]);
			}
		}
		if ($count == 4) {
			if (!isset($options[$val->path[0]]['children'][$val->path[1]])){
				$options[$val->path[0]]['children'][$val->path[1]] = array('label' => $val->path[1]);
			}
			if (!isset($options[$val->path[0]]['children'][$val->path[1]]['children'][$val->path[2]])) {
				$options[$val->path[0]]['children'][$val->path[1]]['children'][$val->path[2]] = array('label' => $val->path[2]);
			}
			if (!isset($options[$val->path[0]]['children'][$val->path[1]]['children'][$val->path[2]]['children'][$val->id])) {
				$options[$val->path[0]]['children'][$val->path[1]]['children'][$val->path[2]]['children'][$val->id."#".$val->path[3]] = array('label' => $val->path[3]);
			}
		}
	}
	
	//fbads_print_pre($options);
	//fbads_print_pre($hierarchy);
	//fbads_print_pre($result->getObjects());
	
	return $options;	
}

function _fbads_reach_estimate($targeting, $optimize = 'LINK_CLICKS', $currency = 'EUR'){
	
	//$targeting = '{"age_min":25,"geo_locations":{"cities":[{"key":2430536}]}}';
	$targeting = urlencode(json_encode($targeting));
	
	$ad_account_id = 'act_' . variable_get('fbads_ad_account');
    $ch = curl_init();
	$url = "https://graph.facebook.com/v2.5/$ad_account_id/reachestimate" .
      '?access_token=' . urlencode($_SESSION['facebook_access_token']) .
      '&targeting_spec=' . $targeting .
      '&optimize_for=' . $optimize .
	  '&currency=' . $currency;
	 
    curl_setopt($ch, CURLOPT_URL, $url);
	
	// fbads_print_pre($url);
	
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 2);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    $result = curl_exec($ch);
    curl_close($ch);
		
	return json_decode($result);
}
