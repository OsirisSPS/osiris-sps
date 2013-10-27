<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="password"/>
  <xsl:param name="login"/>
  <xsl:param name="logout"/>

  <xsl:template match="mcp">
    <div id="container" class="os_mcp_container" style="display:none">
      <div class="os_mcp_panel">
        <div class="os_mcp_panel2">
          <xsl:choose>
            <xsl:when test="$logout">
              <div>
                <xsl:value-of select="lang:text('main.pages.mcp.logged')"/>
              </div>
              <br />
              <br />
              <xsl:value-of select="$logout" disable-output-escaping="yes"/>
            </xsl:when>
            <xsl:when test="$login">
              <div>
                <xsl:value-of select="lang:text('main.pages.mcp.login')"/>
              </div>
              <br />
              <br />
              <xsl:if test="@wrong">
                <div id="wrong" class="os_mcp_wrong">
                  <xsl:value-of select="lang:text('main.pages.mcp.wrong')"/>
                </div>
                <br/>
                <br/>
              </xsl:if>
              <span>
                <xsl:value-of select="lang:text('main.pages.mcp.password')"/>
              </span>
              <xsl:text>: </xsl:text>
              <xsl:value-of select="$password" disable-output-escaping="yes"/>
              <br />
              <br />
              <xsl:value-of select="$login" disable-output-escaping="yes"/>
            </xsl:when>
            <xsl:otherwise>
              <div>
                <xsl:value-of select="lang:text('main.pages.mcp.always')"/>
              </div>
            </xsl:otherwise>
          </xsl:choose>
        </div>
      </div>
    </div>

    <script type="text/javascript">
      jQuery('#container').fadeIn(Osiris.effectsSpeedSlow, function()
      {
      <xsl:if test="$password">
        osGetById("page_password").select();
        osGetById("page_password").focus();
      </xsl:if>
      });
      <!--
      new Effect.Appear('container', { queue: 'end', duration: 1 });
      -->



    </script>
        
  </xsl:template>

  
</xsl:stylesheet>