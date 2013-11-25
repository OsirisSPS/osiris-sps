/*
Osiris OMLEditor javascript library.
*/

Osiris.OmlEditor =
{
	init: function (src) {
		var area = $(src).find("textarea")[0];

		$(area).autosize();		
	}
}

function omlEditorTag(areaId, tagOpen, tagClose) 
{
    area = osGetById(areaId);
    
    omlEditorSurroundText(area, tagOpen, tagClose);
}

function omlEditorEmoticon(areaId, code)
{
    area = osGetById(areaId);
    
    omlEditorInsertText(area, " "+osHtmlDecode(code)+" ");
}

function omlEditorSurroundText(textarea, text1, text2)
{	
	if (Osiris.is_ie)
	{
		textarea.focus(); 
		var curSelect = document.selection.createRange(); 
		curSelect.text = text1 + curSelect.text + text2; 
	}
	// Can a text range be created?
	else if (typeof(textarea.caretPos) != "undefined" && textarea.createTextRange)
	{
		var caretPos = textarea.caretPos, temp_length = caretPos.text.length;

		caretPos.text = caretPos.text.charAt(caretPos.text.length - 1) == ' ' ? text1 + caretPos.text + text2 + ' ' : text1 + caretPos.text + text2;

		if (temp_length == 0)
		{
			caretPos.moveStart("character", -text2.length);
			caretPos.moveEnd("character", -text2.length);
			caretPos.select();
		}
		else
			textarea.focus(caretPos);
	}
	// Mozilla text range wrap.
	else if (typeof(textarea.selectionStart) != "undefined")
	{
		var begin = textarea.value.substr(0, textarea.selectionStart);
		var selection = textarea.value.substr(textarea.selectionStart, textarea.selectionEnd - textarea.selectionStart);
		var end = textarea.value.substr(textarea.selectionEnd);
		var newCursorPos = textarea.selectionStart;
		var scrollPos = textarea.scrollTop;

		textarea.value = begin + text1 + selection + text2 + end;

		if (textarea.setSelectionRange)
		{
			if (selection.length == 0)
				textarea.setSelectionRange(newCursorPos + text1.length, newCursorPos + text1.length);
			else
				textarea.setSelectionRange(newCursorPos, newCursorPos + text1.length + selection.length + text2.length);
			textarea.focus();
		}
		textarea.scrollTop = scrollPos;
	}
	// Just put them on the end, then.
	else
	{
		textarea.value += text1 + text2;
		textarea.focus(textarea.value.length - 1);		
	}
}

// Replaces the currently selected text with the passed text.
function omlEditorInsertText(textarea, text)
{
  if (Osiris.is_ie)
	{
	    textarea.focus(); 
        var curSelect = document.selection.createRange(); 
        curSelect.text = text; 
	}	
	// Attempt to create a text range (IE).
	else if (typeof(textarea.caretPos) != "undefined" && textarea.createTextRange)
	{
		var caretPos = textarea.caretPos;

		caretPos.text = caretPos.text.charAt(caretPos.text.length - 1) == ' ' ? text + ' ' : text;
		caretPos.select();
	}
	// Mozilla text range replace.
	else if (typeof(textarea.selectionStart) != "undefined")
	{
		var begin = textarea.value.substr(0, textarea.selectionStart);
		var end = textarea.value.substr(textarea.selectionEnd);
		var scrollPos = textarea.scrollTop;

		textarea.value = begin + text + end;

		if (textarea.setSelectionRange)
		{
			textarea.focus();
			textarea.setSelectionRange(begin.length + text.length, begin.length + text.length);
		}
		textarea.scrollTop = scrollPos;
	}
	// Just put it on the end.
	else
	{
		textarea.value += text;
		textarea.focus(textarea.value.length - 1);
	}
}

function omlEditorScript_wrap(areaId)
{
	var textarea = osGetById(areaId);
    
	if(textarea.wrap == 'off')
	{
		textarea.wrap = 'soft'; // For IE/Opera
		textarea.setAttribute('wrap', 'soft'); // For Firefox
	}
	else
	{
		textarea.wrap = 'off'; // For IE/Opera
		textarea.setAttribute('wrap', 'off'); // For Firefox
	}	
	
	// Workaround for display bug in Firefox & IE
	var parNod = textarea.parentNode, nxtSib = textarea.nextSibling;
	parNod.removeChild(textarea); parNod.insertBefore(textarea, nxtSib);	
}

/*
function omlEditorScript_height_inc(areaId)
{
	var area = osGetById(areaId);
	area.rows+=3;
}

function omlEditorScript_height_dec(areaId)
{
	var area = osGetById(areaId);
	if(area.rows>3)
		area.rows-=3;
}
*/

function omlEditorScript_emoticons(areaId)
{
	var emoDiv = osGetById(areaId + "_emoticons");
	if(emoDiv.style.display != 'block')
	{
		emoDiv.style.display = 'block';
	}
	else
	{
		emoDiv.style.display = 'none';
	}
	
}
