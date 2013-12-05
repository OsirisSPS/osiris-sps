/*
Osiris JavaScript library.

Note: Global functions must be moved in "Osiris" object.
Note: Most functions need to be replaced/adapted to use more stable library, for example jQuery.
*/

var Osiris =
{
    /**************************************************************************************************
    Variables / Constant
    **************************************************************************************************/

    ensureIdCounter: 0,
    userAgent: navigator.userAgent.toLowerCase(),
    is_ie: (navigator.userAgent.toLowerCase().indexOf("msie") != -1),
    is_moz: (navigator.userAgent.toLowerCase().indexOf("gecko") != -1),
    is_opera: (navigator.userAgent.toLowerCase().indexOf("opera") != -1),
    effectsSpeedSlow: 500,
    effectsSpeedFast: 200,
    
    notificationElement: null,

    /**************************************************************************************************
    Element/Tag Managment
    **************************************************************************************************/

    ensureId: function (src) {
        // Ensure that object have an ID. Otherwise, generate.
        if (!src.id) {
            Osiris.ensureIdCounter++;
            src.id = "gen" + Osiris.ensureIdCounter;
        }
        return src.id;
    },

    // Return an array. First element, html without script. Other elements, script code.
    extractJavascript: function (html) {
        arr = new Array();
        arr.push(""); // html

        for (; ; ) {
            var p1 = 0, p2 = 0, p3 = 0, p4 = 0;
            p1 = html.indexOf("<" + "script", 0);
            if (p1 == -1) {
                arr[0] = html;
                return arr;
            }

            p2 = html.indexOf(">", p1 + 7) + 1;
            p3 = html.indexOf("<" + "/script>", p2);
            p4 = p3 + 9;

            var c = html.substring(p2, p3);
            html = html.substring(0, p1) + html.substr(p4);
            arr.push(c);
        }
    },
    
    // Clone Javascript object.
    /*
    cloneVar: function (a) {
    	if(typeof a ))
    	return jQuery.extend(true, {}, a);
    },
    */
    
    cloneVar: function (obj) {    		
		    // Handle the 3 simple types, and null or undefined
		    if (null == obj || "object" != typeof obj) return obj;
		
		    // Handle Date
		    if (obj instanceof Date) {
		        var copy = new Date();
		        copy.setTime(obj.getTime());
		        return copy;
		    }
		
		    // Handle Array
		    if (obj instanceof Array) {
		        var copy = [];
		        var len = obj.length;
		        for (var i = 0; i < len; ++i) {
		            copy[i] = Osiris.cloneVar(obj[i]);
		        }
		        return copy;
		    }
		
		    // Handle Object
		    if (obj instanceof Object) {
		        var copy = {};
		        for (var attr in obj) {
		            if (obj.hasOwnProperty(attr)) copy[attr] = Osiris.cloneVar(obj[attr]);
		        }
		        return copy;
		    }
		
		    throw new Error("Unable to copy obj! Its type isn't supported.");
		},
		
		// From string 't', remove anything between 'from' and 'to'
		removeStringBetween: function(t, from, to)
		{
			var posStart = t.indexOf(from);
			if(posStart === -1)
				return t;
			var posEnd = t.indexOf(to, posStart + from.length);
			if(posEnd === -1)
				return t;
			return t.substr(0, posStart) + t.substr(posEnd + to.length);
		},


    /**************************************************************************************************
    Client Storage - HTML5
    **************************************************************************************************/

    storageAvailable: function () {
      try {
          return 'localStorage' in window && window['localStorage'] !== null;
      }
      catch (e) {
          return false;
      }
    },
    
    storageHas: function (key) {
    	if ((key == null) || (key == ""))
    		return false;
    	if (Osiris.storageAvailable() == false)
    		return false;
    	var k = "osiris." + key;
      var val = localStorage.getItem(k);
      return (val != null);
    },

    storageGet: function (key, def) {
      if ((key == null) || (key == ""))
          return this.cloneVar(def);
      if (Osiris.storageAvailable() == false)
          return this.cloneVar(def);

			var k = "osiris." + key;
      var val = localStorage.getItem(k);
      if (val === null)
      {
          return this.cloneVar(def);
      }
      else
    	{
    		return JSON.parse(val);
      }
    },

    storageSet: function (key, val) {
      if ((key == null) || (key == ""))
          return;
      if (Osiris.storageAvailable() == false)
          return;

			var k = "osiris." + key;
      localStorage.setItem(k, JSON.stringify(val));
    },
    
    storageRemove: function (key) {
    	if ((key == null) || (key == ""))
          return;
      if (Osiris.storageAvailable() == false)
          return;

			var k = "osiris." + key;
			localStorage.removeItem(k);
    },

    /**************************************************************************************************
    International
    **************************************************************************************************/

    getText: function (key, name1, value1, name2, value2) {
        var out = Osiris.Lang[key];
        if(name1)
            out = out.replace("{@" + name1 + "}", value1);
        if(name2)
            out = out.replace("{@" + name2 + "}", value2);
        return out;
    },

    /**************************************************************************************************
    General ID
    **************************************************************************************************/
    
    getById: function(id) {
			if (id==null) return null;
			if (id=='') return null;
			return document.getElementById(id);
		},
    
    resolveElement: function(src) {
    	if(typeof(src) == "string")
    		return $(src).get(0);
    	else
    		return src;
    },

    scrollTo: function (id) {
        $('html, body').animate(
	    {
	        scrollTop: $('#' + id).offset().top,
	        scrollLeft: $('#' + id).offset().left
	    }, 1000);
    },

    imageResize: function (src, maxWidth, maxHeight) {
        src.width = Math.min(maxWidth, src.width);
        src.height = Math.min(maxHeight, src.height);
    },


    /**************************************************************************************************
    IDE Initialization
    **************************************************************************************************/

    init: function () {
    	
        // Useful to apply CSS styles only if javascript is enabled.
        $('body').addClass("os_javascript_enabled");

        osInitControls(null);
    },

    adjustUrl: function (url) {
        fullUrl = url;

        // {FullUrl Isis Adjustment}
        // Leave the comment above, are replaced by Isis to adjust the base path.

        return fullUrl;
    },

    // This is detected by Isis, to adjust url.
    // We expect that who call this, pass as parameter a direct string, not a variable.
    adjustStaticUrl: function (url) {
        return url;
    },

    /**************************************************************************************************
    Controls utils
    **************************************************************************************************/
    
    // Highlight a "selectedID" child of groupID, de-hightlight others, set an hidden field with "selectedID".
    pickerSelect: function (groupID, selectedID, hiddenFieldID) {
        $('#' + hiddenFieldID).val(selectedID);        
        $('#' + groupID + " a").addClass("os_picker_select_item_normal");
        $('#' + groupID + " a").removeClass("os_picker_select_item_selected");
        $('#' + selectedID).addClass("os_picker_select_item_selected");
    },

    // Look style as button up, emboss effect.
    styleLookDown: function (src) {
    },

    styleLookUp: function (src) {
    },
    
    /**************************************************************************************************
    Ajax
    **************************************************************************************************/
    
    loadUrl: function (url, successEvent) {
    	ajaxDiv = document.createElement('div');
	    ajaxDiv.style.display = "none";
	    ajaxDiv.onAjaxSuccess = successEvent;
	    document.body.appendChild(ajaxDiv);    
	    Osiris.contentFromUrl(ajaxDiv, url, "none", "notify");
    },
    
    contentFromUrl: function (src, url, effect, wait) {

			src = Osiris.resolveElement(src);    	
    	var fullUrl = Osiris.adjustUrl(url);
	    if(effect == null)
	        effect = "none";
	        
	    src.waitStyle = wait;
	    
	    //alert('warning, ' + url);    
	    //Osiris.notify("warning", url);    
	    
	    if(wait == "progress")
	    {
	        divWait = document.createElement('div');
	        divWait.className = "os_ajax_wait";    
	    
	        jQuery(divWait).css( { "left": jQuery(src).offset().left + "px", "top": jQuery(src).offset().top + "px" } );
	        jQuery(divWait).css( { "width": jQuery(src).width() + "px", "height": jQuery(src).height() + "px" } );
	     
	        src.appendChild(divWait);
	    }
	    else if(wait == "notify")
    	{
    		// TOCLEAN alert('wait, ' + url);
    		Osiris.notify("wait");
    	}
	    
	    jQuery.ajax(
			{
	     url: fullUrl,
	     success: function(response) 
	     {
	     		// TOCLEAN alert('clean, ' + url);
	     		// TOCLEAN alert(src.waitStyle);
	     		if(src.waitStyle != "none")
	     			Osiris.notify("clean");
	     		
	        if(src.onAjaxSuccess)
	            src.onAjaxSuccess();
	            
	        //alert("inner result :" + response);
	        if(effect == "slide")    
	        {
	            $(src).slideUp(Osiris.effectsSpeedFast);
	        }
	            
	        if(effect == "fade")    
	        {        
	            $(src).fadeOut(Osiris.effectsSpeedSlow);
	        }
	                
	        var html = response;
	        
	        arr = Osiris.extractJavascript(html);
	        
	        for(i=0;i<arr.length;i++)
	        {
	            //alert("ele " + i + ":" + arr[i]);
	            
	            if(i == 0)
	            {                
	                src.innerHTML = arr[0];
	                
	                for(var i2=0;i2<src.childNodes.length;i2++)
	                {	
	                    osInitControls(src.childNodes[i2]);
	                }
	            }
	            else
	            {
	                var s = document.createElement("script");
	                s.type = "text/javascript";
	                s.text = arr[i];
	                src.appendChild(s);
	            }
	        }                
	        
	        if(src.onReady)
	            src.onReady();
	        
	        if(effect == "slide")    
	        {
	            $(src).slideDown(Osiris.effectsSpeedFast);
	        }
	            
	        if(effect == "fade")    
	        {
	            $(src).fadeIn(Osiris.effectsSpeedSlow);
	        }
	     },
	     error: function(xhr) 
	     {
	        var msg = "Ajax error, status = " + xhr.status + " - url = " + url;
	        
	        // TOCLEAN alert('error, ' + url);
	        Osiris.notify("warning",msg);
	     }
	    });  
    },
    
    /**************************************************************************************************
    Dialogs
    **************************************************************************************************/
    
    prompt: function(msg, val) {
    	return prompt(msg, val);
    },
    
    alert: function(promptTitle, promptMessage) {
	    // promptMessage può essere un ID.
	    
	    var divObj = osCreate("div");
	    divObj.title = promptTitle;    
	    var divP = osCreate("p");
	    var divIcon = osCreate("span");    
	    divP.appendChild(divIcon);
	    divText = osCreate("div");
	    //divText.className = "os_content";
	    
	    if(osGetById(promptMessage))
	        divText.appendChild(osGetById(promptMessage))
	    else
	        divText.innerHTML = promptMessage;
	        
	    divP.appendChild(divText);
	    divObj.appendChild(divText);
	    osGetBody().appendChild(divObj);
	    
	    jQuery(function() {
				jQuery(divObj).dialog(
				{
					autoOpen: false,
					show: 'fade',
					hide: 'fade',
					bgiframe: true,
					resizable: false,
					width:'50%',
					minHeight:140,
					modal: true,
					close: function(event, ui) { jQuery(this).remove(); },
					dialogClass: 'alert',				
				});
			});	
	
			jQuery(divObj).dialog("open");
		},
				
		error: function(promptTitle, promptMessage) {
			Osiris.alert(promptTitle, promptMessage);
		},
		
		notify: function(style, message) {			
			
			//alert("style:" + style + ", message:" + message);
			/* Styles:
			   - wait : Wait icon. Don't removed automatically.
			   - clean : Remove current notification
			   - info, warning, error : Message showed for 3 seconds.
			*/
			
			
			if(Osiris.notificationElement == null)
			{
				Osiris.notificationElement = jQuery(osCreate("div"));
				jQuery("body").append(Osiris.notificationElement);
				//osGetBody().appendChild(Osiris.notificationElement.get());
			}					
			
			if( (style == null) || (style == "clean") )// Hide directly.
			{
				//Osiris.notificationElement.fadeOut(Osiris.effectsSpeedSlow);
				clearTimeout(Osiris.notificationElement.timer);
				Osiris.notificationElement.hide();
				return;
			}			
			
			if(style == "wait")
				message = "Loading ...";			
			
			Osiris.notificationElement.html(message);
			Osiris.notificationElement.attr("class", "os_notification box-" + style);
			
			Osiris.notificationElement.css("top", (($(window).height() - Osiris.notificationElement.outerHeight()) / 2) + 
	                                                $(window).scrollTop() + "px");
	    Osiris.notificationElement.css("left", (($(window).width() - Osiris.notificationElement.outerWidth()) / 2) + 
	                                                $(window).scrollLeft() + "px");
			
			Osiris.notificationElement.fadeIn(Osiris.effectsSpeedSlow);							
			
			if(style != "wait")
			{
				Osiris.notificationElement.timer = setTimeout(function() {
					Osiris.notificationElement.fadeOut(Osiris.effectsSpeedSlow);
				}, 3000);							
			}
			else
			{
				clearTimeout(Osiris.notificationElement.timer);
			}
		},
    
		confirm: function(promptTitle, promptMessage, promptButton, promptCancel, promptHref) {
	    // promptMessage può essere un ID.
	    
	    var divObj = osCreate("div");
	    divObj.title = promptTitle;    
	    var divP = osCreate("p");
	    var divIcon = osCreate("span");    
	    divP.appendChild(divIcon);
	    divText = osCreate("div");
	    //divText.className = "os_content";
	    
	    if(osGetById(promptMessage))
	        divText.appendChild(osGetById(promptMessage))
	    else
	        divText.innerHTML = promptMessage;
	        
	    divP.appendChild(divText);
	    divObj.appendChild(divText);
	    osGetBody().appendChild(divObj);
	    
	    divObj.promptHref = promptHref;
	    
	    jQuery(function() {
			jQuery(divObj).dialog(
			{
			    autoOpen: false,
			    show: 'fade',
	            hide: 'fade',
				bgiframe: true,
				resizable: false,
				width:'50%',
				minHeight:140,
				modal: true,
				close: function(event, ui) { jQuery(this).remove(); },
				dialogClass: 'alert',
				buttons: 
				{	
				    confirm_button: function() 
					{	
					    jQuery(this).dialog('close');
						location.href = this.promptHref;
					},			
					cancel_button: function() 
					{
						jQuery(this).dialog('close');
					}				
				},
				open: function()
				{                
	                jQuery(":button:contains('confirm_button')").html(promptButton);
	                jQuery(":button:contains('cancel_button')").html(promptCancel);
	            }
			});
			});	
	
			jQuery(divObj).dialog("open");
		},

		dialog: function(src, open) {			
			src = Osiris.resolveElement(src);
			jQuery(src).dialog(
			{
				autoOpen: false,
				show: 'fade',
				/*hide: 'fade',*/
				hide: { effect: "fade", duration: Osiris.effectsSpeedSlow },
				bgiframe: true,
				resizable: false,
				minHeight:140,
				modal: true,		
				dialogClass: 'notitle',				
				open: function(){
            $('.ui-widget-overlay').hide().fadeIn(Osiris.effectsSpeedSlow);
        },
			});
			
			width = osGetAttributeValue(src,"data-os-dialog-width",null);
	    if(width == "window")
	        width = jQuery(window).width()-100;    
	    if(width != null)
	        jQuery(src).dialog( "option", "width", width );
	    else
	    		jQuery(src).dialog( "option", "width", 'auto' );
	        
	    height = osGetAttributeValue(src,"data-os-dialog-height",null);
	    if(height == "window")
	        height = jQuery(window).height()-100;
	    if(height != null)
	        jQuery(src).dialog( "option", "height", height );
	    else
	    		jQuery(src).dialog( "option", "height", 'auto' );
	        
	    jQuery(src).dialog( "option", "maxWidth", jQuery(window).width()-100 );
	    jQuery(src).dialog( "option", "maxHeight", jQuery(window).height()-100 );
    									
			buttonClose = osCreate("div");
			buttonClose.dialog = $(src);
			$(buttonClose).attr("class", "ui-dialog-toprightclose");
			$(buttonClose).click(function() {
				$($(this).get(0).dialog).dialog('close');  						
			});
			$(src).parent().prepend($(buttonClose));
			
			if(open == true)
				$(src).dialog("open");
		},
		
		copyClipboard: function(promptTitle, promptMessage, promptButton, promptCancel, promptHref) {
		  var divObj = osCreate("div");
	    divObj.title = promptTitle;    
	    
	    var divP = osCreate("p");
	    var divIcon = osCreate("div");
	    divIcon.className = "os_clipboard_icon";
	    divIcon.style.float = 'left';
	    divObj.appendChild(divIcon);
	    
	    divTitle = osCreate("h1");
	    $(divTitle).text(promptTitle);
	    //$(divObj).prepend(divTitle);
	    divObj.appendChild(divTitle);
	    
	    divText = osCreate("textarea");    
	    divText.style.width = "500px";    
	    divText.style.height = "80px";    
	    divText.style.maxHeight = "300px";
	    divText.value = promptMessage;    
	    divObj.appendChild(divText);
	    
	    $(divText).autosize();		
	    
	    
	        
	    Osiris.dialog(divObj, true);
	    
	    jQuery(":button:contains('close_button')").html(promptButton);                
	                divText.select();
	                divText.focus();
	    /*            
	    $(divObj).bind('dialogclose', function(event) {
			     
			 });
*/
    },
	
	

    /**************************************************************************************************
    Controls initializations
    **************************************************************************************************/

    controlBoolean: function (src) {
    		/*
    	  var boolSwitch = osCreate("span")
        boolSwitch.referenceInput = src;
        $(boolSwitch).addClass("os_controls_bool");
        if ($(src).val() == 1) {
            $(boolSwitch).addClass("os_controls_bool_checked");
        }
        $(boolSwitch).click(function (e) {
            if ($(this.referenceInput).val() == 1) {
                $(this.referenceInput).val(0);
                $(this).removeClass("os_controls_bool_checked");

            } else {
                $(this.referenceInput).val(1);
                $(this).addClass("os_controls_bool_checked");
            }
        });
        */
        
       	var boolContainer = osCreate("span");
       	
      	var boolSwitch = osCreate("span");
      	$(boolContainer).append(boolSwitch);
        boolSwitch.referenceInput = src;
        $(boolContainer).addClass("os_controls_bool_container");
        $(boolSwitch).addClass("os_controls_bool_switch");
        if ($(src).val() == 1) {
            $(boolSwitch).addClass("os_controls_bool_switch_checked");
        }
        $(boolSwitch).click(function (e) {
            if ($(this.referenceInput).val() == 1) {
                $(this.referenceInput).val(0);
                $(this).removeClass("os_controls_bool_switch_checked");                

            } else {
                $(this.referenceInput).val(1);
                $(this).addClass("os_controls_bool_switch_checked");                
            }
        });
      
      	var boolContainerYes = osCreate("span");
       	$(boolContainerYes).addClass("os_controls_bool_container_yes");
       	$(boolContainer).append(boolContainerYes);
       	
       	var boolContainerNo = osCreate("span");
       	$(boolContainerNo).addClass("os_controls_bool_container_no");
       	$(boolContainer).append(boolContainerNo);
      

        //$(src).hide();
        $(boolContainer).insertAfter($(src));
        
        
    },

    controlNumber: function (src) {
        // Note 02/03/2012: Wait jQuery 1.9
        //$(src).spinner({max: 3, min:0});
    },
    
    controlSlider: function (src) {
        src.slider = osCreate("div");
        $(src.slider).slider({
        	min: 5,
        	max: 10,
        	value: 2,
        	slide: function( event, ui) {
        	}
        });
    },

    controlString: function (src, required) {
        $(src).blur(Osiris.controlStringValidation);
        $(src).keyup(Osiris.controlStringValidation);
        src.spanMessage = osCreate("span");
        //alert(src.validationMessage2);
        //$(src.spanMessage).text("xxx");
        $(src).after(src.spanMessage);
        $(src.spanMessage).addClass("os_validation_message");
        $(src).trigger("blur");
    },

    controlStringValidation: function () {
    		if ($(this).attr("data-os-minchars") != "") {
            var submit = $(this).attr("data-os-submit");
            var minChars = $(this).attr("data-os-minchars");
            if ($(this).val().length < minChars) {
                $(this).addClass("os_input_invalid");
                //var msg =
                $(this.spanMessage).text(Osiris.getText("controls.string.minchars","nchars",minChars));
                
                if(submit != "")
                	$('#' + submit).prop('disabled', true);

                /*
                if ($(this.spanMessage).text() == "") {
                if (submit.errors)
                submit.errors++;
                else
                submit.errors = 1;
                alert(submit.errors);
                    
                submit.disabled = (submit.errors != 0);
                }
                */

            }
            else 
            	{
                $(this).removeClass("os_input_invalid");
                $(this.spanMessage).text("");
                
                if(submit != "")
                	$('#' + submit).prop('disabled', false);
                	
                /*
                if ($(this.spanMessage).text() != "") {
                submit.errors--;
                alert(submit.errors);
                submit.disabled = (submit.errors != 0);
                }
                */

            }
        }
    },
    
    controlOmlEditor: function (src) {
    	Osiris.OmlEditor.init(src);
    },
    
    /**************************************************************************************************
    Other
    **************************************************************************************************/
    
    initAdvertising: function() {    
    	var objParent = osGetById("osiris_advertising");
			if( (objParent != null) && (objParent.attributes) )
	    	Osiris.contentFromUrl(objParent, "/AF4E66017B2F4E240E47EA9240F6F7E8C9EF7D78/adv", "fade", "none");	
	  }

}

