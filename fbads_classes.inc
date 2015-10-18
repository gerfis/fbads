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
	