<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object_actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/rating.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
    
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template name="section">
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="@title"/>
      <xsl:with-param name="href" select="@view_href"/>
      <xsl:with-param name="content"> 
        <table class="os_table_data">
          <xsl:if test="sections">
            <tr>
              <th colspan="2" style="width:55%">
                <xsl:value-of select="lang:text('forum.viewer.section')"/>
              </th>
              <th class="os_center" style="width:10%">
                <xsl:value-of select="lang:text('forum.viewer.discussions')"/>
              </th>
              <th class="os_center" style="width:10%">
                <xsl:value-of select="lang:text('forum.viewer.posts')"/>
              </th>
              <th style="width:25%">
                <xsl:value-of select="lang:text('forum.viewer.last_message')"/>
              </th>
            </tr>
            <xsl:apply-templates select="sections/section" mode="link"/>
          </xsl:if>
          <xsl:if test="forums">
            <tr>
              <th colspan="2" style="width:55%">
                <xsl:value-of select="lang:text('forum.viewer.forum')"/>
              </th>
              <th class="os_center" style="width:10%">
                <xsl:value-of select="lang:text('forum.viewer.discussions')"/>
              </th>
              <th class="os_center" style="width:10%">
                <xsl:value-of select="lang:text('forum.viewer.posts')"/>
              </th>
              <th style="width:25%">
                <xsl:value-of select="lang:text('forum.viewer.last_message')"/>
              </th>
            </tr>
            <xsl:apply-templates select="forums/forum" mode="link"/>
          </xsl:if>
        </table>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="forum">
    <!-- Visualizza i sottoforums -->
    <xsl:if test="forums">
      <xsl:call-template name="block">
        <xsl:with-param name="title">
          <xsl:value-of select="@title"/>
          <xsl:if test="discussions">
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
    
    <!-- Visualizza le discussioni -->
    <xsl:if test="discussions">
      <xsl:call-template name="block">
        <xsl:with-param name="title">
          <xsl:value-of select="@title"/>
          <xsl:if test="forums">
            >
            <xsl:value-of select="lang:text('forum.viewer.forum_discussions')"/>
          </xsl:if>
        </xsl:with-param>
        <xsl:with-param name="content">
          <xsl:apply-templates select="discussions"/>
        </xsl:with-param>
      </xsl:call-template>
      <!-- se ci sono discussioni mostra nuovamente le azioni nella parte sottostante -->
      <!--<xsl:call-template name="forumActions"/>-->
      <xsl:call-template name="objectActions"/>
    </xsl:if>

    <!-- Se non ci sono ne forums ne discussioni mostra solo il titolo del forum -->
