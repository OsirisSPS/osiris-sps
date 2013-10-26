<?xml version='1.0' encoding='utf-8'?>

<!--
Nota: Questo template fa il dump dei tag e degli attributi, non del testo presente tra i tag.
Ad esempio,

Xml:

<alfa>
  Prova
  <beta i="123"/>
</alfa>

Dump:

<alfa>
  <beta i="123"></beta>
</alfa>


Un XSL completo, quello usato da IE, è disponibile all'url 'res://msxml.dll/DEFAULTSS.xsl' da IE.
-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="os"
                version="1.0">

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

<!--
  <xsl:template match="/">
    <xsl:call-template name="dump_xml"/>
  </xsl:template>
-->

  <!--
  <xsl:template name="dump_xml">        
    <div data-os-otype="roller" data-os-allowClose="true" data-os-startClose="false" style="color: #FFFFFF; background-color: #000000; height:15px; align: left; padding: 5px; margin: 5px; border: 1px solid #DDDDDD; cursor: hand; font-size: x-small;">
      <div data-os-roller="header">
        Show XML
      </div>
      <div data-os-roller="body" style="display: none; color: #FFFFFF; padding:2px; background-color: #000000; max-height:200px; overflow: auto;">
        <xsl:call-template name="dump_xml_singletag"/>
      </div>
    </div>      
  </xsl:template>
  -->
  
  <xsl:template name="dump_xml">
    <xsl:param name="open" select="true()"/>
    <xsl:choose>
      <xsl:when test="$open">
        <div style="text-align: left; color: #FFFFFF; padding:2px; background-color: #000000; ">
          <xsl:call-template name="dump_xml_singletag"/>
        </div>
      </xsl:when>
      <xsl:otherwise>
        <div data-os-otype="popup" data-os-popupMode="staticclick">
          <div data-os-popup="header" style="text-align: center; color: #FFFFFF; background-color: #000000; height:15px; align: left; padding: 5px; margin: 5px; border: 1px solid #DDDDDD; cursor: hand; font-size: x-small;">
            Show XML
          </div>
          <div data-os-popup="body" style="display: none; text-align: left; color: #FFFFFF; padding:2px; background-color: #000000; max-height:200px; overflow: auto;">
            <xsl:call-template name="dump_xml_singletag"/>
          </div>
        </div>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="dump_xml_singletag">       
    <div style="color: #000000; background-color: #FFFFFF; align: left; padding: 5px; margin: 5px; border: 1px solid #DDDDDD;">
      <span>&lt;</span>
      <span style="color: #008800; background-color: transparent;">
        <xsl:value-of select="name()" />
      </span>
      <xsl:for-each select="@*">
        <br/>
        <span style="color: #0000FF; background-color: transparent; padding-left:20px;">
          <xsl:value-of select="name()" />
        </span>
        <span>="</span>
        <span style="color: #FF0000; background-color: transparent;">
          <!--<xsl:value-of select="." />-->
          <xsl:call-template name="globalReplace">
            <xsl:with-param name="outputString" select="."/>
            <xsl:with-param name="target" select="'$'"/>
            <xsl:with-param name="replacement" select="'&#x20;'"/>
          </xsl:call-template>
        </span>
        <span>"</span>	
      </xsl:for-each>
      <span>&gt;</span>
      <xsl:for-each select="*">
        <xsl:call-template name="dump_xml_singletag"/>
      </xsl:for-each>
      <span>&lt;/</span>
      <span style="color: #008800; background-color: transparent;">
        <xsl:value-of select="name()" />
      </span>
      <span>&gt;</span>
    </div>    
  </xsl:template>

  <xsl:template name="globalReplace">
    <xsl:param name="outputString"/>
    <xsl:param name="target"/>
    <xsl:param name="replacement"/>
    <xsl:choose>
      <xsl:when test="contains($outputString,$target)">
	<!--
        <xsl:value-of select=
        "concat(substring-before($outputString,$target),
               $replacement)" disable-output-escaping="yes"/>
        <xsl:call-template name="globalReplace">
          <xsl:with-param name="outputString" 
               select="substring-after($outputString,$target)"/>
          <xsl:with-param name="target" select="$target"/>
          <xsl:with-param name="replacement" 
               select="$replacement"/>
        </xsl:call-template>
	-->
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$outputString"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>


</xsl:stylesheet>


