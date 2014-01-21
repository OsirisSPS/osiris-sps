<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xsl/os" 
                exclude-result-prefixes="lang date system os"
                version="1.0">
  
  <xsl:output method="html"/>

  <xsl:param name="width"/>
  <xsl:param name="height"/>
  <xsl:param name="number"/>
  
  <xsl:template match="flash_clocks_com">    
    <table>
      <tr>
        <td class="os_label">
          <xsl:text>Width</xsl:text>
         </td>
        <td class="os_value" style="width:100%">
          <xsl:value-of select="$width" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:text>Height</xsl:text>
        </td>
        <td class="os_value" style="width:100%">
          <xsl:value-of select="$height" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:text>Number (extract it from website embedded code, or click a sample above)</xsl:text>
        </td>
        <td class="os_value" style="width:100%">
          <xsl:value-of select="$number" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td colspan="2">
          <xsl:call-template name="sample">
            <xsl:with-param name="number" select="'183'"/>
          </xsl:call-template>
          <xsl:call-template name="sample">
            <xsl:with-param name="number" select="'179'"/>
          </xsl:call-template>
          <xsl:call-template name="sample">
            <xsl:with-param name="number" select="'175'"/>
          </xsl:call-template>
          <xsl:call-template name="sample">
            <xsl:with-param name="number" select="'124'"/>
          </xsl:call-template>
          <xsl:call-template name="sample">
            <xsl:with-param name="number" select="'61'"/>
          </xsl:call-template>
          <xsl:call-template name="sample">
            <xsl:with-param name="number" select="'52'"/>
          </xsl:call-template>
          <xsl:call-template name="sample">
            <xsl:with-param name="number" select="'34'"/>
          </xsl:call-template>
          <xsl:call-template name="sample">
            <xsl:with-param name="number" select="'153'"/>
          </xsl:call-template>

          <div style="clear:both;text-align:center">
            <xsl:text>More here: </xsl:text>
            <a href="http://flash-clocks.com/">
              <xsl:text>http://flash-clocks.com/</xsl:text>
            </a>
          </div>
        </td>
      </tr>
    </table>
  </xsl:template>

  <xsl:template name="sample">
    <xsl:param name="number"/>
    <div style="border:1px solid gray; background-color:white;margin:5px;padding:5px;float:left">
      <embed src="http://flash-clocks.com/free-flash-clocks-blog-topics/free-flash-clock-{$number}.swf" width="200" height="200" wmode="transparent" type="application/x-shockwave-flash">
      </embed>
      <div style="background-color:gray;color:white;text-align:center">Number: 
        <tt style="font-size:2em;font-weight:bold">
          <xsl:value-of select="$number"/>
        </tt>
      </div>
    </div>
  </xsl:template>
</xsl:stylesheet>