/**************************************************************************************************
Old, need to be adjusted.
**************************************************************************************************/

/*
function osIncludeScript(filename) 
{
    var html_doc = document.getElementsByTagName('head').item(0);
    var js = document.createElement('script');
    js.setAttribute('language', 'javascript');
    js.setAttribute('type', 'text/javascript');
    js.setAttribute('src', filename);
    html_doc.appendChild(js);
    return false;
}
*/

/**************************************************************************************************
 Event helper
**************************************************************************************************/

/*
function osGetEventTarget(e)
{
	if (is_ie) return event.srcElement;
	if ( (is_moz) && (e) ) return e.target;
	return null;
}
*/

function osGetEvent(e)
{
    if (Osiris.is_ie) return event;
    if ( (Osiris.is_moz) && (e) ) return e;
    if ( (Osiris.is_opera) && (e) ) return e;
    return null;
}

/**************************************************************************************************
 Element/Tag Managment
**************************************************************************************************/

function osGetBody()
{
    return document.body;
}

function osGetById(id)
{
	return Osiris.getById(id);	
}

function osGetChild(src, childName)
{
	// DAFARE: ottimizzare, non ho trovato altro modo... childNodes non è una mappa, ma un'array..
	var l = src.childNodes.length;
	for(var i=0; i<l; i++)
	{
		if (src.childNodes[i].id == childName)
			return src.childNodes[i];
	}

	return null;
}

