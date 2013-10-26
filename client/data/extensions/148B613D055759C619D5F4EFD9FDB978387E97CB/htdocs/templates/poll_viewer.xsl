<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                exclude-result-prefixes="lang date system"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object_actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/pager.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/user.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/post_viewer.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/tag_viewer.xsl"/>

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="vote_editor"/>
  
  <xsl:template match="poll">
    <xsl:variable name="poll_votes" select="stats/@poll_votes"/>
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="@title"/>
      <xsl:with-param name="content">
        <table class="os_table">
          <tr>
            <td>
              <xsl:call-template name="authors"/>
            </td>
            <td style="width:100%">
              <table class="os_table">
                <tr>
                  <td colspan="3">
                    <div style="float:right;">
                      <xsl:call-template name="rating-edit">
                        <xsl:with-param name="votes" select="stats/@votes"/>
                        <xsl:with-param name="votes_average" select="stats/@votes_average"/>
                        <xsl:with-param name="editor" select="$vote_editor"/>
                      </xsl:call-template>
                    </div>
                  </td>
                </tr>
                <xsl:choose>
                  <xsl:when test="poll_options/poll_option">
                    <xsl:for-each select="poll_options/poll_option">
                      <xsl:variable name="option_href">
                        <xsl:choose>
                          <xsl:when test="actions/action[@name = 'vote']">
                            <xsl:value-of select="actions/action[@name = 'vote']/@href"/>
                          </xsl:when>
                          <xsl:when test="actions/action[@name = 'unvote']">
                            <xsl:value-of select="actions/action[@name = 'unvote']/@href"/>
                          </xsl:when>
                        </xsl:choose>
                      </xsl:variable>
                      <xsl:variable name="option_votes" select="stats/@votes"/>
                      <xsl:variable name="option_percentage">
                        <xsl:choose>
                          <xsl:when test="$poll_votes = 0">
                            <xsl:value-of select="0"/>
                          </xsl:when>
                          <xsl:otherwise>
                            <xsl:value-of select="($option_votes div $poll_votes) * 100"/>
                          </xsl:otherwise>
                        </xsl:choose>
                      </xsl:variable>
                      <tr>
                        <td width="25%">
                          <a href="{$option_href}">
                            <xsl:choose>
                              <xsl:when test="@current='true'">
                                <b>
                                  <xsl:value-of select="@title"/>
                                </b>
                              </xsl:when>
                              <xsl:otherwise>
                                <xsl:value-of select="@title"/>
                              </xsl:otherwise>
                            </xsl:choose>
                          </a>
                        </td>
                        <td width="15%">
                          <xsl:text>[ </xsl:text>
                          <b>
                            <xsl:value-of select="stats/@votes"/>
                          </b>
                          <xsl:text> ]</xsl:text>
                        </td>
                        <td width="60%">
                          <img height="11px" src="{system:resource-url('images/bar_left.gif')}"/>
                          <img height="11px" width="{concat(round($option_percentage)*2, 'px')}" src="{system:resource-url('images/bar.gif')}"/>
                          <img height="11px" src="{system:resource-url('images/bar_right.gif')}"/>
                          <xsl:text> [</xsl:text>
                          <xsl:value-of select="format-number($option_percentage, '#0.00')"/>
                          <xsl:text>%]</xsl:text>
                        </td>
                      </tr>
                    </xsl:for-each>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="lang:text('poll.viewer.no_options')"/>
                  </xsl:otherwise>
                </xsl:choose>
                
                <tr>
                  <td colspan="3">
                    <br/>
                    <b>
                      <xsl:value-of select="lang:text('poll.viewer.votes')"/>
                      <xsl:text>: </xsl:text>
                      <xsl:value-of select="$poll_votes"/>
                    </b>
                    <br/>
                  </td>
                </tr>                
              </table>    
            </td>
          </tr>
        </table>
        <xsl:apply-templates select="tags"/>
      </xsl:with-param>
    </xsl:call-template>

    <xsl:call-template name="objectActions"/>
    <xsl:if test="posts">
      <xsl:apply-templates select="posts"/>
      <xsl:call-template name="objectActions"/>
    </xsl:if>
  </xsl:template>
  
</xsl:stylesheet>