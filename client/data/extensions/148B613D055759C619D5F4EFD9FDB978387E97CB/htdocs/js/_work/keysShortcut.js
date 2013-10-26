/**************************************************************************************************
 Funzioni javascript per gli shortcut da tastiera
 
 Esempio: // 13 = Enter
 osKeysShortcutRegister(13,"javascript:osEventPostCtl('page.search', 'onClick')");
  
**************************************************************************************************/

var mappings = new Object;
document.onkeypress = function(evt)
{
	var href = mappings[osGetEvent(evt).keyCode];
	if(href != null)
	{
		window.location = href;
		return false;
	}
}

function osKeysShortcutRegister(code, href)
{
	mappings[code] = href;
}


