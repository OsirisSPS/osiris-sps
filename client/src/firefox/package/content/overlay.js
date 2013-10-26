const OSIRIS_PROTOCOL_SCHEME = "osiris";
const OSIRIS_PROTOCOL_NAME = "Osiris Protocol";
const OSIRIS_PROTOCOL_CONTRACTID = "@mozilla.org/network/protocol;1?name=" + OSIRIS_PROTOCOL_SCHEME;
const OSIRIS_PROTOCOL_CID = Components.ID("2e398578-7540-4591-9083-44c8899df4c5");
							 
const nsISupports = Components.interfaces.nsISupports;
const nsIProtocolHandler = Components.interfaces.nsIProtocolHandler;

var osiris = 
{
	get interface()
	{
		if(this.initialized)
			return this.osirisInterface;
		
		this.initialized = true;
			
		var extensionManager = Components.classes["@mozilla.org/extensions/manager;1"].getService(Components.interfaces.nsIExtensionManager); 
		var root = extensionManager.getInstallLocation("osiris@kodeware.net").getItemLocation("osiris@kodeware.net").path;
		  
		var osirisComponent = Components.classes["@kodeware.net/osiris;1"];
		if(osirisComponent == null)
		{
			alert("Invalid Osiris component");				
			return null;
		}
				
		var osirisInstance = osirisComponent.createInstance();		
		if(osirisInstance == null)
		{
			alert("Invalid Osiris instance");
			return null;			
		}
			
		this.osirisInterface = osirisInstance.QueryInterface(Components.interfaces.IOsiris);
		if(this.osirisInterface == null)
		{
			alert("Invalid Osiris interface");
			return null;			
		}
		
		this.osirisInterface.init(root);
		
		return this.osirisInterface;
	}
}

function osirisProtocol() 
{

}

osirisProtocol.prototype = 
{
	QueryInterface : function(iid)
	{
		if(!iid.equals(nsIProtocolHandler) && !iid.equals(nsISupports))
			throw Components.results.NS_ERROR_NO_INTERFACE;
		
		return this;
	},

	scheme : OSIRIS_PROTOCOL_SCHEME,
	defaultPort : -1,
	protocolFlags : nsIProtocolHandler.URI_NOAUTH | nsIProtocolHandler.URI_LOADABLE_BY_ANYONE,
  
	allowPort : function(port, scheme)
	{
		return false;
	},
	
	newURI : function(spec, charset, baseURI)
	{
		return Components.classes["@mozilla.org/network/io-service;1"].getService(Components.interfaces.nsIIOService).newURI(osiris.interface.parse(spec), null, null);
	},

	newChannel : function(aURI)
	{	
		// Should be never called...
		return null;   
	},
}

var osirisProtocolFactory = 
{
	createInstance: function (outer, iid)
	{
		if(outer != null)
			throw Components.results.NS_ERROR_NO_AGGREGATION;

		if(!iid.equals(nsIProtocolHandler) && !iid.equals(nsISupports))
			throw Components.results.NS_ERROR_NO_INTERFACE;

		return new osirisProtocol();		
	}
}

var osirisGUI = 
{
	initGUI: function() 
	{
		this.prefs = Components.classes["@mozilla.org/preferences-service;1"].getService(Components.interfaces.nsIPrefService);
		this.prefs = this.prefs.getBranch("extensions.osiris.");
		this.strings = document.getElementById("osiris-strings");
				
		var first_launch = true;		
		try 
		{ 
			first_launch = this.prefs.getBoolPref("first_launch");			
		} 
		catch(e) 
		{		
		}
		
		if(first_launch) 
		{
			var toolbar = document.getElementById("nav-bar");
			var buttonCheck = document.getElementById("osiris-toolbar-button");
			if(!buttonCheck) 
			{
				var newSet = "";
				for(var i=0; i < toolbar.childNodes.length; i++)
				{
					if(toolbar.childNodes[i].id == "urlbar-container")
					{
						newSet += "osiris-toolbar-button,";
					}
					newSet += toolbar.childNodes[i].id + ",";
				}				

				toolbar.setAttribute("currentset", newSet);
				toolbar.currentSet = newSet;
				document.persist("nav-bar", "currentset");
				try 
				{ 
					BrowserToolboxCustomizeDone(true); 
				}
				catch(e) 
				{
				}
			}

			this.prefs.setBoolPref("first_launch", false);			
		}			
	},
	
	notify: function(timer) 
	{
		var osirisInterface = osiris.interface;
		if(osirisInterface == null)
		{
			alert("interface is null");
			this.timer.cancel();
			return;
		}
		
		if(osirisInterface.started)
		{	
			this.timer.cancel();
			this.toolbarButton.disabled = false;			
			this.toolbarButton.tooltipText = this.strings.getString("toolbar.button.tooltip.ready")
		}
	},	
	
	onLoad: function() 
	{	
		var wManager = Components.classes['@mozilla.org/appshell/window-mediator;1'].getService(Components.interfaces.nsIWindowMediator);
		var wIter = wManager.getEnumerator('navigator:browser');

		var counter = 0;
		while(wIter.hasMoreElements()) 
		{
			currentWindow = wIter.getNext();
			counter = counter + 1;
		}

		if(counter == 1) 
			Components.manager.QueryInterface(Components.interfaces.nsIComponentRegistrar).registerFactory(OSIRIS_PROTOCOL_CID, OSIRIS_PROTOCOL_NAME, OSIRIS_PROTOCOL_CONTRACTID, osirisProtocolFactory);			

		this.initGUI();	
		
		this.toolbarButton = document.getElementById("osiris-toolbar-button");
		if(this.toolbarButton)
		{
			this.toolbarButton.disabled = true;	
			this.toolbarButton.tooltipText = this.strings.getString("toolbar.button.tooltip.starting")
			this.timer = Components.classes["@mozilla.org/timer;1"].createInstance(Components.interfaces.nsITimer);
			this.timer.initWithCallback(this, 2000, Components.interfaces.nsITimer.TYPE_REPEATING_SLACK);
		}
		else
		{
			alert("Invalid Osiris toolbar button");
		}	
	},
		
	onToolbarButtonCommand: function(e) 
	{
		var osirisInterface = osiris.interface;
		if(osirisInterface == null)
		{
			alert("interface is null");
			return;
		}
	
		var uri = Components.classes["@mozilla.org/network/io-service;1"].getService(Components.interfaces.nsIIOService).newURI(osirisInterface.homepage, null, null);
		Application.activeWindow.open(uri).focus();		
	}
}

window.addEventListener("load", function(e) { osirisGUI.onLoad(e); }, false);