function osGetChildWithAttribute(src, attrName, attrValue)
{
    // Potrebbe richiedere tempo, assicurarsi che l'elemento non sia 'lontano'...
    if(src.attributes)
    {
        var reqAttr = src.attributes[attrName];
	    if( (reqAttr) && (reqAttr.nodeValue == attrValue) )
	    {
	        return src;
        }
    }

    for(var c=0;c<src.childNodes.length;c++)
	{
		var found = osGetChildWithAttribute(src.childNodes[c], attrName, attrValue);
		if(found)
		    return found;
	}

	return null;
}

function osGetAttributeValue(src, attrName, def)
{
    if( (src) && (src.attributes) && (src.attributes[attrName]) )
        return src.attributes[attrName].nodeValue;
    else
        return def;
}

function osCreate(etype)
{
	return document.createElement(etype);
}

function osCreateText(text)
{
	return document.createTextNode(text);
}

function osGetFrameWndParent()
{
	return window.parent;
}

function osGetFrameWnd(id)
{
	if (Osiris.is_ie)
	{
		return document.frames(id);
	}
	else
	{
		return document.getElementById(id).contentDocument.defaultView;
	}
}

/*
function osAppendHtmlAndJavascript(src, html)
{
    arr = Osiris.extractJavascript(html);
    
    for(i=0;i<arr.length;i++)
    {
        if(i == 0)
            src.innerHTML = arr[0];
        else
        {
            var s = document.createElement("script");
            s.type = "text/javascript";
            s.text = arr[i];
            src.appendChild(s);
        }
    }    
}
*/


/**************************************************************************************************
 Form Functions
**************************************************************************************************/

function osGetForm()
{
	return osGetById("Form");
}

function osSubmit()
{
    var formObj = osGetForm();    
    formObj.submit();
}

function osEventPostCtl(eventId, eventArgument)
{
    osGetById("osPostCtlId").value = eventId.split("$").join(":");
	osGetById("osPostCtlArg").value = eventArgument;
	osSubmit();
	return false;
}

function osAutoPostBack(targetSrc)
{
	osGetById("osPostCtlId").value = targetSrc.id;
	osGetById("osPostCtlArg").value = "";
	osSubmit();
	return false;
}

/**************************************************************************************************
 Styles Functions
**************************************************************************************************/

function osShow(src)
{
	src.style.visibility="visible";
}

function osHide(src)
{
	src.style.visibility="hidden";
}

function osClassNameSet(src,class1)
{
	src.className=class1;
}

function osClassName(src,class1,class2)
{
	if (src.className==class1)
	{
		src.className=class2;
	}
	else
	{
		if (src.className==class2)
		{
			src.className=class1;
		}
	}
}

function osClassNameIf(src,classif,class1)
{
	if (src.className==classif) src.className=class1;
}

function osGetAbsoluteMouseX(e)
{
	if (Osiris.is_opera || Osiris.is_moz) return e.pageX;
	if (Osiris.is_ie) return e.clientX + document.documentElement.scrollLeft;
	return 0;
}

function osGetAbsoluteMouseY(e)
{
    if (Osiris.is_opera || Osiris.is_moz) return e.pageY;
    if (Osiris.is_ie) return e.clientY + document.documentElement.scrollTop;
	return 0;
}

function osGetWindowWidth()
{
    if( typeof( window.innerWidth ) == 'number' )
    {
        // Not IE
        return parseInt(window.innerWidth);
    }
    else
    {
        return parseInt(document.documentElement.clientWidth);
    }
}

function osGetWindowHeight()
{
    if( typeof( window.innerHeight ) == 'number' )
    {
        // Not IE
        return parseInt(window.innerHeight);
    }
    else
    {
        return parseInt(document.documentElement.clientHeight);
    }
}

function osMoveOnCursor(e,obj,offsetLeft,offsetTop)
{
	e=osGetEvent(e);

	if(!offsetTop) offsetTop = offsetLeft;
	
	var newx = (osGetAbsoluteMouseX(e)-offsetLeft);
	var newy = (osGetAbsoluteMouseY(e)-offsetTop);	
	
	// 0.12 - Window border adjust
	var winDX = osGetWindowWidth()-30;
	var winDY = osGetWindowHeight()-30;
	//console.log(winDX);
	if(newx + parseInt(obj.offsetWidth) > winDX)
	    newx = winDX - parseInt(obj.offsetWidth);	
	    
	obj.style.position="absolute";
	obj.style.left=newx + "px";
	obj.style.top=newy + "px";	
}

function osContainsDOM (container, containee)
{
	// Try/catch to avoid FF bug: https://bugzilla.mozilla.org/show_bug.cgi?id=208427
try
{
  var isParent = false;
  do {
    if ((isParent = container == containee))
      break;
    containee = containee.parentNode;
  }
  while (containee != null);
  return isParent;
  } 
  catch(e)
  {
	return false;
  }
}

function osCheckMouseEnter (e,element)
{
	e=osGetEvent(e);

	if (element.contains && e.fromElement) 
	{
		return !element.contains(e.fromElement);
	}
	else if (e.relatedTarget) 
	{
		return !osContainsDOM(element, e.relatedTarget);
	}
}

function osCheckMouseLeave (e,element)
{
	e=osGetEvent(e);

	if (element.contains && e.toElement) 
	{
		return !element.contains(e.toElement);
	}
	else if (e.relatedTarget) 
	{
		return !osContainsDOM(element, e.relatedTarget);
	}
}

function osAlphaGet(src)
{
	if (Osiris.is_ie)
	{
		if (src.filters)
		{
			return src.filters.alpha.opacity;
		}
		else
		{
			return 255;
		}
	}
	else if (Osiris.is_opera)
	{
	    if (src.style)
	    {
	        return src.style.opacity;
	    }
	    else
	    {
	        return 255;
	    }
	}
	else if (Osiris.is_moz)
	{
		if (src.style)
		{
			return src.style.MozOpacity*100;
		}
		else
		{
			return 255;
		}
	}
	else
	{
	    return 255;
	}
}

function osAlphaSet(src,v)
{
	if (Osiris.is_ie)
	{
		if (src.filters)
			src.filters.alpha.opacity = v*100;		
	}
	else if (Osiris.is_opera)
	{
	    if (src.style)
	        src.style.opacity = v;	
	}
	else if (Osiris.is_moz)
	{
		if (src.style)		
			src.style.MozOpacity=v;
	}	
}

function osVisibilityInvert(src)
{    
	jQuery(src).slideToggle(Osiris.effectsSpeedFast);
}

/**************************************************************************************************
 Strings Function
**************************************************************************************************/

function osEndsWith(str, suffix) 
{
    return str.indexOf(suffix, str.length - suffix.length) !== -1;
}

function osReplaceText(sString,sSearch,sReplace)
{
	return sString.replace(sSearch,sReplace);
}

function osReplaceAllText(sString,sSearch,sReplace)
{
	do
	{
		oString=sString;
		sString=sString.replace(sSearch,sReplace);
	}
	while (sString!=oString);
	return sString;
}

