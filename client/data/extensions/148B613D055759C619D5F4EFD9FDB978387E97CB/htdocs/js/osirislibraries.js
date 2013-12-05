/*
Osiris Third Party Libraries Management.
*/

var OsirisLibraries =
{
    sh: null,
    
    
    /*    
    loadScript: function(url, options) 
    {

		  // allow user to set any option except for dataType, cache, and url
		  options = $.extend(options || {}, {
		    dataType: "script",
		    cache: true,
		    url: url
		  });
		
		  // Use $.ajax() since it is more flexible than $.getScript
		  // Return the jqXHR object so we can chain callbacks
		  return jQuery.ajax(options);
		},
		*/
    
    // This function are called with "null" the first time for global document initialization, and after for each elements with attributes,
    // also for every ajax-loaded blocks.
    // We don't use jQuery selector because i think that maybe more cpu-intensive with jQuery (scroll dom for every selectors and not only one).    
    init: function (src) 
    {
        if (src == null) 
        {
            // Start Global initialization
            //$.ajaxSetup({async: false});
            $.ajaxSetup({async: false, cache: true});
            
            // Language
            $.getScript(Osiris.adjustStaticUrl("/htdocs/js/lang.js"));
            
            $.getScript(Osiris.adjustStaticUrl("/htdocs/js/libraries/ripple.js"));

            /* ------------------------------------------------------------
            jQuery - passStrengthener
            ------------------------------------------------------------ */
            //$.getScript(Osiris.adjustStaticUrl("/htdocs/js/jquery/jquery.passStrengthener.js"));
            $.getScript(Osiris.adjustStaticUrl("/htdocs/js/jquery/jquery.passStrengthener.js"));
            
            /* ------------------------------------------------------------
            jQuery - Lightbox
            ------------------------------------------------------------ */

            $.getScript(Osiris.adjustStaticUrl("/htdocs/js/jquery/jquery.lightbox.js"));
            /*
            $.getScript(Osiris.adjustStaticUrl("/htdocs/js/jquery/jquery.fancybox.js"));
            $.getScript(Osiris.adjustStaticUrl("/htdocs/js/jquery/jquery.fancybox-buttons.js"));
            $.getScript(Osiris.adjustStaticUrl("/htdocs/js/jquery/jquery.fancybox-media.js"));
            $.getScript(Osiris.adjustStaticUrl("/htdocs/js/jquery/jquery.fancybox-thumbs.js"));
            */

            /* ------------------------------------------------------------
            jQuery - Autoresize
            ------------------------------------------------------------ */
                       
            $.getScript(Osiris.adjustStaticUrl("/htdocs/js/jquery/jquery.autoresize.js"));
			
            /* ------------------------------------------------------------
            jQuery - DataTables
            ------------------------------------------------------------ */

            // $.getScript(Osiris.adjustStaticUrl("/htdocs/js/jquery/jquery.dataTables.js"));            
            
            /* ------------------------------------------------------------
            jQuery - ColorPicker
            ------------------------------------------------------------ */
            
            $.getScript(Osiris.adjustStaticUrl("/htdocs/js/jquery/jquery.colorpicker.js"));
            $.getScript(Osiris.adjustStaticUrl("/htdocs/js/jquery/i18n/jquery.ui.colorpicker-en.js"));
            

            /* ------------------------------------------------------------
            SyntaxHighlighter
            ------------------------------------------------------------ */

            $.getScript(Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shCore.js"));
            $.getScript(Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shAutoloader.js"));
                
            var brushes = Array();
            brushes.push("applescript            " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushAppleScript.js"));
            brushes.push("actionscript3 as3      " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushAS3.js"));
            brushes.push("bash shell             " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushBash.js"));
            brushes.push("coldfusion cf          " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushColdFusion.js"));
            brushes.push("cpp c                  " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushCpp.js"));
            brushes.push("c# c-sharp csharp      " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushCSharp.js"));
            brushes.push("css                    " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushCss.js"));
            brushes.push("delphi pascal          " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushDelphi.js"));
            brushes.push("diff patch pas         " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushDiff.js"));
            brushes.push("erl erlang             " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushErlang.js"));
            brushes.push("groovy                 " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushGroovy.js"));
            brushes.push("java                   " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushJava.js"));
            brushes.push("jfx javafx             " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushJavaFX.js"));
            brushes.push("js jscript javascript  " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushJScript.js"));
            brushes.push("perl pl                " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushPerl.js"));
            brushes.push("php                    " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushPhp.js"));
            brushes.push("text plain             " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushPlain.js"));
            brushes.push("py python              " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushPython.js"));
            brushes.push("ruby rails ror rb      " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushRuby.js"));
            brushes.push("sass scss              " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushSass.js"));
            brushes.push("scala                  " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushScala.js"));
            brushes.push("sql                    " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushSql.js"));
            brushes.push("vb vbnet               " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushVb.js"));
            brushes.push("xml xhtml xslt html    " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushXml.js"));
            brushes.push("oml osiris bbcode bbc  " + Osiris.adjustStaticUrl("/htdocs/js/syntaxhighlighter/shBrushOml.js"));

            SyntaxHighlighter.autoloader.apply(null, brushes);
            SyntaxHighlighter.defaults['auto-links'] = false;
            SyntaxHighlighter.defaults['toolbar'] = false;
            SyntaxHighlighter.all();
                
            // End Global initialization
            $.ajaxSetup({async: true});
            
        }
        else 
        {
            // Note Clodo 10/04/2011: We tryed many hours about pass "data-os" attributes, AND/OR tell to library to initialize only the current "src".
            // I failed.
            // Actually [code] tags are not syntax-highlighted if come from an ajax call (filling div)
            /*
            if( (src.tagName) && (src.tagName.toLowerCase() == "pre") && (src.attributes) && (src.attributes["data-os-syntaxhighlighter"]) )
            {		    		        
            var params = src.attributes["data-os-syntaxhighlighter"].nodeValue;
		        
            //SyntaxHighlighter.all();    
            //SyntaxHighlighter.highlight();    
            }
            */

            //$('input[data-os-otype*="bool"]',src)

            if (src.attributes["data-os-otype"]) {
                var otype = src.attributes["data-os-otype"].nodeValue;

                /* ------------------------------------------------------------
                jQuery - passStrengthener
                ------------------------------------------------------------ */

                if (otype == "password") {
                    var loginName = src.attributes["data-os-login"].nodeValue;		    
                    $(src).passStrengthener({
						userid:	"#" + loginName,					
					});                    
                }

                /* ------------------------------------------------------------
                jQuery - DataTables
                ------------------------------------------------------------ */

                if (otype == "table") {
                    $(src).dataTable({
                        "bJQueryUI": true,
                        "bPaginate": false,
                        "bLengthChange": true,
                        "bFilter": true,
                        "bSort": true,
                        "bInfo": false,
                        "bAutoWidth": false,
                        "iDisplayLength": 100
                    });
                }
                
                /* ------------------------------------------------------------
                jQuery - ColorPicker
                ------------------------------------------------------------ */
                
                if (otype == "color") {
                	$(src).css('width', "0px");
									$(src).css('opacity', "0");
						
									$(src).colorpicker({
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
                }                
            }            
        }
    }
}



