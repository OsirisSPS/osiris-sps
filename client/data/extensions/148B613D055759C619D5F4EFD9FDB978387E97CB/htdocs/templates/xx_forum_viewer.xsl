<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object_actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/pager.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/user.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/rating.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="forum">
    <xsl:if test="@rules">
      <xsl:call-template name="block_small">
        <xsl:with-param name="title" select="'regole'"/>
        <xsl:with-param name="body">
          <xsl:value-of select="system:parse(@rules)" disable-output-escaping="yes"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>
    
    <!-- Visualizza i sottoforums -->
    <xsl:if test="forums">
      <xsl:call-template name="block">
        <xsl:with-param name="title">
          <xsl:value-of select="@title"/>
          <xsl:if test="topics">
            >
            <xsl:value-of select="lang:text('forum.viewer.forum_forums')"/>
          </xsl:if>
        </xsl:with-param>
        <xsl:with-param name="href" select="@view_href"/>
        <xsl:with-param name="content">
          <xsl:apply-templates select="forums"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>

    <!--<xsl:call-template name="forumActions"/>-->
    <xsl:call-template name="objectActions"/>

    <!-- Visualizza i topics -->
    <xsl:if test="topics">      
      <xsl:call-template name="block">
        <xsl:with-param name="title">
          <xsl:value-of select="@title"/>
          <xsl:if test="forums">
            >
            <xsl:value-of select="lang:text('forum.viewer.forum_topics')"/>
          </xsl:if>
        </xsl:with-param>
        <xsl:with-param name="content">
          <xsl:apply-templates select="topics"/>
        </xsl:with-param>
      </xsl:call-template>      
      <!-- se ci sono topics mostra nuovamente le azioni nella parte sottostante -->
      <!--<xsl:call-template name="forumActions"/>-->
      <xsl:call-template name="objectActions"/>
    </xsl:if>

    <!-- Se non ci sono ne forums ne topics mostra solo il titolo del forum -->
    <xsl:if test="not(forums) and not(topics)">
      <xsl:call-template name="block">
        <xsl:with-param name="title">
          <xsl:value-of select="@title"/>          
        </xsl:with-param>
        <xsl:with-param name="href" select="@view_href"/>
      </xsl:call-template>
    </xsl:if>

    <xsl:call-template name="forum-legend">
      <xsl:with-param name="forums_legend" select="boolean(forums)"/>
      <xsl:with-param name="topics_legend" select="boolean(topics)"/>
    </xsl:call-template>
    
  </xsl:template>

  <xsl:template match="forums">
    <table class="os_table" cellspacing="0">
      <tr>
        <th colspan="2" width="55%">
          <xsl:value-of select="lang:text('forum.viewer.forum')"/>
        </th>
        <th width="10%" align="center">
          <xsl:value-of select="lang:text('forum.viewer.topics')"/>
        </th>
        <th width="10%" align="center">
          <xsl:value-of select="lang:text('forum.viewer.posts')"/>
        </th>
        <th width="25%">
          <xsl:value-of select="lang:text('forum.viewer.last_message')"/>
        </th>
      </tr>
      <xsl:apply-templates select="forum" mode="link"/>
    </table>
  </xsl:template>

  <xsl:template match="topics">
    <table class="os_table" cellspacing="0" style="clear:left;">
      <tr>
        <th colspan="2" width="45%">
          <xsl:value-of select="lang:text('forum.viewer.topic_title')"/>
        </th>
        <th width="10%" align="center">
          <xsl:value-of select="lang:text('forum.viewer.topic_posts')"/>
        </th>
        <th width="10%" align="center">
          <xsl:value-of select="lang:text('forum.viewer.topic_author')"/>
        </th>
        <th width="10%" align="center">
          <xsl:value-of select="lang:text('forum.viewer.topic_rating')"/>
        </th>
        <th width="25%">
          <xsl:value-of select="lang:text('forum.viewer.last_message')"/>
        </th>
      </tr>
      <xsl:for-each select="topic[@pinned = 'true']">
        <xsl:apply-templates select="." mode="link"/>
      </xsl:for-each>

      <xsl:if test="topic[@pinned = 'true'] and topic[@pinned = 'false']">
        <!-- se ci sono sia topics pinnati che non pinnati mostra il separatore -->        
        <tr>
          <td class="os_subtitle" colspan="6">
            <b>
              <xsl:value-of select="lang:text('forum.viewer.topics')"/>
            </b>
          </td>
        </tr>        
      </xsl:if>

      <xsl:for-each select="topic[@pinned = 'false']">
        <xsl:apply-templates select="." mode="link"/>
      </xsl:for-each>
    </table>
  </xsl:template>
  
  <xsl:template match="forum" mode="link">
    <tr data-os-objectId="{@id}">
      <td valign="middle" align="center" width="1%">
        <xsl:call-template name="object-state">
          <xsl:with-param name="new" select="date:compare(stats/@last_message_date, system:reference-date()) > 0"/>
          <xsl:with-param name="node" select="boolean(forums)"/>          
        </xsl:call-template>
      </td>
      <td>
        <b>
          <a href="{@view_href}">
            <xsl:value-of select="@title"/>
          </a>
        </b>
        <xsl:if test="@description != ''">
          <br />
          <span class="os_description">
            <xsl:value-of select="@description"/>
          </span>
        </xsl:if>
        <xsl:if test="forums">
          <br />
          <span class="os_description">
            <b>
              <xsl:value-of select="lang:text('forum.viewer.subforums')"/>:
            </b>
            <xsl:for-each select="forums/forum">
              <a href="{@view_href}">
                <xsl:value-of select="@title"/>
                <xsl:if test="forums">
                  (...)
                </xsl:if>
              </a>
              <xsl:if test="position() != last()">, </xsl:if>
            </xsl:for-each>
          </span>
        </xsl:if>
      </td>
      <td align="center">
        <xsl:value-of select="stats/@topics"/>
      </td>
      <td align="center">
        <xsl:value-of select="stats/@posts"/>
      </td>
      <td>
        <xsl:call-template name="forum_last_message">
          <xsl:with-param name="last_topic" select="stats/last_topic"/>
          <xsl:with-param name="last_message" select="stats/last_message"/>
          <xsl:with-param name="last_message_date" select="stats/@last_message_date"/>
        </xsl:call-template>
      </td>
    </tr>
  </xsl:template>

  <xsl:template match="topic" mode="link">
    <tr data-os-objectId="{@id}">
      <td valign="middle" align="center" width="1%">
        <xsl:call-template name="object-state">          
          <xsl:with-param name="new" select="(date:compare(@submit_date, system:reference-date()) > 0) or (date:compare(stats/@last_entry_date, system:reference-date()) > 0)"/>
          <xsl:with-param name="hot" select="stats/@posts >= system:get-option('ide.objects.hot_state_threshold')"/>
        </xsl:call-template>
      </td>        
      <td>        
        <div>
          <xsl:choose>
            <xsl:when test="@pinned = 'true'">
              <b>
                <xsl:value-of select="lang:text('forum.viewer.topic_pinned')"/>
                <xsl:text>: </xsl:text>
                <a href="{@view_href}">
                  <xsl:value-of select="@title"/>
                </a>
              </b>
            </xsl:when>
            <xsl:otherwise>
              <a href="{@view_href}">
                <xsl:value-of select="@title"/>
              </a>
            </xsl:otherwise>
          </xsl:choose>
          <xsl:if test="@description != ''">
            <div class="osForum_TopicDesc">
              <xsl:value-of select="@description"/>
            </div>
          </xsl:if>
        </div>
      </td>
      <td align="center">
        <xsl:value-of select="stats/@posts"/>
      </td>
      <td align="center">
        <xsl:apply-templates select="author" mode="row"/>        
      </td>
      <td align="center">
        <xsl:call-template name="rating-view">
          <xsl:with-param name="votes" select="stats/@votes"/>
          <xsl:with-param name="votes_average" select="stats/@votes_average"/>
        </xsl:call-template>
      </td>
      <td>
        <xsl:call-template name="topic_last_message">
          <xsl:with-param name="last_message" select="stats/last_entry"/>
          <xsl:with-param name="last_message_date" select="stats/@last_entry_date"/>
        </xsl:call-template>
      </td>
    </tr>
  </xsl:template>

  <xsl:template name="forum-legend">
    <xsl:param name="forums_legend" select="true()"/>
    <xsl:param name="topics_legend" select="true()"/>    

    <xsl:variable name="show_legend" select="$forums_legend or $topics_legend"/>
    <xsl:if test="$show_legend">
      <xsl:call-template name="block">
        <xsl:with-param name="title" select="lang:text('legend.title')"/>
        <xsl:with-param name="content">
          <xsl:if test="$forums_legend">
            <table class="os_table">
              <tr>
                <td class="os_subtitle" colspan="4">
                  <b>
                    <xsl:value-of select="lang:text('legend.forums.title')" />                    
                  </b>
                </td>
              </tr>
              <tr>
                <td align="center" width="25%">
                  <xsl:call-template name="object-state">
                    <xsl:with-param name="name" select="'forum'"/>
                    <xsl:with-param name="new" select="false()"/>
                    <xsl:with-param name="node" select="false()"/>
                  </xsl:call-template>
                </td>
                <td align="center" width="25%">
                  <xsl:call-template name="object-state">
                    <xsl:with-param name="name" select="'forum'"/>
                    <xsl:with-param name="new" select="true()"/>
                    <xsl:with-param name="node" select="false()"/>
                  </xsl:call-template>
                </td>
                <td align="center" width="25%">
                  <xsl:call-template name="object-state">
                    <xsl:with-param name="name" select="'forum'"/>
                    <xsl:with-param name="new" select="false()"/>
                    <xsl:with-param name="node" select="true()"/>
                  </xsl:call-template>
                </td>
                <td align="center" width="25%">
                  <xsl:call-template name="object-state">
                    <xsl:with-param name="name" select="'forum'"/>
                    <xsl:with-param name="new" select="true()"/>
                    <xsl:with-param name="node" select="true()"/>
                  </xsl:call-template>
                </td>                
              </tr>
              <tr>
                <td align="center" width="25%">
                  <xsl:value-of select="lang:text('legend.forums.forum')"/>
                </td>
                <td align="center" width="25%">
                  <xsl:value-of select="lang:text('legend.forums.forum_new')"/>                  
                </td>
                <td align="center" width="25%">
                  <xsl:value-of select="lang:text('legend.forums.forum_node')"/>                                  
                </td>
                <td align="center" width="25%">
                  <xsl:value-of select="lang:text('legend.forums.forum_new_node')"/>                                  
                </td>
              </tr>
            </table>
            </xsl:if>
            <xsl:if test="$topics_legend">
              <table class="os_table">
                <tr>
                  <td class="os_subtitle" colspan="4">
                    <b>
                      <xsl:value-of select="lang:text('legend.topics.title')" />
                    </b>
                  </td>
                </tr>
                <tr>
                  <td align="center" width="25%">
                    <xsl:call-template name="object-state">
                      <xsl:with-param name="name" select="'topic'"/>
                      <xsl:with-param name="new" select="false()"/>
                      <xsl:with-param name="hot" select="false()"/>
                    </xsl:call-template>
                  </td>
                  <td align="center" width="25%">
                    <xsl:call-template name="object-state">
                      <xsl:with-param name="name" select="'topic'"/>
                      <xsl:with-param name="new" select="true()"/>
                      <xsl:with-param name="hot" select="false()"/>
                    </xsl:call-template>
                  </td>
                  <td align="center" width="25%">
                    <xsl:call-template name="object-state">
                      <xsl:with-param name="name" select="'topic'"/>
                      <xsl:with-param name="new" select="false()"/>
                      <xsl:with-param name="hot" select="true()"/>
                    </xsl:call-template>
                  </td>
                  <td align="center" width="25%">
                    <xsl:call-template name="object-state">
                      <xsl:with-param name="name" select="'topic'"/>
                      <xsl:with-param name="new" select="true()"/>
                      <xsl:with-param name="hot" select="true()"/>
                    </xsl:call-template>
                  </td>
                </tr>
                <tr>
                  <td align="center" width="25%">
                    <xsl:value-of select="lang:text('legend.topics.topic')"/>                    
                  </td>
                  <td align="center" width="25%">
                    <xsl:value-of select="lang:text('legend.topics.topic_new')"/>                    
                  </td>
                  <td align="center" width="25%">
                    <xsl:value-of select="lang:text('legend.topics.topic_hot')"/>                                       
                  </td>
                  <td align="center" width="25%">
                    <xsl:value-of select="lang:text('legend.topics.topic_new_hot')"/>                                      
                  </td>
                </tr>
              </table>
            </xsl:if>          
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template name="forum_last_message">
    <xsl:param name="last_topic"/>
    <xsl:param name="last_message"/>
    <xsl:param name="last_message_date"/>
    <span class="os_lastaction" style="white-space: nowrap;">
      <xsl:choose>
        <xsl:when test="$last_message_date">
          <img src="{system:resource-url('images/last_message.gif')}"/>
          <xsl:text> </xsl:text>
          <xsl:value-of select="date:userfriendly-datetime($last_message_date)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>--</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
      <br/>
      <b>
        <xsl:value-of select="lang:text('forum.viewer.last_message_in')"/>
        <xsl:text>: </xsl:text>
      </b>
      <xsl:choose>
        <xsl:when test="$last_topic">
          <a href="{$last_topic/@view_href}">
            <xsl:value-of select="$last_topic/@title"/>
          </a>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>----</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
      <br/>
      <b>
        <xsl:value-of select="lang:text('forum.viewer.last_message_by')"/>
        <xsl:text>: </xsl:text>
      </b>
      <xsl:choose>
        <xsl:when test="$last_message">
          <xsl:apply-templates select="$last_message/author" mode="row"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>----</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </span>
  </xsl:template>

  <xsl:template name="topic_last_message">
    <xsl:param name="last_message"/>
    <xsl:param name="last_message_date"/>
    <span class="os_lastaction" style="white-space: nowrap;">
      <xsl:choose>
        <xsl:when test="$last_message_date">
          <xsl:value-of select="date:userfriendly-datetime($last_message_date)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>--</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
      <br/>
      <xsl:choose>
        <xsl:when test="$last_message">
          <a href="{$last_message/@view_href}">
            <xsl:value-of select="lang:text('forum.viewer.last_message_post')"/>
          </a>
          <xsl:text>: </xsl:text>
          <xsl:apply-templates select="$last_message/author" mode="row"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="lang:text('forum.viewer.last_message_post')"/>
          <xsl:text>: ----</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </span>
  </xsl:template>

</xsl:stylesheet>