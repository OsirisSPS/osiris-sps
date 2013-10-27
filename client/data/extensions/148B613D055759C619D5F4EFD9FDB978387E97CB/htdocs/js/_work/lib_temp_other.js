
// ************************************************************************
// Controls Events
// ************************************************************************

function iCreateDivScript(src)
{
	t=src.attributes['type'].nodeValue;
	//t=iPropGet(src,"type",null);
	if (t==null) return null;
	
	var nProp=src.attributes.length;
	for(var p=0;p<nProp;p++)
	{
		var PropName=src.attributes[p].nodeName;
		
		if (PropName.substr(0,2)=='p_')
		{
			inp=iCreate("INPUT");
			inp.type='hidden';
			inp.name=src.id+"_"+PropName;
			inp.value=src.attributes[p].nodeValue;
			iGetForm().appendChild(inp);		
		}	
	}
	
	
	eval("i"+t+"_Create(src)");	
}

function osEventPostCtl(eventId, eventArgument) 
{
	var f=osGetForm();
	f.osPostCtlId.value = eventId.split("$").join(":");
	f.osPostCtlArg.value = eventArgument;
	f.submit();
}

/*
La iSetEventClick si salva all'interno dell'elemento la stringa di cui in un secondo momento dovrà fare l'eval.
Sotto è riportato un esempio di un'eval con un riferimento 'temporaneo':

function Test(x,y)
{
	d=iCreate("DIV");
	d.innerHTML="Prova"+x;
	d.style.border="2px solid red";
	d.myscript=y;
	d.onclick=function() { alert(x); }
	d.oncontextmenu=function() { eval(d.myscript) }
	iGetById('Form1').appendChild(d);
}
Test("a","alert('a')");
Test("b","alert('b')");
Test("c","alert('c')");
*/
function iEventSetClick(SrcControl,SrcElement)
{
	if ( (SrcControl) && (SrcElement) )
	{
		iEventClick=iPropGet(SrcControl,"EventClick",null);
		//iEventClick=SrcControl.iEventClick;
		if (iEventClick)
		{
			if (iEventClick=='(server)') iEventClick="iEventPostCtl('"+SrcControl.id+"','Click');";
			SrcElement.ClickScript=iEventClick;
			SrcElement.onclick=function() { eval(this.ClickScript); }
		}
	}	
}	


/**************************************************************************************************
 Other Functions
**************************************************************************************************/

// Sarebbe fico creare una funzione js che enumera tutti gli elementi, e di ognuno setta alcune funzioni tipiche,
// ad esempio che la pressione di F1 (onhelp) apre un popup di help...
function osSetCommonFunction(src)
{
	// Help wrapper:
	src.onhelp=function () { alert('Help!');return false; }		
}

// ************************************************************************
// Xml
// ************************************************************************

// TODO: if onloadfunc=null, load sync
function iXml_Open(src,filename,onloadfunc)
{	
	// Mozilla	
	if (document.implementation && document.implementation.createDocument)
	{
		src.xmlDoc = document.implementation.createDocument("", "", null);
		src.xmlDoc.onload = onloadfunc;
	}	
	// IE	
	else if (window.ActiveXObject)
	{
		src.xmlDoc = new ActiveXObject("Microsoft.XMLDOM");
		src.xmlDoc.onreadystatechange = function () 
		{
			if (src.xmlDoc.readyState == 4) 
			{										
				onloadfunc(src);
			}
		};
 	}
	else
	{
		alert('Your browser can\'t handle this script');
		return;
	}
	src.xmlDoc.load(filename);
}

// ************************************************************************
// Object positioning
// ************************************************************************

// Muove l'elemento specificato sotto il cursore.
function iMoveOnCursor(evt,src)
{	
	evt=iGetEvent(evt);
  var coords = { x: 0, y: 0};
  if (typeof window.pageXOffset != 'undefined') 
  {
    coords.x = window.pageXOffset + evt.clientX;
    coords.y = window.pageYOffset + evt.clientY;
  }
  else if (document.compatMode && document.compatMode != 'BackCompat') 
  {
    coords.x = document.documentElement.scrollLeft + evt.clientX;
    coords.y = document.documentElement.scrollTop + evt.clientY;
  }
  else 
  {
    coords.x = document.body.scrollLeft + evt.clientX;
    coords.y = document.body.scrollTop + evt.clientY;
  }
  
  src.style.position="absolute";
  /*
  src.style.left=coords.x;
  src.style.top=coords.y;
  
  if (src.parentElement) 
  {
	ix=iElement_GetAbsoluteX(src.parentElement);
	iy=iElement_GetAbsoluteY(src.parentElement);
	src.style.left=coords.x-ix;
	src.style.top=coords.y-iy;
  }
  */
  iElement_SetAbsoluteLeft(src,coords.x);
  iElement_SetAbsoluteTop(src,coords.y);
}

// Setta la posizione assoluta dell'elemento.
// Da notare che è diverso da una semplice src.style.left=val, dato che in un elemento assoluto la 'left' è
// comunque un valore relativo al padre, se pure lui è assoluto...
function iElement_SetAbsoluteLeft(src,val)
{
	src.style.position="absolute";
	src.style.left=val-iElement_GetAbsoluteLeft(src.parentElement)+"px";
}

function iElement_SetAbsoluteTop(src,val)
{
	src.style.position="absolute";
	src.style.top=val-iElement_GetAbsoluteTop(src.parentElement)+"px";
}

