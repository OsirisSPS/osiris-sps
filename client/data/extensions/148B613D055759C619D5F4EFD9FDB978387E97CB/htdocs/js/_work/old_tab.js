function osTab_Create(srcId,mode,cssForTable,cssForLink,cssForLinkHover)
{
	objTab=osGetById(srcId);		
		
	tab=osCreate("TABLE");
	tabBody=osCreate("TBODY");
	tab.appendChild(tabBody);
	osClassNameSet(tab,cssForTable);
	
	colLinks=osCreate("TD");
	colPages=osCreate("TD");
	colPages.style.width="100%";
	colPages.style.height="100%";
	if ( (mode=="top") || (mode=="bottom") )
	{
		rowLinks=osCreate("TR");
		rowLinks.appendChild(colLinks);
		rowPages=osCreate("TR");
		rowPages.appendChild(colPages);
		if (mode=="top")
		{
			tabBody.appendChild(rowLinks);
			tabBody.appendChild(rowPages);
		}
		else
		{
			tabBody.appendChild(rowPages);
			tabBody.appendChild(rowLinks);			
		}
	}
	else if ( (mode=="left") || (mode=="right") )
	{
		row=osCreate("TR");
		tabBody.appendChild(row);
		if (mode=="left")
		{
			row.appendChild(colLinks);
			row.appendChild(colPages);
		}
		else
		{			
			row.appendChild(colPages);
			row.appendChild(colLinks);
		}
	}
	
	nPages=0;
	
	for(c=0;c<objTab.childNodes.length;c++)
	{
		objChild=objTab.childNodes[c];
		if (objChild.tagName=="DIV")
		{
			pageName=objChild.attributes["pageTitle"].nodeValue;
			if (pageName!=null)
			{
				// Page found!
				lnkPage=osCreate("A");
				lnkPage.cssForLink=cssForLink;
				lnkPage.cssForLinkHover=cssForLinkHover;
				
				osClassNameSet(lnkPage,cssForLink);
				lnkPage.onmouseover=function () { osClassNameSet(this,this.cssForLinkHover); }
				lnkPage.onmouseout=function () { osClassNameSet(this,this.cssForLink); }
				
				lnkPage.objTab=objTab;
				lnkPage.page=objChild;
				if (nPages>0)
				{
					lnkPage.page.style.display="none";
				}
				else
				{
					objTab.currentPage=objChild;
				}
				lnkPage.onclick=function (e) { osTab_ShowPage(e,this); }
				lnkPage.appendChild(osCreateText(pageName));
				colLinks.appendChild(lnkPage);
				colPages.appendChild(objChild);
				c--;
				nPages++;
			}
		}
	}	
	
	objTab.appendChild(tab);
}

function osTab_ShowPage(e,lnkButton)
{
	if (lnkButton.objTab.currentPage) lnkButton.objTab.currentPage.style.display="none";
	lnkButton.objTab.currentPage=lnkButton.page;
	lnkButton.page.style.display="block";
}


