function onChangeCustom(key)
{
	//alert(key);
	//alert(document.getElementsByName("option_" + key)[0]);
	//alert(document.getElementsByName("custom_" + key)[0]);
	//alert(document.getElementsByName("custom_" + key)[0].selectedIndex);
	document.getElementsByName("option_" + key)[0].disabled = (document.getElementsByName("custom_" + key)[0].selectedIndex == 0);
}

function regExTest(r, s)
{ 
	if (r=='') 
		return; 
	var exp = new RegExp(r); 
	if(exp.test(s)) 
	{ 
		alert('Match.'); 
	} 
	else 
	{ 
		alert('Doesn\'t match.'); 
	}
}