function iElement_GetAbsoluteLeft(src) 
{	
	o=src;
	oLeft=0;
	while(o!=null)
	{
		//if (o.style.position=='absolute') oLeft=oLeft+o.offsetLeft;
		oLeft=oLeft+o.offsetLeft;
		o=o.parentElement;
	}	
	return oLeft;
}

function iElement_GetAbsoluteTop(src) 
{	
	o=src;
	oTop=0;
	while(o!=null)
	{
		//if (o.style.position=='absolute') oTop=oTop+o.offsetTop;
		oTop=oTop+o.offsetTop;
		o=o.parentElement;
	}	
	return oTop;
}

/*
function iElement_GetAbsoluteX(src) 
{
	// Get an object left position from the upper left viewport corner
	// Tested with relative and nested objects
	o = src;
	oLeft = o.offsetLeft;            // Get left position from the parent object
	while(o.offsetParent!=null) 
	{   // Parse the parent hierarchy up to the document element
		oParent = o.offsetParent;    // Get parent object reference
		oLeft += oParent.offsetLeft; // Add parent left position
		o = oParent;
	}
	// Return left postion
	return oLeft;
}

function iElement_GetAbsoluteY(src) 
{
	// Get an object top position from the upper left viewport corner
	// Tested with relative and nested objects
	o = src;
	oTop = o.offsetTop;            // Get top position from the parent object
	while(o.offsetParent!=null) 
	{ // Parse the parent hierarchy up to the document element
		oParent = o.offsetParent;  // Get parent object reference
		oTop += oParent.offsetTop; // Add parent top position
		o = oParent;
	}
	// Return top position
	return oTop;
}
*/

/*
// Restituisce la coordinata X assoluta rispetto alla pagina
function iElement_GetAbsoluteX(src)
{
	x=0;
	e=src;
	for(;e.offsetParent!=null;)
	{
		x+=e.offsetLeft;
		e=e.offsetParent;
	}
	return x;	
}
// Restituisce la coordinata Y assoluta rispetto alla pagina
function iElement_GetAbsoluteY(src)
{
	y=0;
	e=src;
	for(;e.offsetParent!=null;)
	{
		y+=e.offsetTop;
		e=e.offsetParent;
	}
	return y;	
}
*/

// Allinea srcmove a SrcRif sovrapponendo gli angoli AlignMove e AlignRif ( (0,1,2 (top), 3,4,5 (middle), 6,7,8 (bottom) )
function iElement_AbsoluteAlign(SrcMove,SrcRif,AlignMove,AlignRif)
{
	SrcMove.style.position="absolute";
		
	ddx=SrcRif.offsetWidth;
	ddy=SrcRif.offsetHeight;
	
	rx=iElement_GetAbsoluteLeft(SrcRif);
	ry=iElement_GetAbsoluteTop(SrcRif);
	if ( (AlignRif==1) || (AlignRif==4) || (AlignRif==7) ) rx=rx+ddx/2;
	if ( (AlignRif==2) || (AlignRif==5) || (AlignRif==8) ) rx=rx+ddx;
	if ( (AlignRif==3) || (AlignRif==4) || (AlignRif==5) ) ry=ry+ddy/2;
	if ( (AlignRif==6) || (AlignRif==7) || (AlignRif==8) ) ry=ry+ddy;	
	
	sdx=SrcMove.offsetWidth;
	sdy=SrcMove.offsetHeight;
	
	if ( (AlignMove==0) || (AlignMove==3) || (AlignMove==6) ) mx=rx;
	if ( (AlignMove==1) || (AlignMove==4) || (AlignMove==7) ) mx=rx-sdx/2;
	if ( (AlignMove==2) || (AlignMove==5) || (AlignMove==8) ) mx=rx-sdx;
	if ( (AlignMove==0) || (AlignMove==1) || (AlignMove==2) ) my=ry;
	if ( (AlignMove==3) || (AlignMove==4) || (AlignMove==5) ) my=ry-sdy/2;
	if ( (AlignMove==6) || (AlignMove==7) || (AlignMove==8) ) my=ry-sdy;
	
	iElement_SetAbsoluteLeft(SrcMove,mx);
	iElement_SetAbsoluteTop(SrcMove,my);
}

function iElement_FitDocument(src)
{
	src.style.left=0;
	src.style.top=0;
	
	doc_width=ie ? 0 : document.width+5;
	doc_height=ie ? 0 : document.height+5;
	
	win_width= document.body.clientWidth;
	win_height= document.body.clientHeight;		
	
	src.style.width=doc_width>win_width ? doc_width : win_width;
	src.style.height=doc_height>win_height ? doc_height : win_height;	
}

function iElement_CenterWindow(src)
{	
	var dsocleft=ie? document.body.scrollLeft : pageXOffset
	var dsoctop=ie? document.body.scrollTop : pageYOffset
	src.style.left=dsocleft+document.body.clientWidth/2-src.offsetWidth/2;
	src.style.top=dsoctop+document.body.clientHeight/2-src.offsetHeight/2;
}

// ************************************************************************
// Module Editing
// ************************************************************************

function iGetEvent(evt)
{
	if (moz) return evt;
	if (ie) return window.event;
	return null;
}

function iGetEventTarget(evt)
{
	if (moz) return evt.target;
	if (ie) return event.srcElement;
	return null;
}