function osHtmlDecode(s)
{
      var out = "";
      if (s==null) return;

      var l = s.length;
      for (var i=0; i<l; i++)
      {
            var ch = s.charAt(i);
            if (ch == '&')
            {
                  var semicolonIndex = s.indexOf(';', i+1);
			            if (semicolonIndex > 0)
			            {
                        var entity = s.substring(i + 1, semicolonIndex);
                        if (entity.length > 1 && entity.charAt(0) == '#')
                        {
                              if (entity.charAt(1) == 'x' || entity.charAt(1) == 'X')
                                    ch = String.fromCharCode(eval('0'+entity.substring(1)));
                              else
                                    ch = String.fromCharCode(eval(entity.substring(1)));
                        }
                    		else
                      	{
                              switch (entity)
                              {
                                    case 'quot': ch = String.fromCharCode(0x0022); break;
                                    case 'amp': ch = String.fromCharCode(0x0026); break;
                                    case 'lt': ch = String.fromCharCode(0x003c); break;
                                    case 'gt': ch = String.fromCharCode(0x003e); break;
                                    case 'nbsp': ch = String.fromCharCode(0x00a0); break;
                                    case 'iexcl': ch = String.fromCharCode(0x00a1); break;
                                    case 'cent': ch = String.fromCharCode(0x00a2); break;
                                    case 'pound': ch = String.fromCharCode(0x00a3); break;
                                    case 'curren': ch = String.fromCharCode(0x00a4); break;
                                    case 'yen': ch = String.fromCharCode(0x00a5); break;
                                    case 'brvbar': ch = String.fromCharCode(0x00a6); break;
                                    case 'sect': ch = String.fromCharCode(0x00a7); break;
                                    case 'uml': ch = String.fromCharCode(0x00a8); break;
                                    case 'copy': ch = String.fromCharCode(0x00a9); break;
                                    case 'ordf': ch = String.fromCharCode(0x00aa); break;
                                    case 'laquo': ch = String.fromCharCode(0x00ab); break;
                                    case 'not': ch = String.fromCharCode(0x00ac); break;
                                    case 'shy': ch = String.fromCharCode(0x00ad); break;
                                    case 'reg': ch = String.fromCharCode(0x00ae); break;
                                    case 'macr': ch = String.fromCharCode(0x00af); break;
                                    case 'deg': ch = String.fromCharCode(0x00b0); break;
                                    case 'plusmn': ch = String.fromCharCode(0x00b1); break;
                                    case 'sup2': ch = String.fromCharCode(0x00b2); break;
                                    case 'sup3': ch = String.fromCharCode(0x00b3); break;
                                    case 'acute': ch = String.fromCharCode(0x00b4); break;
                                    case 'micro': ch = String.fromCharCode(0x00b5); break;
                                    case 'para': ch = String.fromCharCode(0x00b6); break;
                                    case 'middot': ch = String.fromCharCode(0x00b7); break;
                                    case 'cedil': ch = String.fromCharCode(0x00b8); break;
                                    case 'sup1': ch = String.fromCharCode(0x00b9); break;
                                    case 'ordm': ch = String.fromCharCode(0x00ba); break;
                                    case 'raquo': ch = String.fromCharCode(0x00bb); break;
                                    case 'frac14': ch = String.fromCharCode(0x00bc); break;
                                    case 'frac12': ch = String.fromCharCode(0x00bd); break;
                                    case 'frac34': ch = String.fromCharCode(0x00be); break;
                                    case 'iquest': ch = String.fromCharCode(0x00bf); break;
                                    case 'Agrave': ch = String.fromCharCode(0x00c0); break;
                                    case 'Aacute': ch = String.fromCharCode(0x00c1); break;
                                    case 'Acirc': ch = String.fromCharCode(0x00c2); break;
                                    case 'Atilde': ch = String.fromCharCode(0x00c3); break;
                                    case 'Auml': ch = String.fromCharCode(0x00c4); break;
                                    case 'Aring': ch = String.fromCharCode(0x00c5); break;
                                    case 'AElig': ch = String.fromCharCode(0x00c6); break;
                                    case 'Ccedil': ch = String.fromCharCode(0x00c7); break;
                                    case 'Egrave': ch = String.fromCharCode(0x00c8); break;
                                    case 'Eacute': ch = String.fromCharCode(0x00c9); break;
                                    case 'Ecirc': ch = String.fromCharCode(0x00ca); break;
                                    case 'Euml': ch = String.fromCharCode(0x00cb); break;
                                    case 'Igrave': ch = String.fromCharCode(0x00cc); break;
                                    case 'Iacute': ch = String.fromCharCode(0x00cd); break;
                                    case 'Icirc': ch = String.fromCharCode(0x00ce ); break;
                                    case 'Iuml': ch = String.fromCharCode(0x00cf); break;
                                    case 'ETH': ch = String.fromCharCode(0x00d0); break;
                                    case 'Ntilde': ch = String.fromCharCode(0x00d1); break;
                                    case 'Ograve': ch = String.fromCharCode(0x00d2); break;
                                    case 'Oacute': ch = String.fromCharCode(0x00d3); break;
                                    case 'Ocirc': ch = String.fromCharCode(0x00d4); break;
                                    case 'Otilde': ch = String.fromCharCode(0x00d5); break;
                                    case 'Ouml': ch = String.fromCharCode(0x00d6); break;
                                    case 'times': ch = String.fromCharCode(0x00d7); break;
                                    case 'Oslash': ch = String.fromCharCode(0x00d8); break;
                                    case 'Ugrave': ch = String.fromCharCode(0x00d9); break;
                                    case 'Uacute': ch = String.fromCharCode(0x00da); break;
                                    case 'Ucirc': ch = String.fromCharCode(0x00db); break;
                                    case 'Uuml': ch = String.fromCharCode(0x00dc); break;
                                    case 'Yacute': ch = String.fromCharCode(0x00dd); break;
                                    case 'THORN': ch = String.fromCharCode(0x00de); break;
                                    case 'szlig': ch = String.fromCharCode(0x00df); break;
                                    case 'agrave': ch = String.fromCharCode(0x00e0); break;
                                    case 'aacute': ch = String.fromCharCode(0x00e1); break;
                                    case 'acirc': ch = String.fromCharCode(0x00e2); break;
                                    case 'atilde': ch = String.fromCharCode(0x00e3); break;
                                    case 'auml': ch = String.fromCharCode(0x00e4); break;
                                    case 'aring': ch = String.fromCharCode(0x00e5); break;
                                    case 'aelig': ch = String.fromCharCode(0x00e6); break;
                                    case 'ccedil': ch = String.fromCharCode(0x00e7); break;
                                    case 'egrave': ch = String.fromCharCode(0x00e8); break;
                                    case 'eacute': ch = String.fromCharCode(0x00e9); break;
                                    case 'ecirc': ch = String.fromCharCode(0x00ea); break;
                                    case 'euml': ch = String.fromCharCode(0x00eb); break;
                                    case 'igrave': ch = String.fromCharCode(0x00ec); break;
                                    case 'iacute': ch = String.fromCharCode(0x00ed); break;
                                    case 'icirc': ch = String.fromCharCode(0x00ee); break;
                                    case 'iuml': ch = String.fromCharCode(0x00ef); break;
                                    case 'eth': ch = String.fromCharCode(0x00f0); break;
                                    case 'ntilde': ch = String.fromCharCode(0x00f1); break;
                                    case 'ograve': ch = String.fromCharCode(0x00f2); break;
                                    case 'oacute': ch = String.fromCharCode(0x00f3); break;
                                    case 'ocirc': ch = String.fromCharCode(0x00f4); break;
                                    case 'otilde': ch = String.fromCharCode(0x00f5); break;
                                    case 'ouml': ch = String.fromCharCode(0x00f6); break;
                                    case 'divide': ch = String.fromCharCode(0x00f7); break;
                                    case 'oslash': ch = String.fromCharCode(0x00f8); break;
                                    case 'ugrave': ch = String.fromCharCode(0x00f9); break;
                                    case 'uacute': ch = String.fromCharCode(0x00fa); break;
                                    case 'ucirc': ch = String.fromCharCode(0x00fb); break;
                                    case 'uuml': ch = String.fromCharCode(0x00fc); break;
                                    case 'yacute': ch = String.fromCharCode(0x00fd); break;
                                    case 'thorn': ch = String.fromCharCode(0x00fe); break;
                                    case 'yuml': ch = String.fromCharCode(0x00ff); break;
                                    case 'OElig': ch = String.fromCharCode(0x0152); break;
                                    case 'oelig': ch = String.fromCharCode(0x0153); break;
                                    case 'Scaron': ch = String.fromCharCode(0x0160); break;
                                    case 'scaron': ch = String.fromCharCode(0x0161); break;
                                    case 'Yuml': ch = String.fromCharCode(0x0178); break;
                                    case 'fnof': ch = String.fromCharCode(0x0192); break;
                                    case 'circ': ch = String.fromCharCode(0x02c6); break;
                                    case 'tilde': ch = String.fromCharCode(0x02dc); break;
                                    case 'Alpha': ch = String.fromCharCode(0x0391); break;
                                    case 'Beta': ch = String.fromCharCode(0x0392); break;
                                    case 'Gamma': ch = String.fromCharCode(0x0393); break;
                                    case 'Delta': ch = String.fromCharCode(0x0394); break;
                                    case 'Epsilon': ch = String.fromCharCode(0x0395); break;
                                    case 'Zeta': ch = String.fromCharCode(0x0396); break;
                                    case 'Eta': ch = String.fromCharCode(0x0397); break;
                                    case 'Theta': ch = String.fromCharCode(0x0398); break;
                                    case 'Iota': ch = String.fromCharCode(0x0399); break;
                                    case 'Kappa': ch = String.fromCharCode(0x039a); break;
                                    case 'Lambda': ch = String.fromCharCode(0x039b); break;
                                    case 'Mu': ch = String.fromCharCode(0x039c); break;
                                    case 'Nu': ch = String.fromCharCode(0x039d); break;
                                    case 'Xi': ch = String.fromCharCode(0x039e); break;
                                    case 'Omicron': ch = String.fromCharCode(0x039f); break;
                                    case 'Pi': ch = String.fromCharCode(0x03a0); break;
                                    case ' Rho ': ch = String.fromCharCode(0x03a1); break;
                                    case 'Sigma': ch = String.fromCharCode(0x03a3); break;
                                    case 'Tau': ch = String.fromCharCode(0x03a4); break;
                                    case 'Upsilon': ch = String.fromCharCode(0x03a5); break;
                                    case 'Phi': ch = String.fromCharCode(0x03a6); break;
                                    case 'Chi': ch = String.fromCharCode(0x03a7); break;
                                    case 'Psi': ch = String.fromCharCode(0x03a8); break;
                                    case 'Omega': ch = String.fromCharCode(0x03a9); break;
                                    case 'alpha': ch = String.fromCharCode(0x03b1); break;
                                    case 'beta': ch = String.fromCharCode(0x03b2); break;
                                    case 'gamma': ch = String.fromCharCode(0x03b3); break;
                                    case 'delta': ch = String.fromCharCode(0x03b4); break;
                                    case 'epsilon': ch = String.fromCharCode(0x03b5); break;
                                    case 'zeta': ch = String.fromCharCode(0x03b6); break;
                                    case 'eta': ch = String.fromCharCode(0x03b7); break;
                                    case 'theta': ch = String.fromCharCode(0x03b8); break;
                                    case 'iota': ch = String.fromCharCode(0x03b9); break;
                                    case 'kappa': ch = String.fromCharCode(0x03ba); break;
                                    case 'lambda': ch = String.fromCharCode(0x03bb); break;
                                    case 'mu': ch = String.fromCharCode(0x03bc); break;
                                    case 'nu': ch = String.fromCharCode(0x03bd); break;
                                    case 'xi': ch = String.fromCharCode(0x03be); break;
                                    case 'omicron': ch = String.fromCharCode(0x03bf); break;
                                    case 'pi': ch = String.fromCharCode(0x03c0); break;
                                    case 'rho': ch = String.fromCharCode(0x03c1); break;
                                    case 'sigmaf': ch = String.fromCharCode(0x03c2); break;
                                    case 'sigma': ch = String.fromCharCode(0x03c3); break;
                                    case 'tau': ch = String.fromCharCode(0x03c4); break;
                                    case 'upsilon': ch = String.fromCharCode(0x03c5); break;
                                    case 'phi': ch = String.fromCharCode(0x03c6); break;
                                    case 'chi': ch = String.fromCharCode(0x03c7); break;
                                    case 'psi': ch = String.fromCharCode(0x03c8); break;
                                    case 'omega': ch = String.fromCharCode(0x03c9); break;
                                    case 'thetasym': ch = String.fromCharCode(0x03d1); break;
                                    case 'upsih': ch = String.fromCharCode(0x03d2); break;
                                    case 'piv': ch = String.fromCharCode(0x03d6); break;
                                    case 'ensp': ch = String.fromCharCode(0x2002); break;
                                    case 'emsp': ch = String.fromCharCode(0x2003); break;
                                    case 'thinsp': ch = String.fromCharCode(0x2009); break;
                                    case 'zwnj': ch = String.fromCharCode(0x200c); break;
                                    case 'zwj': ch = String.fromCharCode(0x200d); break;
                                    case 'lrm': ch = String.fromCharCode(0x200e); break;
                                    case 'rlm': ch = String.fromCharCode(0x200f); break;
                                    case 'ndash': ch = String.fromCharCode(0x2013); break;
                                    case 'mdash': ch = String.fromCharCode(0x2014); break;
                                    case 'lsquo': ch = String.fromCharCode(0x2018); break;
                                    case 'rsquo': ch = String.fromCharCode(0x2019); break;
                                    case 'sbquo': ch = String.fromCharCode(0x201a); break;
                                    case 'ldquo': ch = String.fromCharCode(0x201c); break;
                                    case 'rdquo': ch = String.fromCharCode(0x201d); break;
                                    case 'bdquo': ch = String.fromCharCode(0x201e); break;
                                    case 'dagger': ch = String.fromCharCode(0x2020); break;
                                    case 'Dagger': ch = String.fromCharCode(0x2021); break;
                                    case 'bull': ch = String.fromCharCode(0x2022); break;
                                    case 'hellip': ch = String.fromCharCode(0x2026); break;
                                    case 'permil': ch = String.fromCharCode(0x2030); break;
                                    case 'prime': ch = String.fromCharCode(0x2032); break;
                                    case 'Prime': ch = String.fromCharCode(0x2033); break;
                                    case 'lsaquo': ch = String.fromCharCode(0x2039); break;
                                    case 'rsaquo': ch = String.fromCharCode(0x203a); break;
                                    case 'oline': ch = String.fromCharCode(0x203e); break;
                                    case 'frasl': ch = String.fromCharCode(0x2044); break;
                                    case 'euro': ch = String.fromCharCode(0x20ac); break;
                                    case 'image': ch = String.fromCharCode(0x2111); break;
                                    case 'weierp': ch = String.fromCharCode(0x2118); break;
                                    case 'real': ch = String.fromCharCode(0x211c); break;
                                    case 'trade': ch = String.fromCharCode(0x2122); break;
                                    case 'alefsym': ch = String.fromCharCode(0x2135); break;
                                    case 'larr': ch = String.fromCharCode(0x2190); break;
                                    case 'uarr': ch = String.fromCharCode(0x2191); break;
                                    case 'rarr': ch = String.fromCharCode(0x2192); break;
                                    case 'darr': ch = String.fromCharCode(0x2193); break;
                                    case 'harr': ch = String.fromCharCode(0x2194); break;
                                    case 'crarr': ch = String.fromCharCode(0x21b5); break;
                                    case 'lArr': ch = String.fromCharCode(0x21d0); break;
                                    case 'uArr': ch = String.fromCharCode(0x21d1); break;
                                    case 'rArr': ch = String.fromCharCode(0x21d2); break;
                                    case 'dArr': ch = String.fromCharCode(0x21d3); break;
                                    case 'hArr': ch = String.fromCharCode(0x21d4); break;
                                    case 'forall': ch = String.fromCharCode(0x2200); break;
                                    case 'part': ch = String.fromCharCode(0x2202); break;
                                    case 'exist': ch = String.fromCharCode(0x2203); break;
                                    case 'empty': ch = String.fromCharCode(0x2205); break;
                                    case 'nabla': ch = String.fromCharCode(0x2207); break;
                                    case 'isin': ch = String.fromCharCode(0x2208); break;
                                    case 'notin': ch = String.fromCharCode(0x2209); break;
                                    case 'ni': ch = String.fromCharCode(0x220b); break;
                                    case 'prod': ch = String.fromCharCode(0x220f); break;
                                    case 'sum': ch = String.fromCharCode(0x2211); break;
                                    case 'minus': ch = String.fromCharCode(0x2212); break;
                                    case 'lowast': ch = String.fromCharCode(0x2217); break;
                                    case 'radic': ch = String.fromCharCode(0x221a); break;
                                    case 'prop': ch = String.fromCharCode(0x221d); break;
                                    case 'infin': ch = String.fromCharCode(0x221e); break;
                                    case 'ang': ch = String.fromCharCode(0x2220); break;
                                    case 'and': ch = String.fromCharCode(0x2227); break;
                                    case 'or': ch = String.fromCharCode(0x2228); break;
                                    case 'cap': ch = String.fromCharCode(0x2229); break;
                                    case 'cup': ch = String.fromCharCode(0x222a); break;
                                    case 'int': ch = String.fromCharCode(0x222b); break;
                                    case 'there4': ch = String.fromCharCode(0x2234); break;
                                    case 'sim': ch = String.fromCharCode(0x223c); break;
                                    case 'cong': ch = String.fromCharCode(0x2245); break;
                                    case 'asymp': ch = String.fromCharCode(0x2248); break;
                                    case 'ne': ch = String.fromCharCode(0x2260); break;
                                    case 'equiv': ch = String.fromCharCode(0x2261); break;
                                    case 'le': ch = String.fromCharCode(0x2264); break;
                                    case 'ge': ch = String.fromCharCode(0x2265); break;
                                    case 'sub': ch = String.fromCharCode(0x2282); break;
                                    case 'sup': ch = String.fromCharCode(0x2283); break;
                                    case 'nsub': ch = String.fromCharCode(0x2284); break;
                                    case 'sube': ch = String.fromCharCode(0x2286); break;
                                    case 'supe': ch = String.fromCharCode(0x2287); break;
                                    case 'oplus': ch = String.fromCharCode(0x2295); break;
                                    case 'otimes': ch = String.fromCharCode(0x2297); break;
                                    case 'perp': ch = String.fromCharCode(0x22a5); break;
                                    case 'sdot': ch = String.fromCharCode(0x22c5); break;
                                    case 'lceil': ch = String.fromCharCode(0x2308); break;
                                    case 'rceil': ch = String.fromCharCode(0x2309); break;
                                    case 'lfloor': ch = String.fromCharCode(0x230a); break;
                                    case 'rfloor': ch = String.fromCharCode(0x230b); break;
                                    case 'lang': ch = String.fromCharCode(0x2329); break;
                                    case 'rang': ch = String.fromCharCode(0x232a); break;
                                    case 'loz': ch = String.fromCharCode(0x25ca); break;
                                    case 'spades': ch = String.fromCharCode(0x2660); break;
                                    case 'clubs': ch = String.fromCharCode(0x2663); break;
                                    case 'hearts': ch = String.fromCharCode(0x2665); break;
                                    case 'diams': ch = String.fromCharCode(0x2666); break;

                                    default: ch = ''; break;
                              }
                        }
                        i = semicolonIndex;
                  }
            }
            out += ch;
      }
      return out;
}


