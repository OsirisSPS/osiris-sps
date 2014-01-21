<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl" />
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="/page">
    <div id="skinEditor" title="Skin Editor">
      <xsl:call-template name="editing"/>      
    </div>

    <script type="text/javascript">
      $(function() {      	
        Osiris.SkinEditor.init();
      });
    </script>


  </xsl:template>

  <xsl:template name="editing">
    <div class="os_skinEditor_toolbar">
			<div style="float:left;padding:5px;">
				Preset:
				<input size="20" id="skinEditor_preset" type="text" value=""/>
			</div>
			<div style="float:right">
				<a href="javascript:Osiris.SkinEditor.resetToPage();" class="os_button_img" data-os-tooltip="Return to default values">
					<img src="{system:resource-url('images/icons/16x16/reload.png')}"/>
				</a>
        <a href="javascript:Osiris.SkinEditor.load();" class="os_button_img" data-os-tooltip="Load a preset">
          <img src="{system:resource-url('images/icons/16x16/load.png')}"/>
        </a>        
        <a href="javascript:Osiris.SkinEditor.save();" class="os_button_img" data-os-tooltip="Save the current preset">
          <img src="{system:resource-url('images/icons/16x16/save.png')}"/>
        </a>
        <a href="javascript:Osiris.SkinEditor.remove();" class="os_button_img" data-os-tooltip="Remove the current preset">
          <img src="{system:resource-url('images/icons/16x16/remove.png')}"/>
        </a>
        <a href="javascript:Osiris.SkinEditor.showLessConfig();" class="os_button_img" data-os-tooltip="Get Less Code. Useful if you are creating a skin extension">
          <img src="{system:resource-url('images/icons/16x16/code.png')}"/>
        </a>
				<a href="javascript:Osiris.SkinEditor.import();" class="os_button_img" data-os-tooltip="Import a file">
					<img src="{system:resource-url('images/icons/16x16/import.png')}"/>
				</a>
				<a id="file_export" href="" class="os_button_img" data-os-tooltip="Export to file">
					<img src="{system:resource-url('images/icons/16x16/export.png')}"/>
				</a>
      </div>
			<input style="display:none" type="file" id="file_import" name="files[]" multiple="" />
			
			<div style="clear:both;"></div>
    </div>

    <div class="os_skinEditor_props">
      <table class="os_table_properties">      	
        <tr>
          <td style="width:40%">
            <xsl:value-of select="'Font'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-font-size" data-os-proptype="text"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	Size:
            	<input class="os_skinEditor_itemControl" type="text" size="10"/>
            </span>
          </td>
        </tr>        
        <tr>
          <td>
            <xsl:value-of select="'Page'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>          	
          	<span class="os_skinEditor_item" data-os-propname="os-skin-page-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>            
            <span class="os_skinEditor_item" data-os-propname="os-skin-page-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>	
            <span class="os_skinEditor_item" data-os-propname="os-skin-page-type" data-os-proptype="type"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	Type:
            	<select class="os_skinEditor_itemControl">
              	<option value="none">None</option>
              	<option value="shadow1">Border shadow</option>
            	</select>
            </span>
          </td>
        </tr>        
        <tr>
          <td>
            <xsl:value-of select="'Dialog'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-dialog-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-dialog-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-overlay-type" data-os-proptype="type"><input type="checkbox" class="os_skinEditor_itemEnable" />
	            Overlay:
	            <select class="os_skinEditor_itemControl">
	              <option value="none">None</option>
	              <option value="style1">Diagonal Dark</option>
	              <option value="style2">Light</option>
	              <option value="style3">Dark</option>
	            </select>
            </span>
          </td>
        </tr>        
        <tr>
          <td>          	
            <xsl:value-of select="'Page size'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-page-size" data-os-proptype="text"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	<input class="os_skinEditor_itemControl" type="text" size="10"/>
            </span>
          </td>
        </tr>        
        <tr>
        	<td>
            <xsl:value-of select="'Outside'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-outside-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	Color:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-outside-type" data-os-proptype="type"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	Type:
            	<select class="os_skinEditor_itemControl">
	              <option value="solid">None</option>
  	            <option value="gradient1">Gradient1</option>
    	        </select>
    	      </span>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="'Header'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-header-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-header-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="'Content'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-content-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-content-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
          </td>
        </tr>        
        <tr>
          <td>
            <xsl:value-of select="'Elements'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-elements-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-elements-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="'Current (elements)'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-current-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-current-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="'Item'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-item-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-item-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="'Action'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
            <span class="os_skinEditor_item" data-os-propname="os-skin-action-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-action-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="'Controls'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-controls-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-controls-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
          </td>
        </tr>        
        <tr>
          <td>
            <xsl:value-of select="'Buttons'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-buttons-type" data-os-proptype="type"><input type="checkbox" class="os_skinEditor_itemEnable" />
	            Type:
	            <select class="os_skinEditor_itemControl">
	              <option value="standard">Standard</option>
	              <option value="shiny">Shiny</option>              
	            </select>
	          </span>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="'Input'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-input-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-input-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
						<span class="os_skinEditor_item" data-os-propname="os-skin-input-border-fg" data-os-proptype="color">
							<input type="checkbox" class="os_skinEditor_itemEnable" />
							Border:
							<input class="os_skinEditor_itemControl" type="text"/>
						</span>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="'Block Big'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-block-big-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-block-big-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
						<br/>Body, 
						<span class="os_skinEditor_item" data-os-propname="os-skin-block-big-body-bg" data-os-proptype="color">
							<input type="checkbox" class="os_skinEditor_itemEnable" />
							BG:
							<input class="os_skinEditor_itemControl" type="text"/>
						</span>
						<span class="os_skinEditor_item" data-os-propname="os-skin-block-big-body-fg" data-os-proptype="color">
							<input type="checkbox" class="os_skinEditor_itemEnable" />
							FG:
							<input class="os_skinEditor_itemControl" type="text"/>
						</span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-block-big-type" data-os-proptype="type"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	Type:
	            <select class="os_skinEditor_itemControl">
	              <option value="plain">Plain</option>
	              <option value="style1">Style 1</option>
	              <option value="style2">Style 2</option>
	              <option value="heading">Heading</option>
	            </select>
	          </span>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="'Block Small'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
            <span class="os_skinEditor_item" data-os-propname="os-skin-block-small-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-block-small-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
						<br/>Body,
						<span class="os_skinEditor_item" data-os-propname="os-skin-block-small-body-bg" data-os-proptype="color">
							<input type="checkbox" class="os_skinEditor_itemEnable" />
							BG:
							<input class="os_skinEditor_itemControl" type="text"/>
						</span>
						<span class="os_skinEditor_item" data-os-propname="os-skin-block-small-body-fg" data-os-proptype="color">
							<input type="checkbox" class="os_skinEditor_itemEnable" />
							FG:
							<input class="os_skinEditor_itemControl" type="text"/>
						</span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-block-small-type" data-os-proptype="type"><input type="checkbox" class="os_skinEditor_itemEnable" />
	            Type:
	            <select class="os_skinEditor_itemControl">
	              <option value="plain">Plain</option>
	              <option value="style1">Style 1</option>
	              <option value="style2">Style 2</option>
	              <option value="heading">Heading</option>
	            </select>
	          </span>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="'Pathway'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-pathway-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
           	</span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-pathway-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="'Help'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-help-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-help-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="'System Bar'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-systembar-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-systembar-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="'Highlight'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-highlight-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
          </td>
        </tr>         
        <tr>
          <td>
            <xsl:value-of select="'Title'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-title-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	<input class="os_skinEditor_itemControl" type="text"/>            	
            </span>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="'Subtitle'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-subtitle-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="'Label'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-label-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="'Link'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-link-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="'Quote'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>          	
            1'Level, 
            <span class="os_skinEditor_item" data-os-propname="os-skin-quote1-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-quote1-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <br/>
            2'Level, 
            <span class="os_skinEditor_item" data-os-propname="os-skin-quote2-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-quote2-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <br/>
            N'Level, 
            <span class="os_skinEditor_item" data-os-propname="os-skin-quoten-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-quoten-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="'Code'"/>
            <xsl:text> :</xsl:text>
          </td>
          <td>
          	<span class="os_skinEditor_item" data-os-propname="os-skin-code-bg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	BG:
            	<input class="os_skinEditor_itemControl" type="text"/>            	
            </span>
            <span class="os_skinEditor_item" data-os-propname="os-skin-code-fg" data-os-proptype="color"><input type="checkbox" class="os_skinEditor_itemEnable" />
            	FG:
            	<input class="os_skinEditor_itemControl" type="text"/>
            </span>
          </td>
        </tr>
				<tr>
					<td>
						<xsl:value-of select="'Info'"/>
						<xsl:text> :</xsl:text>
					</td>
					<td>
						<span class="os_skinEditor_item" data-os-propname="os-skin-info-bg" data-os-proptype="color">
							<input type="checkbox" class="os_skinEditor_itemEnable" />
							BG:
							<input class="os_skinEditor_itemControl" type="text"/>
						</span>
						<span class="os_skinEditor_item" data-os-propname="os-skin-info-fg" data-os-proptype="color">
							<input type="checkbox" class="os_skinEditor_itemEnable" />
							FG:
							<input class="os_skinEditor_itemControl" type="text"/>
						</span>
					</td>
				</tr>
				<tr>
					<td>
						<xsl:value-of select="'Warning'"/>
						<xsl:text> :</xsl:text>
					</td>
					<td>
						<span class="os_skinEditor_item" data-os-propname="os-skin-warning-bg" data-os-proptype="color">
							<input type="checkbox" class="os_skinEditor_itemEnable" />
							BG:
							<input class="os_skinEditor_itemControl" type="text"/>
						</span>
						<span class="os_skinEditor_item" data-os-propname="os-skin-warning-fg" data-os-proptype="color">
							<input type="checkbox" class="os_skinEditor_itemEnable" />
							FG:
							<input class="os_skinEditor_itemControl" type="text"/>
						</span>
					</td>
				</tr>
				<tr>
					<td>
						<xsl:value-of select="'Error'"/>
						<xsl:text> :</xsl:text>
					</td>
					<td>
						<span class="os_skinEditor_item" data-os-propname="os-skin-error-bg" data-os-proptype="color">
							<input type="checkbox" class="os_skinEditor_itemEnable" />
							BG:
							<input class="os_skinEditor_itemControl" type="text"/>
						</span>
						<span class="os_skinEditor_item" data-os-propname="os-skin-error-fg" data-os-proptype="color">
							<input type="checkbox" class="os_skinEditor_itemEnable" />
							FG:
							<input class="os_skinEditor_itemControl" type="text"/>
						</span>
					</td>
				</tr>
				<tr>
					<td>
						<xsl:value-of select="'Hint'"/>
						<xsl:text> :</xsl:text>
					</td>
					<td>
						<span class="os_skinEditor_item" data-os-propname="os-skin-hint-bg" data-os-proptype="color">
							<input type="checkbox" class="os_skinEditor_itemEnable" />
							BG:
							<input class="os_skinEditor_itemControl" type="text"/>
						</span>
						<span class="os_skinEditor_item" data-os-propname="os-skin-hint-fg" data-os-proptype="color">
							<input type="checkbox" class="os_skinEditor_itemEnable" />
							FG:
							<input class="os_skinEditor_itemControl" type="text"/>
						</span>
					</td>
				</tr>
				<tr>	
					<td>
						<xsl:value-of select="'Note'"/>
						<xsl:text> :</xsl:text>
					</td>
					<td>
						<span class="os_skinEditor_item" data-os-propname="os-skin-note-bg" data-os-proptype="color">
							<input type="checkbox" class="os_skinEditor_itemEnable" />
							BG:
							<input class="os_skinEditor_itemControl" type="text"/>
						</span>
						<span class="os_skinEditor_item" data-os-propname="os-skin-note-fg" data-os-proptype="color">
							<input type="checkbox" class="os_skinEditor_itemEnable" />
							FG:
							<input class="os_skinEditor_itemControl" type="text"/>
						</span>
					</td>
				</tr>
        
        
      </table>
    </div>		
  </xsl:template>

  
</xsl:stylesheet>