<xsl:if test="not(forums) and not(discussions)">
      <xsl:call-template name="block">
        <xsl:with-param name="title">
          <xsl:value-of select="@title"/>
        </xsl:with-param>
        <xsl:with-param name="href" select="@view_href"/>
      </xsl:call-template>
    </xsl:if>

  </xsl:template>
  
  <xsl:template match="forums">
    <table class="os_table_data">
      <tr>
        <th colspan="2" style="width:55%">
          <xsl:value-of select="lang:text('forum.viewer.forum')"/>
        </th>
        <th class="os_center" style="width:10%">
          <xsl:value-of select="lang:text('forum.viewer.discussions')"/>
        </th>
        <th class="os_center" style="width:10%">
          <xsl:value-of select="lang:text('forum.viewer.posts')"/>
        </th>
        <th style="width:25%">
          <xsl:value-of select="lang:text('forum.viewer.last_message')"/>
        </th>
      </tr>
      <xsl:apply-templates select="forum" mode="link"/>
    </table>
  </xsl:template>

  <xsl:template match="discussions">
    <table class="os_table_data" style="clear:left;">
      <tr>
        <th colspan="2" style="width:45%">
          <xsl:value-of select="lang:text('forum.viewer.discussion_title')"/>
        </th>
        <th class="os_center" style="width:10%">
          <xsl:value-of select="lang:text('forum.viewer.discussion_posts')"/>
        </th>
        <th class="os_center" style="width:10%">
          <xsl:value-of select="lang:text('forum.viewer.discussion_author')"/>
        </th>
        <th style="width:10%">
          <xsl:value-of select="lang:text('forum.viewer.discussion_rating')"/>          
        </th>
        <th style="width:25%">
          <xsl:value-of select="lang:text('forum.viewer.last_message')"/>
        </th>
      </tr>
      <xsl:for-each select="discussion[@pinned = 'true']">
        <xsl:apply-templates select="." mode="link"/>
      </xsl:for-each>

      <xsl:if test="discussion[@pinned = 'true'] and discussion[@pinned = 'false']">
        <!-- se ci sono sia discussioni in rilievo che non mostra il separatore -->
        <tr>
          <td class="os_subtitle" colspan="6">
            <b>
              <xsl:value-of select="lang:text('forum.viewer.discussions')"/>
            </b>
          </td>
        </tr>
      </xsl:if>

      <xsl:for-each select="discussion[@pinned = 'false']">
        <xsl:apply-templates select="." mode="link"/>
      </xsl:for-each>
    </table>
  </xsl:template>

  <xsl:template match="section" mode="link">
    <tr data-os-entity="{@entity}">
      <td class="os_center os_middle" style="width:1%">
        <xsl:call-template name="object-state">
          <xsl:with-param name="new" select="date:compare(stats/@last_message_date, system:reference-date()) > 0"/>
          <xsl:with-param name="node" select="boolean(forums)"/>
        </xsl:call-template>        
      </td>
      <td>
        <a href="{@view_href}">
          <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes"/>
        </a>        
        <xsl:if test="@description != ''">
          <br />
          <span class="os_description">
            <xsl:value-of select="system:parse(@description)" disable-output-escaping="yes"/>
          </span>
        </xsl:if>
        <xsl:if test="sections">
          <br />
          <span class="os_description">
            <span class="os_label">
              <xsl:value-of select="lang:text('forum.viewer.subsections')"/>:
            </span>
            <xsl:for-each select="sections/section">
              <a href="{@view_href}">
                <xsl:value-of select="@title"/>
                <xsl:if test="sections">
                  (...)
                </xsl:if>
              </a>
              <xsl:if test="position() != last()">, </xsl:if>
            </xsl:for-each>
          </span>
        </xsl:if>
        
        <xsl:if test="forums">
          <br />
          <span class="os_description">
            <span class="os_label">
              <xsl:value-of select="lang:text('forum.viewer.subforums')"/>:
            </span>
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
      <td class="os_center">
        <xsl:value-of select="stats/@topics"/>
      </td>
      <td class="os_center">
        <xsl:value-of select="stats/@posts"/>
      </td>
      <td>
        <xsl:call-template name="forum_last_message">
          <xsl:with-param name="last_topic" select="stats/last_topic"/>
          <xsl:with-param name="last_message" select="stats/last_message"/>
          <xsl:with-param name="last_message_href" select="stats/@last_entry_href"/>
          <xsl:with-param name="last_message_date" select="stats/@last_message_date"/>
        </xsl:call-template>        
      </td>
    </tr>
  </xsl:template>
  
  <xsl:template match="forum" mode="link">
    <tr data-os-entity="{@entity}">
      <td class="os_center os_middle" style="width:1%">
        <xsl:call-template name="object-state">
          <xsl:with-param name="new" select="date:compare(stats/@last_message_date, system:reference-date()) > 0"/>
          <xsl:with-param name="node" select="boolean(forums)"/>
        </xsl:call-template>
      </td>
      <td>
        
          <a href="{@view_href}">
            <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes"/>
          </a>
        
        <xsl:if test="@description != ''">
          <br />
          <span class="os_description">
            <xsl:value-of select="system:parse(@description)" disable-output-escaping="yes"/>
          </span>
        </xsl:if>
        <xsl:if test="forums">
          <br />
          <span class="os_description">
            <span class="os_label">
              <xsl:value-of select="lang:text('forum.viewer.subforums')"/>:
            </span>
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
      <td class="os_center">
        <xsl:value-of select="stats/@topics"/>
      </td>
      <td class="os_center">
        <xsl:value-of select="stats/@posts"/>
      </td>
      <td>
        <xsl:call-template name="forum_last_message">
          <xsl:with-param name="last_topic" select="stats/last_topic"/>
          <xsl:with-param name="last_message" select="stats/last_message"/>
          <xsl:with-param name="last_message_href" select="stats/@last_entry_href"/>
          <xsl:with-param name="last_message_date" select="stats/@last_message_date"/>
        </xsl:call-template>        
      </td>
    </tr>
  </xsl:template>

  <xsl:template match="discussion" mode="link">
    <tr data-os-entity="{@entity}">
      <td class="os_center os_middle" style="width:1%">
        <xsl:call-template name="object-state">
          <xsl:with-param name="new" select="(date:compare(@entity_date, system:reference-date()) > 0) or (date:compare(stats/@last_entry_date, system:reference-date()) > 0)"/>
          <xsl:with-param name="hot" select="stats/@posts >= system:get-option('ide.objects.hot_state_threshold')"/>
        </xsl:call-template>
      </td>
      <td>
        <div>
          <xsl:choose>
            <xsl:when test="@pinned = 'true'">
              <b>
                <xsl:value-of select="lang:text('forum.viewer.discussion_pinned')"/>
                <xsl:text>: </xsl:text>
                <a href="{@view_href}">
                  <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes"/>
                </a>
              </b>
            </xsl:when>
            <xsl:otherwise>
              <a href="{@view_href}">
                <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes"/>
              </a>
            </xsl:otherwise>
          </xsl:choose>
          <xsl:if test="@description != ''">
            <div class="osForum_TopicDesc">
              <xsl:value-of select="system:parse(@description)" disable-output-escaping="yes"/>              
            </div>
          </xsl:if>
        </div>
      </td>
      <td class="os_center">
        <xsl:value-of select="stats/@posts"/>
      </td>
      <td class="os_center">
        <xsl:apply-templates select="author" mode="row"/>
      </td>
      <td class="os_center">
        <xsl:call-template name="rating-view">
          <xsl:with-param name="votes" select="stats/@votes"/>
          <xsl:with-param name="votes_average" select="stats/@votes_average"/>
        </xsl:call-template>
      </td>
      <td>
        <xsl:call-template name="topic_last_message">
          <xsl:with-param name="last_message" select="stats/last_entry"/>
          <xsl:with-param name="last_message_href" select="stats/@last_entry_href"/>
          <xsl:with-param name="last_message_date" select="stats/@last_entry_date"/>
        </xsl:call-template>
      </td>
    </tr>
  </xsl:template>
  
  <xsl:template name="forum-legend">
    <xsl:param name="forums_legend" select="true()"/>
    <xsl:param name="discussions_legend" select="true()"/>

    <xsl:variable name="show_legend" select="$forums_legend or $discussions_legend"/>
    <xsl:if test="$show_legend">
      <xsl:call-template name="block">
        <xsl:with-param name="title" select="lang:text('legend.title')"/>
        <xsl:with-param name="content">
          <xsl:if test="$forums_legend">
            <table class="os_table_data">
              <tr>
                <td class="os_subtitle" colspan="4">
                  <b>
                    <xsl:value-of select="lang:text('forum.legend.forums.title')" />
                  </b>
                </td>
              </tr>
              <tr>
                <td class="os_center" style="width:25%">
                  <xsl:call-template name="object-state">
                    <xsl:with-param name="type" select="'section'"/>
                    <xsl:with-param name="new" select="false()"/>
                    <xsl:with-param name="node" select="false()"/>
                  </xsl:call-template>
                </td>
                <td class="os_center" style="width:25%">
                  <xsl:call-template name="object-state">
                    <xsl:with-param name="type" select="'section'"/>
                    <xsl:with-param name="new" select="true()"/>
                    <xsl:with-param name="node" select="false()"/>
                  </xsl:call-template>
                </td>
                <td class="os_center" style="width:25%">
                  <xsl:call-template name="object-state">
                    <xsl:with-param name="type" select="'section'"/>
                    <xsl:with-param name="new" select="false()"/>
                    <xsl:with-param name="node" select="true()"/>
                  </xsl:call-template>
                </td>
                <td class="os_center" style="width:25%">
                  <xsl:call-template name="object-state">
                    <xsl:with-param name="type" select="'section'"/>
                    <xsl:with-param name="new" select="true()"/>
                    <xsl:with-param name="node" select="true()"/>
                  </xsl:call-template>
                </td>
              </tr>
              <tr>
                <td class="os_center" style="width:25%">
                  <xsl:value-of select="lang:text('forum.legend.forums.forum')"/>
                </td>
                <td class="os_center" style="width:25%">
                  <xsl:value-of select="lang:text('forum.legend.forums.forum_new')"/>
                </td>
                <td class="os_center" style="width:25%">
                  <xsl:value-of select="lang:text('forum.legend.forums.forum_node')"/>
                </td>
                <td class="os_center" style="width:25%">
                  <xsl:value-of select="lang:text('forum.legend.forums.forum_new_node')"/>
                </td>
              </tr>
            </table>
          </xsl:if>
          <xsl:if test="$discussions_legend">
            <table class="os_table_data">
              <tr>
                <td class="os_subtitle" colspan="4">
                  <b>
                    <xsl:value-of select="lang:text('forum.legend.topics.title')" />
                  </b>
                </td>
              </tr>
              <tr>
                <td class="os_center" style="width:25%">
                  <xsl:call-template name="object-state">
                    <xsl:with-param name="type" select="'text'"/>
                    <xsl:with-param name="new" select="false()"/>
                    <xsl:with-param name="hot" select="false()"/>
                  </xsl:call-template>
                </td>
                <td class="os_center" style="width:25%">
                  <xsl:call-template name="object-state">
                    <xsl:with-param name="type" select="'text'"/>
                    <xsl:with-param name="new" select="true()"/>
                    <xsl:with-param name="hot" select="false()"/>
                  </xsl:call-template>
                </td>
                <td class="os_center" style="width:25%">
                  <xsl:call-template name="object-state">
                    <xsl:with-param name="type" select="'text'"/>
                    <xsl:with-param name="new" select="false()"/>
                    <xsl:with-param name="hot" select="true()"/>
                  </xsl:call-template>
                </td>
                <td class="os_center" style="width:25%">
                  <xsl:call-template name="object-state">
                    <xsl:with-param name="type" select="'text'"/>
                    <xsl:with-param name="new" select="true()"/>
                    <xsl:with-param name="hot" select="true()"/>
                  </xsl:call-template>
                </td>
              </tr>
              <tr>
                <td class="os_center" style="width:25%">
                  <xsl:value-of select="lang:text('forum.legend.topics.topic')"/>
                </td>
                <td class="os_center" style="width:25%">
                  <xsl:value-of select="lang:text('forum.legend.topics.topic_new')"/>
                </td>
                <td class="os_center" style="width:25%">
                  <xsl:value-of select="lang:text('forum.legend.topics.topic_hot')"/>
                </td>
                <td class="os_center" style="width:25%">
                  <xsl:value-of select="lang:text('forum.legend.topics.topic_new_hot')"/>
                </td>
              </tr>
              <tr>
                <td class="os_subtitle" colspan="4">
                  <b>
                    <xsl:value-of select="lang:text('forum.legend.polls.title')" />
                  </b>
                </td>
              </tr>
              <tr>
                <td class="os_center" style="width:25%">
                  <xsl:call-template name="object-state">
                    <xsl:with-param name="type" select="'poll'"/>
                    <xsl:with-param name="new" select="false()"/>
                    <xsl:with-param name="hot" select="false()"/>
                  </xsl:call-template>
                </td>
                <td class="os_center" style="width:25%">
                  <xsl:call-template name="object-state">
                    <xsl:with-param name="type" select="'poll'"/>
                    <xsl:with-param name="new" select="true()"/>
                    <xsl:with-param name="hot" select="false()"/>
                  </xsl:call-template>
                </td>
                <td class="os_center" style="width:25%">
                  <xsl:call-template name="object-state">
                    <xsl:with-param name="type" select="'poll'"/>
                    <xsl:with-param name="new" select="false()"/>
                    <xsl:with-param name="hot" select="true()"/>
                  </xsl:call-template>
                </td>
                <td class="os_center" style="width:25%">
                  <xsl:call-template name="object-state">
                    <xsl:with-param name="type" select="'poll'"/>
                    <xsl:with-param name="new" select="true()"/>
                    <xsl:with-param name="hot" select="true()"/>
                  </xsl:call-template>
                </td>
              </tr>
              <tr>
                <td class="os_center" style="width:25%">
                  <xsl:value-of select="lang:text('forum.legend.polls.poll')"/>
                </td>
                <td class="os_center" style="width:25%">
                  <xsl:value-of select="lang:text('forum.legend.polls.poll_new')"/>
                </td>
                <td class="os_center" style="width:25%">
                  <xsl:value-of select="lang:text('forum.legend.polls.poll_hot')"/>
                </td>
                <td class="os_center" style="width:25%">
                  <xsl:value-of select="lang:text('forum.legend.polls.poll_new_hot')"/>
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
    <xsl:param name="last_message_href"/>    
    <xsl:param name="last_message_date"/>
    <span class="os_lastaction">
        <xsl:value-of select="lang:text('object.contained_by')"/>
        <xsl:text>: </xsl:text>

      <xsl:choose>
        <xsl:when test="$last_topic">
          <a href="{$last_topic/@view_href}" title="{$last_topic/@title}">
            <xsl:call-template name="limit-title">
              <xsl:with-param name="title" select="$last_topic/@title" />
            </xsl:call-template>
          </a>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>----</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
      <br/>
      <xsl:value-of select="lang:text('object.last_post_by')"/>
      <xsl:text>: </xsl:text>
      <xsl:choose>
        <xsl:when test="$last_message">
          <xsl:apply-templates select="$last_message/author" mode="row"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>----</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
      <br />
      <xsl:choose>        
        <xsl:when test="$last_message_date">
          <a class="os_nolink" href="{$last_message_href}">            
            <img src="{system:resource-url('images/last_post.png')}"/>
          </a>
          <xsl:text> </xsl:text>
          <xsl:value-of select="date:userfriendly-datetime($last_message_date)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>--</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </span>
  </xsl:template>

  <xsl:template name="topic_last_message">
    <xsl:param name="last_message"/>
    <xsl:param name="last_message_href"/>
    <xsl:param name="last_message_date"/>
    <span class="os_lastaction">
      <xsl:value-of select="lang:text('object.last_post_by')"/>
      <xsl:text>: </xsl:text>
      <xsl:choose>
        <xsl:when test="$last_message">          
          <xsl:apply-templates select="$last_message/author" mode="row"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>: --</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
      <br/>
      <xsl:choose>
        <xsl:when test="$last_message_date">
          <a class="os_nolink" href="{$last_message_href}">            
            <img src="{system:resource-url('images/last_post.png')}"/>
          </a>
          <xsl:text> </xsl:text>
          <xsl:value-of select="date:userfriendly-datetime($last_message_date)"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:text>--</xsl:text>
        </xsl:otherwise>
      </xsl:choose>
    </span>
  </xsl:template>
  
</xsl:stylesheet>