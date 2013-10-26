<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                exclude-result-prefixes="lang date system"
                version="1.0">

<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  
<xsl:output method="html"/>

  <xsl:template match="page">
    <div class="os_external_redirect" id="redirect" style="display:none;">
      <div class="os_external_warning">        
        <xsl:value-of select="lang:text('main.pages.external.run')"/>
      </div>
      <div class="os_external_url">
        <xsl:value-of select="@url"/>
      </div>
      <div class="os_commands">
        <a class="os_button" id="skip_link" href="{@url}">
          <xsl:value-of select="lang:text('main.pages.external.skip_link')"/>
        </a>
      </div>
    </div>
    <script>
      <![CDATA[
        function doStartRedirect()
        {
          var divWarning = osGetById("warning");
          if(divWarning)
          divWarning.style.display = "none";
          osGetById("redirect").style.display = "block";        
          timerID = setTimeout("doRedirect()", 2000);
        }
        function doRedirect()
        {
          window.location = osGetById("skip_link").href;       
        }        
        ]]>
    </script>
    <xsl:choose>
      <xsl:when test="@confirm='true'">
        <div id="warning" class="os_external_confirm">
          <div class="os_external_warning">
            <xsl:value-of select="lang:text('main.pages.external.warning')"/>
          </div>
          <div class="os_external_url">
            <xsl:value-of select="@url"/>
          </div>
          <div class="os_commands">
            <a class="os_button" href="javascript:doStartRedirect()">
              <xsl:value-of select="lang:text('main.pages.external.confirm_link')"/>
            </a>
          </div>
        </div>
      </xsl:when>
      <xsl:otherwise>
        <script>
          <![CDATA[
            doStartRedirect();
            ]]>
        </script>
      </xsl:otherwise>
    </xsl:choose>


  </xsl:template>
	
</xsl:stylesheet>