/**************************************************************************************************
 Window navigation and open, MessageBox
**************************************************************************************************/

function osGotoUrl(url)
{
    window.location = url;
}

function osWindowOpenFullScreen(url)
{
	window.open(url,"","fullscreen=yes")
}

function osShowWaitMessage(waitMessage)
{
	alert(waitMessage);
}

/**************************************************************************************************
 Cookie management
**************************************************************************************************/

function osCookieCreate(name,value,days)
{
	if (days)
	{
		var date = new Date();
		date.setTime(date.getTime()+(days*24*60*60*1000));
		var expires = "; expires="+date.toGMTString();
	}
	else var expires = "";
	document.cookie = name+"="+value+expires+"; path=/";
}

function osCookieRead(name)
{
	var nameEQ = name + "=";
	var ca = document.cookie.split(';');
	for(var i=0;i < ca.length;i++) {
		var c = ca[i];
		while (c.charAt(0)==' ') c = c.substring(1,c.length);
		if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
	}
	return null;
}

function osCookieErase(name)
{
	osCookieCreate(name,"",-1);
}

/**************************************************************************************************
	Controls initalizations
**************************************************************************************************/

function osInitControls(src,debug)
{
    if (src == null) {
        // Global Initialization of javascript code inside "libraries.js"		
        OsirisLibraries.init(null);

        osInitControls(this.document.body);

        setTimeout('Osiris.initAdvertising();', 3000);
    }
    else {
        if (src.attributes) {
            if (src.attributes["data-os-otype"]) {
                var otype = src.attributes["data-os-otype"].nodeValue;

                if (otype == "popup") {
                    osPopupCreate(src);
                }

                if (otype == "roller") {
                    osRollerCreate(src);
                }

                if (otype == "tab") {
                    osTabCreate(src);
                }

                if (otype == "dialog") {                    
                    Osiris.dialog(src);                         
    								jQuery(src).dialog("open");
                }
                                
                if (otype == "bool") {
                    Osiris.controlBoolean(src);
                }

                if (otype == "number") {
                    Osiris.controlNumber(src);
                }
                
                if (otype == "slider") {
                    Osiris.controlSlider(src);
                }

                if (otype == "string") {
                    Osiris.controlString(src);
                }
                
                if (otype == "omleditor") {
                		Osiris.controlOmlEditor(src);
                }
                
                if (otype == "textarea") {
                	$(src).autosize();		
                }
            }

            if (src.attributes["data-os-tooltip"]) {
                var tooltip = src.attributes["data-os-tooltip"].nodeValue;
                osTooltipInit(src, tooltip);
            }
                        
            /*
            if( (src.attributes) && (src.attributes["data-os-drag"]) )
            {
            osDragEnable(src);
            }
            */

            if (src.attributes["data-os-url"]) {
            	  osDynamicDiv(src);
            }



            if (src.attributes["data-os-delayedsrc"]) {
                // May occur that browser hang waiting for image src, before osiris.js initialization.
                // This lap allow specific resources to be loaded after.
                //alert(src.attributes["data-os-delayedsrc"].nodeValue);
                $(src).attr("src", src.attributes["data-os-delayedsrc"].nodeValue);
            }

            // Control Initialization of javascript code inside "libraries.js"
            OsirisLibraries.init(src);
        }

        for (var i = 0; i < src.childNodes.length; i++) {
            osInitControls(src.childNodes[i], debug);
        }
    }
}

/**************************************************************************************************
	Ajax
**************************************************************************************************/

/*
function osAjax_GetXmlHttpObject(handler)
{ 
	var objXMLHttp=null
	if (window.XMLHttpRequest)
	{
		objXMLHttp=new XMLHttpRequest()
	}
	else if (window.ActiveXObject)
	{
		objXMLHttp=new ActiveXObject("Microsoft.XMLHTTP")
	}
	return objXMLHttp;
}
*/

/*
function osAjax_InnerHtmlFromUrl_StateChanged(src)
{
    if (src.xmlHttp.readyState==4 || src.xmlHttp.readyState=="complete")
	{ 	    
	    var response = src.xmlHttp.responseText; 	    
	    try 
	    {
	        var html = response;
	        
	        html = execJS(html);

	        src.innerHTML = html;
	        	
	        if(src.onReady)
	            src.onReady();
        }
        catch (e) 
        {
            // IE fails unless we wrap the string in another element.
            if(src.ie_wrappingDiv)
                src.removeChild(src.ie_wrappingDiv);
            src.ie_wrappingDiv = document.createElement('div');
            src.ie_wrappingDiv.innerHTML = response;
            src.appendChild(src.ie_wrappingDiv);
        }
	
	    for(var i=0;i<src.childNodes.length;i++)
	    {	
		    osInitControls(src.childNodes[i]);
	    }		
        
	    if(src.ajaxWaitIconActive)
	    {
		    src.className = src.oldClassName;
		    src.oldClassName = "";
	    }
    }
}
*/

