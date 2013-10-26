<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:exsl="http://exslt.org/common" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system"                 
                exclude-result-prefixes="exsl os date lang system"
                version="1.0">

  <xsl:template name="portal-box">
    <!--
    <img style="float:left" src="{system:resource-url(concat('images/objects/special_portal_', @access, '.png'))}">
      <xsl:attribute name="title">
        <xsl:call-template name="portal-type"/>
      </xsl:attribute>
    </img>
    -->
    <a style="font-size:1.3em" href="{@href}">
      <xsl:value-of select="system:parse(@name, false(), false(), true())" disable-output-escaping="yes" />
    </a>
    <xsl:if test="@description">
      <br/>
      <span class="os_description">
        <xsl:value-of select="system:parse(@description, false(), false(), true())" disable-output-escaping="yes" />
      </span>
    </xsl:if>        
  </xsl:template>

  <!--
  <xsl:template name="portal-type">
        
    <xsl:choose>
      <xsl:when test="@access='public'">
        <xsl:value-of select="lang:text('portal.type.public')"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="lang:text('portal.type.private')"/>
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>
  -->

  <xsl:template name="boolean-icon">
    <xsl:param name="value" />
    <xsl:param name="icon" />
    <xsl:choose>
      <xsl:when test="$value = 'true'">
        <xsl:variable name="desc" select="lang:text('common.boolean.yes')"/>
        <img src="{system:resource-url('images/icons/16x16/on.png')}" alt="{$desc}" data-os-tooltip="{$desc}"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="desc" select="lang:text('common.boolean.no')"/>
        <img src="{system:resource-url('images/icons/16x16/off.png')}" alt="{$desc}" data-os-tooltip="{$desc}"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="status-icon">
    <xsl:param name="value" />
    <xsl:param name="icon" />
    <xsl:choose>
      <xsl:when test="$value = 'true'">
        <xsl:variable name="desc" select="lang:text('common.boolean.yes')"/>
        <img src="{system:resource-url('images/icons/16x16/on.png')}" alt="{$desc}" data-os-tooltip="{$desc}"/>
      </xsl:when>
      <xsl:when test="$value = 'false'">
        <xsl:variable name="desc" select="lang:text('common.boolean.no')"/>
        <img src="{system:resource-url('images/icons/16x16/off.png')}" alt="{$desc}" data-os-tooltip="{$desc}"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="desc" select="lang:text('common.status.unknown')"/>
        <img src="{system:resource-url('images/icons/16x16/unknown.png')}" alt="{$desc}" data-os-tooltip="{$desc}"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="hash">
    <xsl:param name="hash"/>
    <span class="os_hash">
      <xsl:value-of select="$hash"/>      
      <span style="margin-left:5px;padding-right:32px;background-color:#{substring($hash,10,6)}"></span>
    </span>
    
  </xsl:template>

  <xsl:template name="filter-alphabetic">
    <div class="os_filter_alphabetic">
      <div class="os_filter_alphabetic_letters">
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'*'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'A'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'B'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'C'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'D'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'E'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'F'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'G'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'H'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'I'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'J'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'K'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'L'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'M'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'N'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'O'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'P'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'Q'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'R'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'S'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'T'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'U'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'V'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'W'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'X'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'Y'"/>
        </xsl:call-template>
        <xsl:call-template name="filter-alphabetic-letter">
          <xsl:with-param name="l" select="'Z'"/>
        </xsl:call-template>
      </div>
      <div style="clear:both"/>      
    </div>
  </xsl:template>

  <xsl:template name="filter-alphabetic-letter">
    <xsl:param name="l"/>
    <xsl:param name="t" select="$l"/>    
    <a href="{system:replace(@href_filter_alphabetic,'Letter',$l)}" title="{$l}">
      <xsl:choose>
        <xsl:when test="$l = @current_filter_alphabetic">
          <xsl:attribute name="class">
            os_filter_alphabetic_selected
          </xsl:attribute>
        </xsl:when>        
        <xsl:otherwise>
          <xsl:attribute name="class">
          os_filter_alphabetic_letter
          </xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>      
      <xsl:value-of select="$t"/>
    </a>
  </xsl:template>

</xsl:stylesheet>