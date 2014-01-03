function focusInstalled(id)
{
    osGetById("addons_tab").switchPage(2);
    
    Osiris.scrollTo(id + "_box");
    
    //$("#" + id + "_box").effect("highlight", {}, 3000);    
    $("#mytest").effect("highlight", {}, 3000);    
}

function extensionAction(title, message, confirmButton, cancelButton, action, id, token)
{
    
    osConfirm(title, message, confirmButton, cancelButton, "javascript:extensionActionConfirmed('" + action + "','" + id + "','" + token + "');");
    
    // Direct
    //extensionActionConfirmed(action, id);    
}
    
function extensionActionConfirmed(action, id, token)
{
    // Start Job
    ajaxUrl = "/main/addons?act=job&type=" + action + "&id=" + id + "&token=" + token;    
    Osiris.loadUrl(ajaxUrl, function(response)
    {
        // Update boxes
        ajaxUrl = "/main/addons?act=addon&id=" + id + "&actions=no";    
        osDynamicDiv(osGetById(id + "_recommended"), ajaxUrl + "&mode=remote");        
        osDynamicDiv(osGetById(id + "_catalog"), ajaxUrl + "&mode=remote");
        osDynamicDiv(osGetById(id + "_installed"), ajaxUrl + "&mode=local");
        osDynamicDiv(osGetById(id + "_upgradable"), ajaxUrl + "&mode=remote");            
    });
    
    
}

function extensionActionCompleted(action, id)
{	
	  ajaxUrl = "/main/addons?act=addon&id=" + id + "&actions=yes";
        
    osDynamicDiv(osGetById(id + "_recommended"), ajaxUrl + "&mode=remote");
    osDynamicDiv(osGetById(id + "_catalog"), ajaxUrl + "&mode=remote");
    
    if(action == "uninstall")
    {
        // Remove installed
        $(osGetById(id + "_installed")).remove();
        $(osGetById(id + "_upgradable")).remove();        
    }
    else if(action == "install")
    {
        // Add installed
        var div = osCreate("div");
        div.id = id + "_installed";
        osGetById("list_installed").appendChild(div);
        osDynamicDiv(osGetById(id + "_installed"), ajaxUrl + "&mode=local");
    }    
    else if(action == "upgrade")
    {
        osDynamicDiv(osGetById(id + "_installed"), ajaxUrl + "&mode=local");
        $(osGetById(id + "_upgradable")).remove();
    }
    else
    {
        osDynamicDiv(osGetById(id + "_installed"), ajaxUrl + "&mode=local");
        osDynamicDiv(osGetById(id + "_upgradable"), ajaxUrl + "&mode=remote");
    }
}