function osDynamicDiv(src, url, waitStyle, effect)
{
    if(src == null)
        return;
        
    if(url == null)
        url = src.attributes["data-os-url"].nodeValue;	
                
    if( (effect == null) && src.attributes && src.attributes["data-os-effect"] )
        effect = src.attributes["data-os-effect"].nodeValue;	
        
    if( (waitStyle == null) && src.attributes && src.attributes["data-os-wait"] )
        waitStyle = src.attributes["data-os-wait"].nodeValue;	
		
	/*    
	if(waitStyle == null)
	{
	    if(src.attributes["data-os-wait"])
	    {
	        waitStyle = src.attributes["data-os-wait"].nodeValue;	        
	    }
	}
	src.ajaxWaitStyle = waitStyle;
	*/
	
	src.updateUrl = url;
	src.effect = effect;
	src.wait = waitStyle;
	src.updateContent = function()
	{
		Osiris.contentFromUrl(this,this.updateUrl,this.effect,this.wait);		
	}
	
	src.onReady = function()
	{
		// Se nel div ottenuto c'è l'informazione di refresh, creo un timer per l'aggiornamento.		
		var divChild = this.getElementsByTagName("div")[0];
		
		var refresh = 0;
		if(divChild && divChild.attributes && divChild.attributes["data-os-refresh"])
			refresh = divChild.attributes["data-os-refresh"].nodeValue;
		
		if(refresh != 0)
		{
			Osiris.ensureId(this);
			setTimeout("osGetById('" + this.id + "').updateContent();",refresh*1000);
		}
		else if(src.attributes && src.attributes["data-os-completed"])
		{
		    window.location = src.attributes["data-os-completed"].nodeValue;
		}
	}
	
	src.updateContent();
}

/**************************************************************************************************
	Drag
**************************************************************************************************/

document.onmousemove = mouseMove;
document.onmouseup   = mouseUp;

var dragObject  = null;
var mouseOffset = null;
  
function mouseCoords(ev)
{
	if(ev.pageX || ev.pageY)
	{
		return {x:ev.pageX, y:ev.pageY};
	}
	if(document.body)
	return {
		x:ev.clientX + document.body.scrollLeft - document.body.clientLeft,
		y:ev.clientY + document.body.scrollTop  - document.body.clientTop
	};
}

function getMouseOffset(target, ev)
{
    ev = ev || window.event;

    var docPos    = getPosition(target);
    var mousePos  = mouseCoords(ev);
    return {x:mousePos.x - docPos.x, y:mousePos.y - docPos.y};
}

function getPosition(e)
{
    var left = 0;
    var top  = 0;

    while (e.offsetParent)
    {
        left += e.offsetLeft;
        top  += e.offsetTop;
        e     = e.offsetParent;
    }

    left += e.offsetLeft;
    top  += e.offsetTop;

    return {x:left, y:top};
}

function panelMove(src, newX, newY)
{	
	newX = parseInt(newX);
	newY = parseInt(newY);
	
	if(newX < 0)
		newX = 0;
		
	if(newY < 0)
		newY = 0;
		
//alert(src.style.position);
	//src.style.position = 'absolute';	
	if(Osiris.is_ie)
	{
		src.style.top      = newY;
		src.style.left     = newX;		    
	}
	else
	{	
		newX += '';
		newY += '';
	
		if(osEndsWith(newX,'px') == false)
			newX += "px";
		if(osEndsWith(newY,'px') == false)
			newY += "px";			
		
			
		src.style.top      = newY;
		src.style.left     = newX;		    		
	}    	
}

function mouseMove(ev)
{      
    ev = ev || window.event;
    var mousePos = mouseCoords(ev);

    if(dragObject)
    {
	    var newX, newY;
	    newX = (mousePos.x - mouseOffset.x);
	    newY = (mousePos.y - mouseOffset.y);
	
	    panelMove(dragObject, newX, newY);	

        return false;
    }
}
     
function mouseUp()
{
    if(dragObject == null)
        return;
    
    if(dragObject.saveStatus)    
    	dragObject.saveStatus();
    
    osAlphaSet(dragObject, 1);
    dragObject = null;
}

function osDragEnable(src)
{
    var id = src.id;

    var objMove = osGetById(id);
    var objDragArea = osGetById(id + "DragArea");
    
    if(!objMove) return;
    if(!objDragArea) return;
        
    //objMove.style.display = "none";
    //objMove.visible = false;
    objDragArea.style.cursor = "pointer";
    objDragArea.objMove = objMove;
   

    objDragArea.onmousedown = function(ev)
    {
        dragObject  = this.objMove;
        mouseOffset = getMouseOffset(this, ev);
        
        osAlphaSet(dragObject, 0.8);

        return false;
    }    
}
      
/**************************************************************************************************
	Redirect & Confirm actions
**************************************************************************************************/

function osRedirect(href, confirmMsg, inNewWindow)
{
	if(confirmMsg != "")
		if(!confirm(confirmMsg))
			return;
	
	if( (inNewWindow) && (inNewWindow!=0) )
	{
		window.open(href, "_blank");
	}
	else
	{
		window.location = href;
	}
}

function osRedirectConfirm(msg, href)
{
	var answer = confirm (msg)
	if (answer)
		window.location = href;
}

globalWaitMessage = null;

function osRedirectWait(msg, href)
{
	if(globalWaitMessage==null)
	{
		globalWaitMessage = osCreate("img");
		globalWaitMessage.className = "os_messageWait_back";
		globalWaitMessage.id = "osMessage";
		globalWaitMessage.src = "/skins/osiris/images/message_wait_bg.png";
		document.body.appendChild(globalWaitMessage);		
		
		globalWaitMessage.objBody = osCreate("div");
		globalWaitMessage.objBody.className = "os_messageWait_div";
		globalWaitMessage.objIcon = osCreate("img");
		globalWaitMessage.objIcon.className = "os_messageWait_icon";
		globalWaitMessage.objIcon.src = "/skins/osiris/images/message_wait_icon.gif";		
		globalWaitMessage.objBody.appendChild(globalWaitMessage.objIcon);
		globalWaitMessage.objMessage = osCreate("div");
		globalWaitMessage.objMessage.className = "os_messageWait_message";
		globalWaitMessage.objBody.appendChild(globalWaitMessage.objMessage);
		
		document.body.appendChild(globalWaitMessage.objBody);		
	}
	
	var body = document.body;
	
	var docwidth=(Osiris.is_ie)? body.clientWidth : window.innerWidth;
	var docheight=(Osiris.is_ie)? body.clientHeight: window.innerHeight;
	var scroll_top=(Osiris.is_ie)? body.scrollTop : window.pageYOffset;
	
	docwidth = window.innerWidth;
	docheight = window.innerHeight;
	
	alert("scroll:"+scroll_top+", w:"+docwidth+", h:"+docheight);

	globalWaitMessage.style.display = "block";
	globalWaitMessage.style.left=0;
	globalWaitMessage.style.top=0;	
	globalWaitMessage.style.width=body.offsetWidth + "px";
	globalWaitMessage.style.height=body.offsetHeight + 5 + "px";
	
	globalWaitMessage.objMessage.textContent = msg;
	globalWaitMessage.objMessage.innerText = msg;
	globalWaitMessage.objBody.style.display = "block";
	//globalWaitMessage.objBody.style.left=globalWaitMessage.offsetWidth/2 - globalWaitMessage.objBody.offsetWidth/2 + "px";
	//globalWaitMessage.objBody.style.top=globalWaitMessage.offsetHeight/2 - globalWaitMessage.objBody.offsetHeight/2 + "px";
	globalWaitMessage.objBody.style.left=docwidth/2 - globalWaitMessage.objBody.offsetWidth/2 + "px";
	globalWaitMessage.objBody.style.top=scroll_top+docheight/2 - globalWaitMessage.objBody.offsetWidth/2 + "px";
	
	
	
	globalWaitMessage.redirect_href = href;
	globalWaitMessage.redirect = function(evt)
	{
		window.location = this.redirect_href;
	}
		
	t = window.setTimeout("globalWaitMessage.redirect()",500); 			
}

/**************************************************************************************************
	Tooltip
	
Esempio di aggancio:
    <a href="xxx" data-os-tooltip="Tooltip Text">MyTextWithTooltip</a>
**************************************************************************************************/

globalTooltip = null;

function osTooltipInit(src, text)
{    
    if(globalTooltip == null)
        osTooltipCreate();
        
    src.tooltip = text;
    
    if(src.onmouseover == null)        
        src.onmouseover = function (e) { osTooltipShow(e,this); }
                
    if(src.onmousemove == null)
        src.onmousemove = function (e) { osTooltipMove(e); }
        
    if(src.onmouseout == null)
        src.onmouseout = function (e) { osTooltipHide(e,this); }        
}

function osTooltipCreate()
{
    globalTooltip = osCreate("div");   
        
    globalTooltip.style.display = "none";
    globalTooltip.style.position = "absolute";    
    globalTooltip.style.zIndex = 9999;
    
    osClassNameSet(globalTooltip,"os_tooltip");
    
    osGetBody().appendChild(globalTooltip);
}

function osTooltipShow(e, objTooltip)
{    
    globalTooltip.innerHTML = objTooltip.tooltip;
    globalTooltip.left = 0;
    globalTooltip.top = 0;
    
    osTooltipMove(e);
    
    jQuery(globalTooltip).stop(true,true).fadeIn(Osiris.effectsSpeedFast);
}

function osTooltipMove(e)
{
    osMoveOnCursor(e, globalTooltip, -10, (Osiris.is_opera ? -45:-15));    
}

function osTooltipHide(e, objElement)
{
	if(osCheckMouseLeave(e,objElement) == true)
	{		
		jQuery(globalTooltip).stop(true,true).fadeOut(Osiris.effectsSpeedFast);
	}
}

/**************************************************************************************************
	Prompts & Windows
**************************************************************************************************/

function osDialog(src)
{
	/*
    jQuery(function() 
      {
        jQuery(src).dialog(
        {
          autoOpen: false,
          show: 'fade',
          hide: 'fade',
          width: '60%',
          bgiframe: true,
          modal: true,
          close: function(event, ui) { jQuery(this).remove(); }
        });
      });
      */
    Osiris.dialog(src); 
    
    jQuery(src).dialog("open");
}

function osConfirm(promptTitle, promptMessage, promptButton, promptCancel, promptHref)
{
	return Osiris.confirm(promptTitle, promptMessage, promptButton, promptCancel, promptHref);	
}

function osPrompt(msg)
{
	return Osiris.prompt(msg,"");
}

function osCopyClipboard(promptTitle, promptMessage, promptButton)
{
	return Osiris.copyClipboard(promptTitle, promptMessage, promptButton);    
}

