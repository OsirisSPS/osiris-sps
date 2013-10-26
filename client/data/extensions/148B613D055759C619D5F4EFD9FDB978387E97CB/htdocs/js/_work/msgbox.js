var osMessages = new Array(0);
		
function osMessage(level, text, title)
{
	count = osMessages.length;
	osMessages[count] = new Object();
	osMessages[count].level = level;
	osMessages[count].text = text;
	osMessages[count].title = title;
}

function osMessagesInit()
{
	osMessageHide();
	osMessageShow(0);
}		

function osMessageShow(index)
{
	var body = osGetById("body");
	var msgbox_back = osGetById("os_msgbox_back");
	var msgbox = osGetById("os_msgbox");
	
	msgbox.className = "os_msgbox_"+osMessages[index].level;
	osGetById("os_msgbox_title").innerHTML=osMessages[index].title;
	osGetById("os_msgbox_message").innerHTML=osMessages[index].text;
	if(index==osMessages.length-1)
	{
		osGetById("os_msgbox_ok").onclick = function () { osMessageHide(); }
	}
	else
	{
		osGetById("os_msgbox_ok").onclick = function () { osMessageShow(index+1); }
	}
	
	msgbox_back.style.visibility = "visible";		
	msgbox_back.style.position="absolute";
	msgbox_back.style.left=0;
	msgbox_back.style.top=0;	
	msgbox_back.style.width=body.offsetWidth + "px";
	msgbox_back.style.height=body.offsetHeight + "px";
	
	msgbox.style.display = "block";
	msgbox.style.position="absolute";
	msgbox.style.left=msgbox_back.offsetWidth/2 - msgbox.offsetWidth/2 + "px";
	msgbox.style.top=msgbox_back.offsetHeight/2 - msgbox.offsetHeight/2 + "px";
}		

function osMessageHide()
{
	osGetById("os_msgbox").style.display = "none";
	osGetById("os_msgbox_back").style.visibility = "hidden";	
}