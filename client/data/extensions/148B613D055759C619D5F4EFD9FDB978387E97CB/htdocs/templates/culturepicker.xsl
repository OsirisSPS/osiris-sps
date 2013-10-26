<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="showAll"/>
  
  <xsl:template match="root">
    <xsl:variable name="current" select="@current"/>
    <table class="os_table">
      <tr>
        <td>
          <div style="float:right;">
            <xsl:value-of select="$showAll" disable-output-escaping="yes"/>
          </div>
        </td>
      </tr>
      <tr>
        <th>
          <xsl:value-of select="lang:text('cultures.current')"/>
        </th>
      </tr>
      <tr>
        <td>
          <xsl:choose>
            <xsl:when test="$current = ''">
              <xsl:apply-templates select="//culture[@id=//@system]">
                <xsl:with-param name="as_system" select="true()"/>
              </xsl:apply-templates>
            </xsl:when>
            <xsl:otherwise>
              <xsl:apply-templates select="//culture[@id=$current]"/>
            </xsl:otherwise>
          </xsl:choose>          
        </td>
      </tr>
      <tr>
        <th>
          <xsl:value-of select="lang:text('cultures.available')"/>
        </th>
      </tr>
      <xsl:if test="//@showSystem = 'true'">
        <tr>
          <td>
            <xsl:apply-templates select="//culture[@id=//@system]">
              <xsl:with-param name="as_system" select="true()"/>
            </xsl:apply-templates>
          </td>
        </tr>
      </xsl:if>
      <tr>
        <td>
          <xsl:apply-templates select="//culture[@id='inv']"/>
        </td>
      </tr>
    </table>
  </xsl:template>

  <xsl:template match="cultures">
    <xsl:apply-templates />
  </xsl:template>

  <xsl:template match="culture">
    <xsl:param name="as_system" select="false()"/>

    <xsl:if test="@is_neutral='false' and @id!='inv'">
      <a class="os_cultures_selectable">
        <xsl:attribute name="href">
          <xsl:choose>
            <xsl:when test="$as_system">
              <xsl:value-of select="//@system_href"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="@href"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
        <xsl:call-template name="culture_detail">
          <xsl:with-param name="as_system" select="$as_system"/>
        </xsl:call-template>
      </a>
    </xsl:if>
    <xsl:if test="@is_neutral='true' or @id='inv'">
      <div class="os_cultures_neutral">
        <xsl:call-template name="culture_detail">
          <xsl:with-param name="as_system" select="$as_system"/>
        </xsl:call-template>
      </div>
    </xsl:if>


  </xsl:template>

  <xsl:template name="culture_detail">
    <xsl:param name="as_system"/>
    <div class="os_cultures_detail">
      <span class="os_label">
        <xsl:value-of select="lang:text('cultures.code')"/>
        <xsl:text>: </xsl:text>
      </span>
      <span class="os_value">
        <xsl:value-of select="@id"/>
      </span>
      <span class="os_label">
        <xsl:text> - </xsl:text>
        <xsl:value-of select="lang:text('cultures.lcid')"/>
        <xsl:text>: </xsl:text>
      </span>
      <span class="os_value">
        <xsl:value-of select="@LCID"/>
      </span>
      <span class="os_label">
        <xsl:text> - </xsl:text>
        <xsl:value-of select="lang:text('cultures.iso')"/>
        <xsl:text>: </xsl:text>
      </span>
      <span class="os_value">
        <xsl:value-of select="@iso_name"/>
      </span>
      <br />
      <span class="os_label">
        <xsl:value-of select="lang:text('cultures.translation')"/>
        <xsl:text>: </xsl:text>
      </span>
      <span class="os_value">
        <xsl:value-of select="@translation_derived"/>
      </span>
      <xsl:if test="@is_neutral='false'">
        <br />
        <span class="os_label">
          <xsl:value-of select="lang:text('cultures.samples.long_date')"/>
          <xsl:text>: </xsl:text>
        </span>
        <span class="os_value">
          <xsl:value-of select="@sample_long_datetime"/>
        </span>
        <br />
        <span class="os_label">
          <xsl:value-of select="lang:text('cultures.samples.short_date')"/>
          <xsl:text>: </xsl:text>
        </span>
        <span class="os_value">
          <xsl:value-of select="@sample_short_datetime"/>
        </span>
      </xsl:if>
    </div>
    <div class="os_cultures_flag">
      <img src="{system:resource-url(concat('images/flags/icon/',@id,'.png'))}"/>
    </div>
    <span style="font-size:1.2em">
      <xsl:if test="$as_system">
        <xsl:value-of select="lang:text('cultures.is_system')"/>
        <xsl:text> > </xsl:text>
      </xsl:if>
      <xsl:value-of select="@english_name"/>
    </span>
    <br />
    <span style="font-size:0.8em">
      <xsl:value-of select="@native_name"/>
    </span>

    <div style="clear:both;"/>

    <div class="os_cultures_sub" colspan="3">
      <xsl:apply-templates select="cultures/culture">
        <xsl:sort select="@id"/>
      </xsl:apply-templates>
    </div>    
  </xsl:template>

</xsl:stylesheet>


