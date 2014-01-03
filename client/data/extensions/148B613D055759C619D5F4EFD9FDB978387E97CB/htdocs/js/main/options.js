Osiris.Options =
{
	countersFilter: new Array(),
	countersTotal: new Array(),

	init: function () {
		$('#filterinput').change(function () {
			Osiris.Options.doFilter(false, false);
		});

		$('#filterinput').keyup(function () {
			Osiris.Options.doFilter(false, false);
		});

		$('#options_tab').get(0).onSwitchPage = function () {
			Osiris.Options.doFilter(false, true);
		}

		Osiris.Options.doFilter(true);
	},

	doFilter: function (first, istant) {
		Osiris.Options.countersFilter = new Array();

		var search = $("#filterinput").val();
		//search = "p2p";

		$(".os_area_content").find(".os_option").each(function () {
			var option = $(this);
			//alert(option);

			var pageName = option.parents("[data-os-tabtype=body]").attr("data-os-optionPage");
			//alert(pageName);				

			var pageHeader = $("#" + pageName);

			pageHeader.fadeTo(Osiris.effectsSpeedSlow, 0.1);

			var counter = $("#" + pageName + "_counter");
			//var counterDom = counter.get(0);
			if (pageName in Osiris.Options.countersFilter) {
			}
			else {
				Osiris.Options.countersFilter[pageName] = 0;
				if (first)
					Osiris.Options.countersTotal[pageName] = 0;
			}

			var text = $(this).text();

			var show = true;
			if ((search != "") && (text.indexOf(search) == -1))
				show = false;

			if (show)
				Osiris.Options.countersFilter[pageName]++;
			if (first)
				Osiris.Options.countersTotal[pageName]++;

			var speedEffect = Osiris.effectsSpeedSlow;
			if (istant)
				speedEffect = 0;

			if (show) {
				//pageHeader.fadeTo(Osiris.effectsSpeedSlow, 1);
				$(this).slideDown(Osiris.speedEffect);
			}
			else {
				$(this).slideUp(Osiris.speedEffect);
			}
			
			counter.html(Osiris.Options.countersFilter[pageName] + " / " + Osiris.Options.countersTotal[pageName]);
			counter.removeClass("os_options_counter_zero");
			counter.removeClass("os_options_counter_found");
			if (Osiris.Options.countersFilter[pageName] != 0)
				counter.addClass("os_options_counter_found");
			else
				counter.addClass("os_options_counter_zero");
		});
	}
}

$(function() {
 // Handler for .ready() called.
 	Osiris.Options.init();
});