function osPromptInteger(msg, minValue,maxValue)
{
	var strValue = osPrompt(msg);
	if(strValue == null)
		return null;
		
	var value = parseInt(strValue);
	if(isNaN(value))
		return null;
	if(value<minValue)
		return minValue;
	if(value>maxValue)
		return maxValue;
	return value;
}

/**************************************************************************************************
	Roller
**************************************************************************************************/


function osRollerCreate(src)
{	
	var objRoller = src;
	
	var allowClose = src.attributes["data-os-allowClose"] ? src.attributes["data-os-allowClose"].nodeValue : "true";
	var startClose = src.attributes["data-os-startClose"] ? src.attributes["data-os-startClose"].nodeValue : "false";
	objRoller.storage = src.attributes["data-os-storage"] ? src.attributes["data-os-storage"].nodeValue : "";
	
	startClose = Osiris.storageGet("roller." + objRoller.storage, startClose);
	
	objRoller.objHeader = osGetChildWithAttribute(objRoller, "data-os-roller", "header");
	if(!objRoller.objHeader) 
	{
	    // E' importante segnalare l'errore, che deve essere corretto, altrimenti la osGetChildWithAttribute scorrerebbe
	    // tutto il dom inutilmente.
	    console.error("Osiris: data-os-roller='header' not found.");
	    return;
	}
	objRoller.objHeader.objRoller=objRoller;
	
	objRoller.objBody = osGetChildWithAttribute(objRoller, "data-os-roller", "body");
	if(!objRoller.objBody)
	{
	    console.error("Osiris: data-os-roller='body' not found.");
	    return;
	}
	objRoller.objBody.objRoller=objRoller;	
		
	if (allowClose == "true")
	{
		objRoller.objHeader.style.cursor = "pointer";
		objRoller.objHeader.onclick=function () { osRollerSwitchStatus(this.objRoller); }
	}	
	
	if (startClose == "true")
	{
		osRollerClose(objRoller, false);
	}
	else
	{
		osRollerOpen(objRoller, false);
	}	
}

function osRollerClose(src, withEffect)
{
	src.rollerStatus="close";
	if(src.storage != "")
		Osiris.storageSet("roller." + src.storage, true);
	
	var className = osGetAttributeValue(src.objHeader,"data-os-class_expand");
	if(className) src.objHeader.className = className;	
		
	if(withEffect)
	{
		//new Effect.BlindUp(src.objBody, { queue: 'end', duration: 0.3 });
		jQuery(src.objBody).slideUp(Osiris.effectsSpeedFast);
	}
	else
		src.objBody.style.display="none";
}

function osRollerOpen(src, withEffect)
{
	src.rollerStatus="open";
	if(src.storage != "")
		Osiris.storageSet("roller." + src.storage, false);
	
	var className = osGetAttributeValue(src.objHeader,"data-os-class_collapse");
	if(className) src.objHeader.className = className;
				
	if(withEffect)
	{
		//new Effect.BlindDown(src.objBody, { queue: 'end', duration: 0.3 });
		jQuery(src.objBody).slideDown(Osiris.effectsSpeedFast);
	}
	else
		src.objBody.style.display="block";			
}

function osRollerSwitchStatus(src)
{	
	if (src.rollerStatus==null) src.rollerStatus="open";
	
	if (src.rollerStatus=="open")
	{
		osRollerClose(src, true);		
	}
	else
	{
		osRollerOpen(src, true);		
	}	
}

/**************************************************************************************************
	Popup
**************************************************************************************************/

function osPopupCreate(src)
{	
	objPopup = src;
	
	var mode = objPopup.attributes["data-os-popupMode"] ? src.attributes["data-os-popupMode"].nodeValue : "popup";
	
	objPopup.objHeader = osGetChildWithAttribute(objPopup, "data-os-popup", "header");
	if(!objPopup.objHeader) 
	{
	    // E' importante segnalare l'errore, che deve essere corretto, altrimenti la osGetChildWithAttribute scorrerebbe
	    // tutto il dom inutilmente.
	    console.error("Osiris: data-os-popup='header' not found.");
	    return;
	}
	objPopup.objHeader.objPopup=objPopup;
	
	objPopup.objBody = osGetChildWithAttribute(objPopup, "data-os-popup", "body");
	if(!objPopup.objBody)
	{
	    console.error("Osiris: data-os-popup='body' not found.");
	    return;
	}
	
	// Intermediary DIV.
	// Useful only to avoid jerky effects with Scriptaculus.
	// http://goingspare.wordpress.com/2007/08/03/fix-jerky-scriptaculous-scriptaculous-animation-effects/
	
	bodyDiv = osCreate("div");
	bodyDiv.style.display = "none";
	
	objPopup.appendChild(bodyDiv);
	bodyDiv.appendChild(objPopup.objBody);
	objPopup.objBody.style.display = "block";
	objPopup.objBody = bodyDiv;
	
		
		
	 	
	objPopup.objBody.objPopup=objPopup;	
	objPopup.objBody.os_visible = false;
		
		
	objPopup.mode=mode;
	objPopup.objBody.objHeader=objPopup.objHeader;		
		
	if (mode=="tooltip")
	{
		objPopup.objHeader.onmouseout=function (e) { osPopupHide(e,this.objPopup,"appear"); }
		objPopup.objHeader.onmouseover=function (e) { osPopupShow(e,this.objPopup,"appear"); osPopupMoveOnCursor(e,this.objPopup,-15); }
		objPopup.objHeader.onmousemove=function (e) { osPopupMoveOnCursor(e,this.objPopup,-15); }
	}
	else if (mode=="static")
	{
		objPopup.objHeader.onmouseout=function (e) { osPopupHide(e,this.objPopup,"blind"); }
		objPopup.objHeader.onmouseover=function (e) { osPopupShow(e,this.objPopup,"blind"); }
	}
	else if (mode=="staticclick")
	{
		objPopup.objHeader.onclick=function (e) { osPopupInvertShow(e,this.objPopup,"blind",false); return true; }		
	}	
	else if (mode=="popup")
	{
		objPopup.objHeader.onmouseover=function (e) { osPopupClickableShow(e,this.objPopup,this); }		
		objPopup.objBody.onmouseout=function (e) { osPopupClickableMayHide(e,this.objPopup,this); }		
		objPopup.objHeader.onmouseout=function (e) { osPopupClickableMayHide(e,this.objPopup,this); }		
	}
	else if (mode=="popupclick")
	{
		//objPopup.objHeader.onclick=function (e) { osPopupClickableShow(e,this.objPopup,this); }
		//objPopup.objBody.onmouseout=function (e) { osPopupClickableHide(e,this.objPopup,this); }		
		objPopup.objHeader.onclick=function (e) { osPopupInvertShow(e,this.objPopup,"appear",true); return true; }
	}	
	else if (mode=="window")
	{
		jQuery(objPopup.objBody).dialog(
			{
				autoOpen: false,
				show: 'fade',
				hide: 'fade',
				bgiframe: false,
				resizable: true,
				width:'50%',
				minHeight:140,
				modal: false,		
			});
		
		objPopup.objHeader.onclick=function (e) { osPopupInvertShow(e,this.objPopup,"appear",true); return true; }
		
	}
	else if (mode=="dialog")
	{
		//$('.selector').dialog({ dialogClass: 'notitle' });
		Osiris.dialog(objPopup.objBody);
		
		objPopup.objHeader.onclick=function (e) { osPopupInvertShow(e,this.objPopup,"appear",true); return true; }
	}
	
	osPopupHide(null,objPopup);		
}

function osPopupShow(e,objPopup,effectMode)
{
	if( (objPopup) && (objPopup.objBody) && (objPopup.objBody.os_visible == false) )
	{
	    if(effectMode == "appear")
	    {
	        //Effect.Appear(objPopup.objBody, { queue: 'end', duration: 0.3 });
	        jQuery(objPopup.objBody).fadeIn(Osiris.effectsSpeedSlow);
	    }
	    else if(effectMode == "blind")
	    {
	        //Effect.BlindDown(objPopup.objBody, { queue: 'end', duration: 0.3 });
	        jQuery(objPopup.objBody).slideDown(Osiris.effectsSpeedSlow);
	    }
	    else
	        objPopup.objBody.style.display="block";		
	    
	    objPopup.objBody.os_visible = true;
	    return true;
	}
	else
	    return false;
}

function osPopupHide(e,objPopup,effectMode)
{
    if( (objPopup) && (objPopup.objBody) && (objPopup.objBody.os_visible == true) )
	{
	    if(effectMode == "appear")
	    {
	        //Effect.Fade(objPopup.objBody, { queue: 'end', duration: 0.3 });
	        jQuery(objPopup.objBody).fadeOut(Osiris.effectsSpeedSlow);
	    }
	    else if(effectMode == "blind")
	    {
	        //Effect.BlindUp(objPopup.objBody, { queue: 'end', duration: 0.3 });
	        jQuery(objPopup.objBody).slideUp(Osiris.effectsSpeedSlow);
	    }
	    else
	        objPopup.objBody.style.display="none";	    
	    
	    objPopup.objBody.os_visible = false;
	}
}

function osPopupClickableShow(e,objPopup,objElement)
{
	// TODO: Ogni tanto rimane appeso.
	// Secondo me, è perchè nel momento in cui il div di body compare, il mouse non è + sopra, e quindi il body non spara il onmouseout.
	// 04/11/2007 - Ho provato di tutto, ma nulla; e creare un timer che dopo x controlla se effettivamente il mouse è già fuori non si può, xchè dall'evento chiamato dal
	// timer non ho l'oggetto evento, e quindi non posso sapere le coordinate del mouse.
	// Neanche ridefinire document.body.onmousemouse , nell'evento "e" lanciato non ho informazioni sull'elemento su cui è il cursore.
	if(osPopupShow(e,objPopup,"appear") == true)
	    osPopupMoveOnCursor(e,objPopup,10);	
}

function osPopupClickableMayHide(e,objPopup,objElement)
{
    /*objPopup
	if(osCheckMouseLeave(e,objElement))
	{		
		osPopupHide(e,objPopup,"appear");		
	}
	*/
	var s1 = osCheckMouseLeave(e,objPopup.objBody);
	var s2 = osCheckMouseLeave(e,objPopup.objHeader);
	
	if( (s1 == true) && (s2 == true) )	
	{
	    osPopupHide(e,objPopup,"appear");
	}
}

function osPopupInvertShow(e,objPopup,effectMode, withMove)
{
  if (objPopup)
	{
		var mode = objPopup.mode;
		
		if(mode == "window")
		{
			jQuery(objPopup.objBody).dialog("open");						
		}
		else if(mode == "dialog")
		{
			jQuery(objPopup.objBody).dialog("open");			
		}
		else
		{
		  if(objPopup.objBody.os_visible == true)		
			{
				osPopupHide(e,objPopup,effectMode);
			}
			else
			{
				osPopupShow(e,objPopup,effectMode);
				if(withMove)
				    osPopupMoveOnCursor(e,objPopup,-15);
			}
		}
	}
}	

