<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:lang="http://www.osiris-sps.org/xsl/lang" xmlns:date="http://www.osiris-sps.org/xsl/date" xmlns:system="http://www.osiris-sps.org/xsl/system" xmlns:os="http://www.osiris-sps.org/xhtml/os" exclude-result-prefixes="lang date system os" version="1.0">
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes" /> 
  
  <xsl:template match="/html">     
  	<link href="/{//@extension_id}/css/htmlsnippet.css" type="text/css" rel="stylesheet"/>
		<script src="/{//@extension_id}/js/htmlsnippet.js" type="text/javascript"/>

    <xsl:variable name="logo_href">
      <xsl:value-of select="concat('/',//@extension_id,'/images/snippets/',@code,'.png')"/>
    </xsl:variable>
    <xsl:variable name="play_href">
      <xsl:value-of select="concat('/',//@extension_id,'/images/play.png')"/>
    </xsl:variable>
    
    <div id="{@uid}_play" class="os_htmlsnippet_play" style="width:{@width}px;height:{@height}px;background-image:url('{$play_href}'),url('{$logo_href}');" onclick="javascript:Osiris.HtmlSnippet.play('{@uid}', '{@code}', '{@html}', {@reload}, '{@token}');">
    </div>
    <div id="{@uid}_dialog" style="display:none;" title="{lang:text(concat('extensions.',//@extension_id,'.label.confirm.title'))}">
			<div class="os_content">
        <img class="os_htmlsnippet_box_logo" src="{$logo_href}" alt="{@name}" title="{@name}"/>
        <div class="os_htmlsnippet_play_message">
          <xsl:value-of select="lang:text(concat('extensions.',//@extension_id,'.label.confirm.message'))"/>
        </div>
        <div class="os_htmlsnippet_play_name">
          <xsl:value-of select="@name"/>
        </div>
        <div class="os_htmlsnippet_play_url">
          <a target="_blank" href="{@href}">
            <xsl:value-of select="@url"/>
          </a>          
        </div>
        <div class="os_htmlsnippet_play_security">
          <span class="os_label">
            <xsl:value-of select="lang:text(concat('extensions.',//@extension_id,'.label.confirm.security'))"/>
            <xsl:text> : </xsl:text>
          </span>
          <span class="os_value">
            <xsl:value-of select="system:parse(lang:text(concat('extensions.',//@extension_id,'.security.',@security,'.help')))" disable-output-escaping="yes"/>
          </span>
        </div>
        <div style="clear:both;text-align:right;padding:5px;">
          <input id="{@uid}_remember" type="checkbox">
            <xsl:value-of select="lang:text('common.dontaskagain')"/>
          </input>
          </div>
		  	<div id="{@uid}_buttons_No" style="display:none;">
          <xsl:value-of select="lang:text(concat('extensions.',//@extension_id,'.label.buttons.no'))"/>
        </div>		  	
		  	<div id="{@uid}_buttons_Yes" style="display:none;">
          <xsl:value-of select="lang:text(concat('extensions.',//@extension_id,'.label.buttons.yes'))"/>
        </div>
		  </div>
		</div>
   	<div id="{@uid}_placeholder" style="display:none;"></div>
  </xsl:template>

</xsl:stylesheet>
