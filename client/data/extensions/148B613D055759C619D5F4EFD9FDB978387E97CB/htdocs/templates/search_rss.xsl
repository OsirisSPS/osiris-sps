<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system"
                xmlns:date="http://www.osiris-sps.org/xsl/date"                 
                exclude-result-prefixes="lang system date"
                version="1.0">
  
  <xsl:output method="xml" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="feed">
    <rss xmlns:itunes="http://www.itunes.com/dtds/podcast-1.0.dtd" xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:taxo="http://purl.org/rss/1.0/modules/taxonomy/" xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" version="2.0">
      <channel>
        <title>
          <xsl:value-of select="@title"/>
        </title>
        <link>
          <xsl:value-of select="@link"/>
        </link>
        <description>
          <xsl:value-of select="@description"/>
        </description>
        <language>
          <xsl:value-of select="@language"/>
        </language>
        <pubDate>
          <xsl:value-of select="date:rfc822-datetime(date:current-datetime())"/>
        </pubDate>

        <xsl:apply-templates select="results/object"/>
      </channel>
    </rss>
  </xsl:template>

  <xsl:template match="object">
    <item>
      <title>
        <!-- Note 10/2011: This produce a 'double encoding'. The first is the XML, the second is HTML. For example 'Osiris & Isis' -> 'Osiris &amp;amp; Isis'.
        It's correct, although some aggregator don't handle it correctly. The same aggregator handle correctly the 'description' fields.
        So it's a problem related to understand RSS specifications. Doesn't exists an universal solution. -->
        <xsl:value-of select="system:parse(@title, false(), false(), true(), 2, @entity)" />
      </title>
      <link>
        <xsl:value-of select="@view_href"/>
      </link>
      <description>
        <xsl:choose>
          <xsl:when test="//@body = 'link'">
            <xsl:value-of select="system:parse(concat('[url=',@view_href,']',lang:text('common.read_full_article'),'[/url]'), false(), false(), false(), 2, @entity, @view_href)" />
          </xsl:when>
          <xsl:when test="//@body = 'none'">
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="system:parse(@content, false(), false(), false(), 2, @entity, @view_href)" />            
          </xsl:otherwise>
        </xsl:choose>
        
      </description>
      <pubDate>
        <xsl:value-of select="date:rfc822-datetime(@entity_date)"/>
      </pubDate>
      <guid isPermaLink="false">
        <xsl:value-of select="@entity"/>
      </guid>
    </item>
  </xsl:template>
                
</xsl:stylesheet>


