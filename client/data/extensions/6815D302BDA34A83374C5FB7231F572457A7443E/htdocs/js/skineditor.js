Osiris.SkinEditor =
{
	propList: new Object(),
	propListControl: new Object(),
	propListControlEnabled: new Object(),

	/*
	prop: new Object(),
	propType: new Object(),
	propControl: new Object(),    
	*/

	propPage: new Object(),


	// json default value, normally stored in localstorage.
	presets: { "test": {} },

	disableApply: false,

	applyDelay: 100,

	lastChange: 0, // tickcount of last change

	mergeProp: function (b) {

		for (var i in b) {
			//this.prop[i] = b[i];
			Osiris.SkinEditor.propList[i].value = b[i].value;
			Osiris.SkinEditor.propList[i].enabled = b[i].enabled;
		}
	},

	init: function () {

		$("div").addClass("transitions-all");

		Osiris.storageRemove("osiris.skinEditor.current");
		Osiris.SkinEditor.presets = Osiris.storageGet("skinEditor.presets", Osiris.SkinEditor.presets);

		$(".os_skinEditor_item").each(function (index) {
			var propname = $(this).attr("data-os-propname");
			var proptype = $(this).attr("data-os-proptype");
			var prop = new Object();
			prop.type = $(this).attr("data-os-proptype");
			prop.enabled = false;
			Osiris.SkinEditor.propList[propname] = prop;
			Osiris.SkinEditor.propListControl[propname] = $(this).find(".os_skinEditor_itemControl").first();
			Osiris.SkinEditor.propListControlEnabled[propname] = $(this).find(".os_skinEditor_itemEnable").first();

			/*
			Osiris.SkinEditor.prop[propname] = "";
			Osiris.SkinEditor.propType[propname] = proptype;
			Osiris.SkinEditor.propControl[propname] = $(this).find(".os_skinEditor_itemControl").first();
			*/
		});

		console.log("1:" + Osiris.SkinEditor.propList["os-skin-page-bg"].value);

		/*
		// Properties initialization
		$(".os_skinEditor_color").each(function (index) {
		var propname = $(this).attr('id').replace("skinEditor_", "");
		Osiris.SkinEditor.prop[propname] = "";
		Osiris.SkinEditor.propType[propname] = "color";
		});
		$(".os_skinEditor_size").each(function (index) {
		var propname = $(this).attr('id').replace("skinEditor_", "");
		Osiris.SkinEditor.prop[propname] = "";
		Osiris.SkinEditor.propType[propname] = "size";
		});
		$(".os_skinEditor_type").each(function (index) {
		var propname = $(this).attr('id').replace("skinEditor_", "");
		Osiris.SkinEditor.prop[propname] = "";
		Osiris.SkinEditor.propType[propname] = "type";
		});
		*/

		// Read page values
		console.log("clone issue");
		Osiris.SkinEditor.propPage = Osiris.cloneVar(Osiris.SkinEditor.propList);
		//this.loadCurrent();        

		// Connect to Less to receive their data
		less.customParse = Osiris.SkinEditor.parseLess;
		less.refresh(true);


		//alert(Osiris.SkinEditor.prop["os-skin-page-bg"]);

		// Read current values        
		Osiris.SkinEditor.mergeProp(Osiris.storageGet("skinEditor.current", Osiris.SkinEditor.propPage));
		//alert(Osiris.SkinEditor.prop["os-skin-page-bg"]);

		console.log("1a:" + Osiris.SkinEditor.propList["os-skin-page-bg"].value);

		this.applyLess(true);

		console.log("2a:" + Osiris.SkinEditor.propList["os-skin-page-bg"].value);

		// Controls initialization        
		for (var propname in Osiris.SkinEditor.propList) {
			Osiris.SkinEditor.initControl(propname);
			Osiris.SkinEditor.itemEnabled(propname, Osiris.SkinEditor.propList[propname].enabled);
		}

		console.log("2:" + Osiris.SkinEditor.propList["os-skin-page-bg"].value);

		// Some properties are applied in realtime via jQuery; when too complex, all CSS/LESS are reloaded.         

		//$("#skinEditor_os-skin-page-bg").get(0).applyProp = function () {
		Osiris.SkinEditor.propListControl["os-skin-page-bg"].get(0).applyProp = function () {
			$(".os_page").css("background-color", Osiris.SkinEditor.getFinalPropValue("os-skin-page-bg"));
		};

		Osiris.SkinEditor.propListControl["os-skin-page-size"].get(0).applyProp = function () {
			//$("#skinEditor_os-skin-page-size").get(0).applyProp = function () {
			$(".os_page").css("width", Osiris.SkinEditor.getFinalPropValue("os-skin-page-size"));
		};

		Osiris.SkinEditor.propListControl["os-skin-outside-bg"].get(0).applyProp = function () {
			$(".os_page_container").css("background-color", Osiris.SkinEditor.getFinalPropValue("os-skin-outside-bg"));
		};


		$("#skinEditor_preset").val(Osiris.storageGet("skinEditor.current_preset", ""));


		// Open Dialog
		$("#skinEditor").dialog({ width: 550, height: 500 }).parent().css('position', 'fixed');

		// Workaround, to maintain "fixed" also on resize.
		//$("#skinEditor").bind("dialogresize", function () {
		//$("#skinEditor").parent().css('position', 'fixed');
		//});

		jQuery("#file_import").live("change", function () { Osiris.SkinEditor.handleFileImport(event); });

		setInterval("Osiris.SkinEditor.applyLessTimer();", this.applyDelay);

		console.log("3:" + Osiris.SkinEditor.propList["os-skin-page-bg"].value);
	},

	initControl: function (propname) {

		/*
		var proptype = this.propType[propname];
		var propvalue = Osiris.SkinEditor.prop[propname];
		//var obj = $("#skinEditor_" + propname);
		var obj = Osiris.SkinEditor.propControl[propname];
		*/
		var proptype = Osiris.SkinEditor.propList[propname].type;
		var propvalue = Osiris.SkinEditor.propList[propname].value;
		var obj = Osiris.SkinEditor.propListControl[propname];
		var objEnabled = Osiris.SkinEditor.propListControlEnabled[propname];

		if (obj == null)
			return;

		var objDom = obj.get(0);
		if (objDom == null)
			return;

		objDom.propname = propname;

		objDom.applyProp = function () {
			var d = new Date();
			Osiris.SkinEditor.lastChange = d.getTime();
		}

		// Enabled
		objEnabled.get(0).propname = propname;
		objEnabled.change(function () {
			Osiris.SkinEditor.itemEnabled(propname, $(this).is(":checked"));
			Osiris.SkinEditor.propListControl[propname].get(0).applyProp();

			Osiris.SkinEditor.change2(propname);
		});

		if (proptype == "color") {
			//var value = "rgba(255,0,255,0.5)";

			console.log(propname + " - " + propvalue);
			//obj.val(propvalue);


			/*
			obj.addClass("os_picker_color");
      
			obj.click(function () { alert('click'); $(this).colorpicker('open'); });
			*/

			//obj.attr("width", "0px");
			//obj.attr('type', "hidden");
			obj.css('width', "0px");
			obj.css('opacity', "0");

			obj.colorpicker({
				onClose: function (hex, rgba, inst) {
					//console.log('onClose: ' + hex + ', (' + rgba.r + ',' + rgba.g + ',' + rgba.b + ',' + rgba.a + ')');
				},
				onSelect: function (hex, rgba, inst) {
					var r = Math.floor(rgba.r * 255);
					var g = Math.floor(rgba.g * 255);
					var b = Math.floor(rgba.b * 255);
					var a = Math.round(rgba.a * 100) / 100;
					if (propname == "os-skin-page-bg")
						console.log("new alpha:" + a);
					if (a == 0)
						a = 0.001; // Due to jquery color picker bug: 'setColor' consider opaque colors with alpha=0...
					Osiris.SkinEditor.change(propname, "rgba(" + r + "," + g + "," + b + "," + a + ")", "color");
				},
				alpha: true,
				buttonImage: '/htdocs/images/controls/color.png',
				buttonImageOnly: true,
				showOn: 'both',
				showSwatches: true,
				showNoneButton: false,
				buttonColorize: true,
				showButtonPanel: false,
				colorFormat: '#rxgxbxax',
				//colorFormat: 'RGBA',
				limit: '',
				parts: 'full',
				zIndex: 9000
			});

			console.log("f1:" + Osiris.SkinEditor.propList["os-skin-page-bg"].value);
			console.log("v1:" + propvalue);
			obj.colorpicker('setColor', propvalue);
			console.log("f2:" + Osiris.SkinEditor.propList["os-skin-page-bg"].value);

		}
		else if (proptype == "type") {
			//alert("ic, name:" +propname+ ",val:" + this.value);
			obj.change(function () { Osiris.SkinEditor.change(propname, this.value, "type"); });
			obj.val(propvalue);
		}
		else {
			//alert("ic, name:" +propname+ ",val:" + this.value);
			obj.keyup(function () { Osiris.SkinEditor.change(propname, this.value, "size"); });
			obj.val(propvalue);
		}
	},

	itemEnabled: function (propname, val) {
		var objControl = Osiris.SkinEditor.propListControl[propname];
		var objContainer = objControl.parent();

		if (val) {
			objContainer.find(".os_skinEditor_itemEnable").attr('checked', 'checked');
			objContainer.removeClass("os_skinEditor_disabled");
			objContainer.addClass("os_skinEditor_enabled");
			Osiris.SkinEditor.propList[propname].enabled = true;
			Osiris.SkinEditor.propListControl[propname].prop("disabled", false);
		}
		else {
			objContainer.find(".os_skinEditor_itemEnable").removeAttr('checked');
			objContainer.removeClass("os_skinEditor_enabled");
			objContainer.addClass("os_skinEditor_disabled");
			Osiris.SkinEditor.propList[propname].enabled = false;
			Osiris.SkinEditor.propListControl[propname].prop("disabled", true);
		}
	},


	parseLess: function (data) {
		// Used for read current property values from less.

		//console.log("parseLess");

		// We remove the SkinEditor values, to catch only page values.
		//data = Osiris.removeStringBetween(data,"/* OsirisSkinEditor_Start */","/* OsirisSkinEditor_End */"); 

		//alert(data);

		var r = data.match(/@os-[^\:]*\:[^\;]*;/g);

		if (r != null) {
			//alert(r.length);
			for (i = 0; i < r.length; i++) {
				//alert(i + ":" + r[i] + "\n-----------\n" + data);

				var p = r[i].split(":");
				var propname = p[0].substr(1); // no first @
				var propvalue = p[1].substr(0, p[1].length - 1); // no last ;

				//alert("name:\n"+propname+"\n,value:\n"+propvalue+"\n-------------\n" + data);

				if (typeof Osiris.SkinEditor.propPage[propname] !== "undefined") {
					Osiris.SkinEditor.propPage[propname].value = propvalue.trim();

					//alert("name:\n"+propname+"\n,value:\n"+propvalue);
				}
			}
		}
		/*        
		if(this.lessParsedOneTime == false)
		{
		console.log("xx1");
		this.lessParsedOneTime = true;
		this.init2();
		}
		*/
	},

	/*
	loadCurrent: function()
	{
	// ToFix: For each property, we create an hidden DOM elements, and apply style from "skinelements.less", 
	// to catch the current CSS/Less value. 
	// Maybe avoided if i understand how ask to Less Library the variables values.      
            
	for (var i in Osiris.SkinEditor.propType) {        
	var propname = i;
	var proptype = this.propType[propname];
        
	var cssname = propname.replace(/-/g,'_');
	var $p = $("<div class=\"" + cssname + "\"></div>").hide().appendTo("body");
	//propElements[propname] = $p;
        
	if(proptype == "color")
	{
	this.propPage[propname] = $p.css("color");          
	}
	else if(proptype == "type")
	{
	this.propPage[propname] = $p.css("padding-left");          
	}
        	
	else if(proptype == "size")
	{        	
	this.propPage[propname] = $p.css("padding-left");          
	}        
        
	$p.remove();
	}                 
	},
	*/

	applyToForm: function () {
		//this.disableApply = true;

		for (var i in Osiris.SkinEditor.propList) {
			var propname = i;
			var propvalue = Osiris.SkinEditor.propList[propname].value;
			var proptype = Osiris.SkinEditor.propList[propname].type;
			var propenabled = Osiris.SkinEditor.propList[propname].enabled;
			var obj = Osiris.SkinEditor.propListControl[propname];

			console.log(propname);
			console.log(Osiris.SkinEditor.propList[propname]);

			if (proptype == "color") {
				obj.colorpicker('setColor', propvalue);
			}
			else if (proptype == "type") {
				obj.val(propvalue);
			}
			else if (proptype == "size") {
				obj.val(propvalue);
			}
			obj.get(0).applyProp();

			Osiris.SkinEditor.itemEnabled(propname, propenabled);
		}

		//this.disableApply = false;

		//this.applyLess();
	},

	load: function (name) {
		if (name == null) {
			var divObj = osCreate("div");
			divObj.title = "Skin Editor - Load Preset"; ;

			for (var i in Osiris.SkinEditor.presets) {
				linkElement = osCreate("a");
				linkElement.innerHTML = i;
				linkElement.className = "os_item_box os_center";
				linkElement.href = "javascript:Osiris.SkinEditor.load('" + i + "');";

				divObj.appendChild(linkElement);
			}


			jQuery(function () {
				jQuery(divObj).dialog({
					show: 'fade',
					hide: 'fade',
					bgiframe: true,
					resizable: false,
					width: '600px',
					modal: true,
					overlay:
						{
							backgroundColor: '#000',
							opacity: 0.5
						},
					open: function () {
					}
				});
			});
		}
		else {
			$("#skinEditor_preset").val(name);
			Osiris.SkinEditor.mergeProp(Osiris.SkinEditor.presets[name]);
			Osiris.SkinEditor.applyToForm();
			Osiris.storageSet("skinEditor.current", Osiris.SkinEditor.propList);
			Osiris.storageSet("skinEditor.current_preset", name);
		}
	},

	resetToPage: function () {
		Osiris.SkinEditor.mergeProp(Osiris.SkinEditor.propPage);
		Osiris.SkinEditor.applyToForm();
		Osiris.notify("info", "Reset to page.");
	},

	save: function () {
		var name = $("#skinEditor_preset").val().trim();
		Osiris.storageSet("skinEditor.current", Osiris.SkinEditor.propList);
		Osiris.storageSet("skinEditor.current_preset", name);
		if ((name == "") || (name.match(/[^-_\sa-zA-Z0-9]/) == false)) {
			Osiris.notify("error", "Please enter a valid name.");
		}
		else {
			Osiris.SkinEditor.presets[name] = Osiris.cloneVar(Osiris.SkinEditor.propList);

			console.log(Osiris.SkinEditor.presets[name]);

			Osiris.storageSet("skinEditor.presets", Osiris.SkinEditor.presets);

			Osiris.notify("info", "Profile <i>" + name + "</i> saved.");
		}
	},

	remove: function () {
		var name = $("#skinEditor_preset").val().trim();
		if ((name == "") || (name.match(/[^-_\sa-zA-Z0-9]/) == false)) {
			Osiris.notify("error", "Please enter a valid name.");
		}
		else {
			if (typeof Osiris.SkinEditor.presets[name] === "undefined") {
				Osiris.notify("error", "Unable to found.");
			}
			else {
				delete Osiris.SkinEditor.presets[name];
				Osiris.storageSet("skinEditor.presets", Osiris.SkinEditor.presets);
				$("#skinEditor_preset").val("");
				Osiris.notify("info", "Profile <i>" + name + "</i> Removed.");
			}
		}
	},

	showLessConfig: function () {
		var current = Osiris.SkinEditor.getLessConfig(false);
		var result = osCopyClipboard("Skin Editor", current, "Close");
		//alert('Skin Editor', Osiris.SkinEditor.getLessConfig());
		if (current != result) {
			//Osiris.notify("info", "Applying..."); 
			Osiris.SkinEditor.parseLess(result);
		}
	},

	import: function () {
		jQuery("#file_import").click();
	},

	export: function () {
		var current = Osiris.SkinEditor.getLessConfig(false);

		//window.location.href="data:application/octet-stream;base64,"+Base64.encode(current);
		window.location.href = "data:application/octet-stream;base64," + "YWxmYStiZXRhDQpnYW1tYQ0KZGVsdGE=";

	},

	handleFileImport: function (evt) {
		evt.stopPropagation();
		evt.preventDefault();

		var files = evt.target.files || evt.dataTransfer.files; // FileList object.
		var file = files[0];

		var profileName = files[0].name.replace(".less", "");

		// this creates the FileReader and reads stuff as text
		var fr = new FileReader();
		fr.onload = parse;
		fr.readAsText(file);

		// Reset to page
		Osiris.SkinEditor.mergeProp(Osiris.SkinEditor.propPage);


		// this is the function that actually parses the file
		// and populates the table
		function parse() {
			lines = fr.result.split(/\n/);
			for (var i in lines) {
				var l = lines[i];
				alert(l);
				var matches = l.match(/@(.*?):(.*?);\s/);
				if (matches) {
					Osiris.SkinEditor.propList[matches[1]].value = matches[2];
					Osiris.SkinEditor.propList[matches[1]].enabled = true;
				}
			}

			$("#skinEditor_preset").val(profileName);
			Osiris.SkinEditor.save();
			Osiris.SkinEditor.load(profileName);
		}
	},

	change: function (propname, propvalue, proptype) {
		//var obj = $("#skinEditor_" + propname);
		var obj = Osiris.SkinEditor.propListControl[propname];
		var oldvalue = Osiris.SkinEditor.propList[propname].value;
		var newvalue = propvalue;


		var valid = false;
		if (proptype == "color")
		//valid = /^#[0-9A-F]{6}$/i.test(newvalue);
			valid = true;
		else if (proptype == "type")
			valid = true;
		else if (proptype == "size")
			valid = /^[0-9]+(%|px)$/i.test(newvalue);

		if ((valid) && (oldvalue != newvalue)) {
			//alert('newvalue:' + newvalue);
			Osiris.SkinEditor.propList[propname].value = newvalue;

			Osiris.SkinEditor.change2(propname);

		}
	},

	change2: function (propname) {
		var obj = Osiris.SkinEditor.propListControl[propname];
		if (Osiris.SkinEditor.disableApply == false)
			obj.get(0).applyProp();

		Osiris.storageSet("skinEditor.current", Osiris.SkinEditor.propList);
	},

	getFinalPropValue: function (propname) {
		if (Osiris.SkinEditor.propList[propname].enabled)
			return Osiris.SkinEditor.propList[propname].value;
		else
			return Osiris.SkinEditor.propPage[propname].value;
	},

	getLessConfig: function (full) {
		var config = "";
		for (var i in Osiris.SkinEditor.propList) {
			var propname = i;
			var propvalue = Osiris.SkinEditor.propList[i].value;
			var skip = false;

			if (Osiris.SkinEditor.propList[i].enabled == false) {
				if (full)
					propvalue = Osiris.SkinEditor.propPage[i].value;
				else
					skip = true;
			}

			if (skip == false) {
				config += "@" + propname + ": " + propvalue + ";\r\n";
			}
		}

		return config;
	},

	applyLess: function (forceRefresh) {
		//alert('less apply');
		Osiris.SkinEditor.lessConfig = Osiris.SkinEditor.getLessConfig(true);

		var globalLess = $("#lesslib").html();
		var currentCustom = globalLess.indexOf("\n/* OsirisSkinEditor_Start */");
		if (currentCustom != -1)
			globalLess = globalLess.substr(0, currentCustom);
		globalLess += "\/* OsirisSkinEditor */\n" + Osiris.SkinEditor.getLessConfig(true) + "\n/* OsirisSkinEditor_End */";
		//alert(globalLess);
		$("#lesslib").html(globalLess);

		var fileExportData = "// Generated by Osiris Skin Editor extension, " + (new Date).toString("yyyyMMddhhmm") + "\r\n" + Osiris.SkinEditor.getLessConfig(false);
		$("#file_export").attr("download", $("#skinEditor_preset").val() + ".less");
		$("#file_export").attr("href", "data:application/octet-stream;base64," + btoa(fileExportData));

		//alert(globalLess);

		destroyLessCache("/");
		forceRefresh = true;
		less.refresh(forceRefresh);
	},

	applyLessTimer: function () {
		var d = new Date();
		var t = d.getTime();

		if ((Osiris.SkinEditor.lastChange != 0) &&
    			(t - Osiris.SkinEditor.lastChange >= Osiris.SkinEditor.applyDelay)) {
			Osiris.SkinEditor.lastChange = 0;
			Osiris.SkinEditor.applyLess(false);
		}

	}
}

$(function() {
 // Handler for .ready() called.
 Osiris.loadUrl('/main/skineditor'); 
});