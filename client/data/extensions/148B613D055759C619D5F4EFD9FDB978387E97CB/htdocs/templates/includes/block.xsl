<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
		            xmlns:system="http://www.osiris-sps.org/xsl/system"                 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang"                 
                exclude-result-prefixes="os system lang"
                version="1.0">

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template name="block">
    <!--Titolo del blocco-->
    <xsl:param name="title" />
    <!--Link opzionale-->
    <xsl:param name="href" />
    <!--Style interno opzionale-->
    <xsl:param name="innerStyle" />
    <!--Style esterno opzionale-->
    <xsl:param name="outerStyle" />
    <!--Body opzionale del blocco (testo)-->
    <xsl:param name="body" />
    <!--Apertura opzionale del blocco-->
    <xsl:param name="startClose" />
    <!--Storage di salvataggio lato client-->
    <xsl:param name="storage" />
    <!--Contenuto opzionale del blocco content-->
    <xsl:param name="content" />

    <xsl:call-template name="block_big">
      <xsl:with-param name="title" select="$title"/>
      <xsl:with-param name="href" select="$href"/>
      <xsl:with-param name="innerStyle" select="$innerStyle"/>
      <xsl:with-param name="outerStyle" select="$outerStyle"/>
      <xsl:with-param name="startClose" select="$startClose"/>
      <xsl:with-param name="storage" select="$storage"/>
      <xsl:with-param name="body" select="$body"/>
      <xsl:with-param name="content" select="$content"/>
    </xsl:call-template>    
  </xsl:template>

  <xsl:template name="block_page">
    <xsl:param name="prefix" />
    <xsl:param name="body" />
    <xsl:param name="content" />
    <div class="os_page_title">
      <xsl:value-of select="lang:text(concat($prefix,'.title'))"/>
    </div>
    <div class="os_page_description">
      <xsl:value-of select="lang:text(concat($prefix,'.description'))"/>
    </div>
    <xsl:if test="$body">
      <xsl:value-of select="$body" disable-output-escaping="yes"/>
    </xsl:if>
    <xsl:if test="$content">
      <xsl:copy-of select="$content"/>
    </xsl:if>

  </xsl:template>

  <xsl:template name="block_item">
    <xsl:param name="title" />
    <xsl:param name="innerStyle" />
    <xsl:param name="style" />
    <xsl:param name="body" />
    <xsl:param name="content" />

    <div class="os_block_item">
      <xsl:if test="$style">
        <xsl:attribute name="style">
          <xsl:value-of select="$style"/>
        </xsl:attribute>
      </xsl:if>
      <div class="os_block_item_title">
        <xsl:copy-of select="$title"/>        
      </div>
      <xsl:if test="$body or $content">
        <div style="clear:both;"/>
        <div class="os_block_item_body">
          <xsl:if test="$innerStyle">
            <xsl:attribute name="style">
              <xsl:value-of select="$innerStyle"/>
            </xsl:attribute>
          </xsl:if>
          <xsl:if test="$body">
            <xsl:value-of select="$body" disable-output-escaping="yes"/>
          </xsl:if>
          <xsl:if test="$content">
            <xsl:copy-of select="$content"/>
          </xsl:if>
        </div>
      </xsl:if>
    </div>    
  </xsl:template>

  <xsl:template name="block_none">
    <xsl:param name="style" />
    <xsl:param name="body" />
    <xsl:param name="content" />

    <div class="os_block_none">
      <xsl:if test="$style">
        <xsl:attribute name="style">
          <xsl:value-of select="$style"/>
        </xsl:attribute>
      </xsl:if>
      <xsl:if test="$body">
        <xsl:value-of select="$body" disable-output-escaping="yes"/>
      </xsl:if>
      <xsl:if test="$content">
        <xsl:copy-of select="$content"/>
      </xsl:if>
    </div>
  </xsl:template>

  <xsl:template name="block_heading">
    <xsl:param name="title" />
    <xsl:param name="href" />
    <xsl:param name="innerStyle" />
    <xsl:param name="outerStyle" />
    <xsl:param name="body" />
    <xsl:param name="content" />

    <div class="os_block_heading">
      <xsl:if test="$outerStyle">
        <xsl:attribute name="style">
          <xsl:value-of select="$outerStyle"/>
        </xsl:attribute>
      </xsl:if>
      
      <h1>
        <xsl:choose>
          <xsl:when test="$href">
            <a href="{$href}">
              <xsl:value-of select="system:parse(string($title), false(), false(), true())" disable-output-escaping="yes"/>
            </a>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="system:parse(string($title), false(), false(), true())" disable-output-escaping="yes"/>
          </xsl:otherwise>
        </xsl:choose>
      </h1>

      <div>
        <xsl:if test="$innerStyle">
          <xsl:attribute name="style">
            <xsl:value-of select="$innerStyle"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="$body">
          <xsl:value-of select="$body" disable-output-escaping="yes"/>
        </xsl:if>
        <xsl:if test="$content">
          <xsl:copy-of select="$content"/>
        </xsl:if>
      </div>      
    </div>
    <div style="clear:both;"></div>
  </xsl:template>

  <xsl:template name="block_minimal">
    <xsl:param name="title" />
    <xsl:param name="href" />
    <xsl:param name="innerStyle" />
    <xsl:param name="outerStyle" />
    <xsl:param name="body" />
    <xsl:param name="startClose" />
    <xsl:param name="storage" />
    <xsl:param name="content" />

    <div class="os_block_minimal" data-os-otype="roller" data-os-allowClose="true">
      <xsl:if test="$outerStyle">
        <xsl:attribute name="style">
          <xsl:value-of select="$outerStyle"/>
        </xsl:attribute>
      </xsl:if>
      <xsl:if test="$startClose">
        <xsl:attribute name="data-os-startClose">
          <xsl:value-of select="$startClose"/>
        </xsl:attribute>
      </xsl:if>
      <xsl:if test="$storage">
        <xsl:attribute name="data-os-storage">
          <xsl:value-of select="$storage"/>
        </xsl:attribute>
      </xsl:if>
            <div class="os_block_minimal_collapse" data-os-roller="header" data-os-class_collapse="os_block_minimal_collapse" data-os-class_expand="os_block_minimal_expand"/>
            <div class="os_block_minimal_title">
              <xsl:choose>
                <xsl:when test="$href">
                  <a href="{$href}">
                    <xsl:value-of select="system:parse(string($title), false(), false(), true())" disable-output-escaping="yes"/>
                  </a>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:value-of select="system:parse(string($title), false(), false(), true())" disable-output-escaping="yes"/>
                </xsl:otherwise>
              </xsl:choose>
            </div>
      <div style="clear:both;" class="os_block_minimal_body" data-os-roller="body">
        <xsl:if test="$innerStyle">
          <xsl:attribute name="style">
            <xsl:value-of select="$innerStyle"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="$startClose = true()">
          <xsl:attribute name="style">
            <xsl:text>display:none</xsl:text>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="$body">
          <xsl:value-of select="$body" disable-output-escaping="yes"/>
        </xsl:if>
        <xsl:if test="$content">
          <xsl:copy-of select="$content"/>
        </xsl:if>
      </div>      
    </div>
    <div style="clear:both;"></div>
  </xsl:template>
  
  <xsl:template name="block_small">
    <xsl:param name="title" />
    <xsl:param name="href" />
    <xsl:param name="innerStyle" />
    <xsl:param name="outerStyle" />
    <xsl:param name="body" />
    <xsl:param name="content" />

    <div class="os_block_small">
      <xsl:if test="$outerStyle">
        <xsl:attribute name="style">
          <xsl:value-of select="$outerStyle"/>
        </xsl:attribute>
      </xsl:if>
      <div class="os_block_small_title">
        <xsl:choose>
          <xsl:when test="$href">
            <a href="{$href}">
              <xsl:value-of select="system:parse(string($title), false(), false(), true())" disable-output-escaping="yes"/>
            </a>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="system:parse(string($title), false(), false(), true())" disable-output-escaping="yes"/>
          </xsl:otherwise>
        </xsl:choose>
      </div>
      <xsl:if test="$body or $content">
        <div style="clear:both;"/>
        <div class="os_block_small_body">
          <xsl:if test="$innerStyle">
            <xsl:attribute name="style">
              <xsl:value-of select="$innerStyle"/>
            </xsl:attribute>
          </xsl:if>
          <xsl:if test="$body">
            <xsl:value-of select="$body" disable-output-escaping="yes"/>
          </xsl:if>
          <xsl:if test="$content">
            <xsl:copy-of select="$content"/>
          </xsl:if>
        </div>
      </xsl:if>      
    </div>
    <div style="clear:both;"></div>
  </xsl:template>

  <xsl:template name="block_big">
    <xsl:param name="title" />
    <xsl:param name="href" />
    <xsl:param name="innerStyle" />
    <xsl:param name="outerStyle" />
    <xsl:param name="startClose" />
    <xsl:param name="storage"/>
    <xsl:param name="body" />
    <xsl:param name="content" />

    <div class="os_block_big" data-os-otype="roller" data-os-allowClose="true">
      <xsl:if test="$outerStyle">
        <xsl:attribute name="style">
          <xsl:value-of select="$outerStyle"/>
        </xsl:attribute>
      </xsl:if>
      <xsl:if test="$startClose">
        <xsl:attribute name="data-os-startClose">
          <xsl:value-of select="$startClose"/>
        </xsl:attribute>
      </xsl:if>
      <xsl:if test="$storage">
        <xsl:attribute name="data-os-storage">
          <xsl:value-of select="$storage"/>
        </xsl:attribute>
      </xsl:if>


      <div class="os_block_big_collapse" data-os-roller="header" data-os-class_collapse="os_block_big_collapse" data-os-class_expand="os_block_big_expand"></div>
      <div class="os_block_big_title">
        <xsl:choose>
          <xsl:when test="$href">
            <a href="{$href}">
              <xsl:value-of select="system:parse(string($title), false(), false(), true())" disable-output-escaping="yes"/>
            </a>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="system:parse(string($title), false(), false(), true())" disable-output-escaping="yes"/>
          </xsl:otherwise>
        </xsl:choose>

      </div>

      <div class="os_block_big_body" data-os-roller="body">
        <xsl:choose>
          <xsl:when test="$startClose">
            <xsl:attribute name="style">
              <xsl:value-of select="concat('clear:both;display:none;',$innerStyle)"/>
            </xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:attribute name="style">
              <xsl:value-of select="concat('clear:both;',$innerStyle)"/>
            </xsl:attribute>
          </xsl:otherwise>
        </xsl:choose>

        <xsl:if test="$body">
          <xsl:value-of select="$body" disable-output-escaping="yes"/>
        </xsl:if>
        <xsl:if test="$content">
          <xsl:copy-of select="$content"/>
        </xsl:if>

      </div>
    </div>
    <div style="clear:both;"></div>
  </xsl:template>

</xsl:stylesheet>