<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>
  
  <xsl:template name="rating-view">
    <xsl:param name="votes" select="'0'"/>
    <xsl:param name="votes_average" select="'0'"/>
    <div class="os_rating_bg" data-os-tooltip="{lang:text('rating.votes')}: {$votes}, {lang:text('rating.average')}: {$votes_average div 2}">
      <div class="os_rating_fg" style="width:{$votes_average*5.5}px;">        
      </div>
    </div>    
  </xsl:template>

  <xsl:template name="rating-edit">
    <xsl:param name="votes" select="'0'"/>
    <xsl:param name="votes_average" select="'0'"/>
    <xsl:param name="editor"/>
    <div class="os_rating_bg" style="margin:4px;float:left;" data-os-tooltip="{lang:text('rating.votes')}: {$votes}, {lang:text('rating.average')}: {$votes_average div 2}">
      <div class="os_rating_fg" style="width:{$votes_average*5.5}px;">
      </div>
    </div>
    <xsl:value-of select="$editor" disable-output-escaping="yes"/>
  </xsl:template>

</xsl:stylesheet>
