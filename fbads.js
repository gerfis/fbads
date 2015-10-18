(function($) {
Drupal.behaviors.fbadsBehavior = {
  attach: function (context, settings) {

/*
    //code starts
    $("#country_autocomplete").click(function() {
      alert("Hello World");
    });
    //code ends
*/

	// countries
	var country_config = {
		source: "/fbads/countryresults",
		select: function(event, ui){
			console.log(ui);
			$("#country_autocomplete").val(ui.item.name);
			$("#countries_list").val($("#edit-countries").val()+","+ui.item.code);
//			$('<input />').attr({type: 'checkbox',id: 'foo',name: 'countries_list',value: ui.item.code}).appendTo($('#countries_list'));
			
			var $label = $("<label>").text(ui.item.name);
			var $input = $('<input type="text">').attr({type: 'checkbox', name: 'countries_list',value: ui.item.code});
			$input.appendTo($label);
			$('#countries_list').append($label);
			
			$.ajax({
			  url: "/fbads/addcountry",
			  data: { code: ui.item.code, location: ui.item.name }
			})
			  .done(function( msg ) {
			    alert( "Data Saved: " + msg );
			  });
		},
		minLength:1,
	};
	$("#country_autocomplete").autocomplete(country_config);
	
	// interests
	var interests_config = {
		source: "/fbads/interestsresults",
		select: function(event, ui){
			console.log(ui);
			$("#interests_autocomplete").val(ui.item.name);
			//$("#edit-countries").val($("#edit-countries").val()+","+ui.item.code);
		},
		minLength:1,
	};
	$("#interests_autocomplete").autocomplete(interests_config);	
  }
};
})(jQuery);
