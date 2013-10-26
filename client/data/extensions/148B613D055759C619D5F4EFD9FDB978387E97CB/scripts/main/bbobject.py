'''
page.onLoad = function()
{
	var url = "javascript:history.back()";
	
	var name = page.request.getUrlParam("name");
	var value = page.request.getUrlParam("value");
	
	var current = "";
	page.showMessage(name);
	page.showMessage(value);
	if(name == "type")
	{
		current = options["anonymity.object_trusted_types"];
		if(current != "")
			current += "|";
		current += value;
		page.showMessage("Option 'anonymity.object_trusted_types':" + current);
	}
	else if(name == "domain")
	{
		current = options["anonymity.object_trusted_domains"];
		if(current != "")
			current += "|";
		current += value;
		page.showMessage("Option 'anonymity.object_trusted_domains':" + current);
	}
	
	var document = new xml.Document();
	var root = document.create("page");		
	root.attributes["url"] = url;
	root.attributes["confirm"] = true;
	root.attributes["name"] = name;
	root.attributes["value"] = value;
	
	var template = new html.XSLControl();
	template.stylesheet = page.loadStylesheet(page.skin.getResourcePath("templates/bbobject.xsl"));
	template.document = document;
	page.getArea(ide.content).controls.add(template);
}
'''