function osPopupMoveOnCursor(e,objPopup,offset)
{
	osMoveOnCursor(e,objPopup.objBody,offset);
}

/**************************************************************************************************
	Tab
**************************************************************************************************/

function osTabCreate(src)
{	
	var objTab = src;
	
	var startPage = src.attributes["data-os-startPage"] ? src.attributes["data-os-startPage"].nodeValue : 0;
	//var tabSize = src.attributes["data-os-tabSize"] ? src.attributes["data-os-tabSize"].nodeValue : "200";
	var tabLayout = src.attributes["data-os-layout"] ? src.attributes["data-os-layout"].nodeValue : "top";
	var tabStorage = src.attributes["data-os-storage"] ? src.attributes["data-os-storage"].nodeValue : "";
			
	//objTab.tabSize = tabSize;
	objTab.tabLayout = tabLayout;
	objTab.tabStorage = tabStorage;
	
	objTab.className = "os_tab";
	
	startPage = Osiris.storageGet("tab." + objTab.tabStorage, startPage);
		
	objTab.headers = new Array(0);
	objTab.bodies = new Array(0);		
		
	for(var objIndex=0;objIndex<objTab.childNodes.length;objIndex++)
	{
	 	objChild=objTab.childNodes[objIndex];
		
		if(objChild.attributes != null)
		{
			tabAttr=objChild.attributes["data-os-tabType"];
			if(tabAttr)
			{
				var pageIndex = -1;
			    
				if(tabAttr.nodeValue=="header")
				{					
					pageIndex = objTab.headers.length;
					if(objTab.tabLayout == "top")
						objChild.className = "os_tab_top_header";						
					else
						objChild.className = "os_tab_left_header";
				  	
					objChild.style.cursor = "pointer";					
					objChild.style.display = "block";
					objChild.onclick = function () { this.objTab.switchPage(this.pageIndex); }										
					objTab.headers[pageIndex] = objChild;
				}
				else if(tabAttr.nodeValue=="body")
				{
					pageIndex = objTab.bodies.length;
				  objChild.className = "os_tab_" + objTab.tabLayout + "_body";	
				  objChild.style.display = "none";			    
					objTab.bodies[pageIndex] = objChild;
				}							
				
				objChild.pageIndex = pageIndex;
				objChild.objTab = objTab;
			}
			
		}
	}		
	
	// Now, move/reorganize div pages.	
	objTab.headersDiv = osCreate("div");
	objTab.headersDiv.className = "os_tab_" + objTab.tabLayout + "_headers";	
		
	objTab.bodiesDiv = osCreate("div");
	objTab.bodiesDiv.className = "os_tab_" + objTab.tabLayout + "_bodies";		
	
	var c=0;
	for(var i=0;i<objTab.bodies.length;i++)
	{
		var objHeader = objTab.headers[i];
		var objBody = objTab.bodies[i];	
		
		objTab.headersDiv.appendChild(objHeader);
		objTab.bodiesDiv.appendChild(objBody);
	}
	
	objTab.appendChild(objTab.headersDiv);
	objTab.appendChild(objTab.bodiesDiv);
	
	divClear = osCreate("div");
	divClear.style.clear = "both";
	objTab.headersDiv.appendChild(divClear);
	
	objTab.expand = function(withEffect)
	{	    
		for(var i=0;i<objTab.bodies.length;i++)
		{	    		
			var objHeader = this.headers[i];
			var objBody = this.bodies[i];
			
			//if(withEffect)
			if(false)
			{
				if(objHeader.style.display != "none")
				{
					jQuery(objHeader).stop(true,true).slideUp(Osiris.effectsSpeedSlow);
					//jQuery(objHeader).stop(true,true).fadeOut(Osiris.effectsSpeedSlow);
					//jQuery(objHeader).fadeOut( { queue: true, duration: Osiris.effectsSpeedSlow } );
				}
				if(objBody.style.display != "block")
				{
					jQuery(objBody).stop(true,true).slideDown(Osiris.effectsSpeedSlow);
					//jQuery(objBody).stop(true,true).fadeIn(Osiris.effectsSpeedSlow);
					//jQuery(objBody).fadeIn( { queue: true, duration: Osiris.effectsSpeedSlow } );
				}
			}
			else
			{
				objHeader.style.display = "none";
				objBody.style.display = "block";
			}
		}
		objTab.collapsed = false;
	}
	
	objTab.collapse = function()
	{
		// Verifico che gli header siano aperti
		for(var i=0;i<objTab.bodies.length;i++)
		{	    		
			var objHeader = this.headers[i];
			var objBody = this.bodies[i];
			
			if(objHeader.style.display != "block")
			{
				//Effect.BlindDown(objHeader, { queue: 'end', duration: 0.3 });			    	
				jQuery(objHeader).stop(true,true).slideDown(Osiris.effectsSpeedSlow);
				//jQuery(objHeader).stop(true,true).fadeIn(Osiris.effectsSpeedSlow);				
				//jQuery(objHeader).fadeIn( { queue: true, duration: Osiris.effectsSpeedSlow } );
			}
		}
		
		objTab.switchPage(startPage, false);
		objTab.collapsed = true;
	}
	
	objTab.switchPage = function(pageIndex, withEffect)
	{
		if(withEffect == null)
			withEffect = true;

		/*    
		if(pageIndex == this.pageIndex)
		return;
		*/

		// Chiudo il vecchio
		for(var i=0;i<objTab.bodies.length;i++)
		{	    		
			var objHeader = this.headers[i];
			var objBody = this.bodies[i];

			if( (objBody.pageIndex != pageIndex) && (objBody.style.display != "none") )
			{
				if(objTab.tabLayout == "top")
					objHeader.className = "os_tab_top_header";
				else
					objHeader.className = "os_tab_left_header";
				//if(withEffect)
				if(false)
				{
					if(objBody.style.display != "none")
					{
						jQuery(objBody).stop(true,true).slideUp(Osiris.effectsSpeedSlow);
						//jQuery(objBody).stop(true,true).fadeOut(Osiris.effectsSpeedSlow);
						//jQuery(objBody).fadeOut( { queue: true, duration: Osiris.effectsSpeedSlow } );
					}
				}
				else
				{
					objBody.style.display = "none";
				}
			}	        
		}		   
		
		// Apro il nuovo
		for(var i=0;i<objTab.bodies.length;i++)
		{	    		
			var objHeader = this.headers[i];
			var objBody = this.bodies[i];

			if(objBody.pageIndex == pageIndex)
			{
				if(objTab.tabLayout == "top")
					objHeader.className = "os_tab_top_header_selected";
				else
					objHeader.className = "os_tab_left_header_selected";
				//objBody.style.display = "block";			    
				//Effect.Appear(objBody, { queue: 'end', duration: 1 });			    	
				//if(withEffect)
				if(false)
				{
					if(objBody.style.display != "block")
					{
						//Effect.BlindDown(objBody, { queue: 'end', duration: 0.3 });			    			    	    
						jQuery(objBody).stop(true,true).slideDown(Osiris.effectsSpeedSlow);
						//jQuery(objBody).stop(true,true).fadeIn(Osiris.effectsSpeedSlow);
						//jQuery(objBody).fadeIn( { queue: true, duration: Osiris.effectsSpeedSlow } );
					}
				}
				else
				{
					objBody.style.display = "block";
				}
			}	        
		} 

		this.pageIndex = pageIndex;

		if(this.onSwitchPage)
			this.onSwitchPage();

		if(objTab.tabStorage != "")
			Osiris.storageSet("tab." + objTab.tabStorage, pageIndex);
		/*
		if(objTab.cookie != "")
		{
			osCookieCreate(objTab.cookie, pageIndex);
		}
		*/
	}	
		
	objTab.collapse();
	
	objTab.style.display = "block";
}

function osPagerGotoPage(href, minValue,maxValue,itemsInPage)
{
	var fx=href;
	
	var value = osPromptInteger('Goto page:', minValue,maxValue);
	if(value != null)
	{
		for(;;)
		{
			var tmp = fx;
			fx = fx.replace("#","'");
			if(tmp == fx) break;
		}
		var offset = (value-1) * itemsInPage;
		
		if(fx.substr(0,11) == "javascript:")
		{			
			fx = fx.replace(":999999",":"+offset.toString());
			eval(fx);
		}
		else 
		{
			fx = fx.replace("offset=999999","offset="+offset.toString());
			document.location = fx;			
		}
	}	
}

/**************************************************************************************************
	OSML Codes
**************************************************************************************************/

function osOmlInject(id, code)
{	
    code = code.replace("\\r","\r");
    code = code.replace("\\n","\n");
    var obj = osGetById(id);
    obj.innerHTML = code;
}

/* Not used anymore.
function osOmlSelectAll(src)
{
	var e = src.parentNode.parentNode.getElementsByTagName('code')[0];
	
	// Not IE
	if (window.getSelection)
	{
		var s = window.getSelection();
		// Safari
		if (s.setBaseAndExtent)
		{
			s.setBaseAndExtent(e, 0, e, e.innerText.length - 1);
		}
		// Firefox and Opera
		else
		{
			// workaround for bug # 42885
			if (window.opera && e.innerHTML.substring(e.innerHTML.length - 4) == '<BR>')
			{
				e.innerHTML = e.innerHTML + '&nbsp;';
			}

			var r = document.createRange();
			r.selectNodeContents(e);
			s.removeAllRanges();
			s.addRange(r);
		}
	}
	// Some older browsers
	else if (document.getSelection)
	{
		var s = document.getSelection();
		var r = document.createRange();
		r.selectNodeContents(e);
		s.removeAllRanges();
		s.addRange(r);
	}
	// IE
	else if (document.selection)
	{
		var r = document.body.createTextRange();
		r.moveToElementText(e);
		r.select();
	}
}
*/

function osOmlUrlsLaunch(obj)
{
	var url = obj.a[obj.n];
	obj.n++;
	if( (obj.n == obj.a.length) && (obj.iv) )
	{
		clearInterval(obj.iv);
		obj.iv = null;			
	}
	
	document.location = url;	
	return true;    
}

function osOmlUrlsLaunchAll(obj)
{
	obj.iv = setInterval("osOmlUrlsLaunch(osmlUrls_" + obj.id + ")",250);
	obj.n = 0;
}

function osOmlUrlsPopup(obj)
{
	obj.theWin = window.open("", "links", "width=800,height=600,scrollbars=1");
	obj.theWin.document.title = "Links";
	obj.theWin.document.write(obj.theWin.document.body.innerHTML + "<pre id='data'>");
	var i = 0;
	while (obj.a[i])
	{	
		obj.theWin.document.write(obj.a[i]+"\n");
		i++;
	}
	obj.theWin.document.write("\n</pre>");	
	obj.theWin.document.close();
}



$(function() {
 // Handler for .ready() called.
 Osiris.init();
});