<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
                exclude-result-prefixes="date lang system os"
                version="1.0">

<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>  
<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/user.xsl"/>
<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/results.xsl"/>
<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/common.xsl"/>
<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>

<xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

<xsl:param name="score"/>
<xsl:param name="directReputation_toUser"/>
<xsl:param name="directReputation_fromUser"/>

  <xsl:template match="user">
    <table>
      <tr>
        <td>
          <xsl:call-template name="user-full"/>            

            <xsl:for-each select="actions/action">
              <xsl:call-template name="action-page">
                <xsl:with-param name="prefix" select="'user.actions'"/>
              </xsl:call-template>
            </xsl:for-each>

          <div class="os_content_box">
            Qui, se è già presente il portale dell'utente, click su "Visit".
            Altrimenti, se a livello utente "portal" è a true, link "Subscribe/Follow".
          </div>
        </td>
        <td width="100%">
          <div class="os_tab_left_bodies">
            <xsl:if test="@subpage = 'info'">
              <xsl:call-template name="block">
                <xsl:with-param name="title">
                  <xsl:value-of select="lang:text('user.informations')"/>
                </xsl:with-param>
                <xsl:with-param name="content">
                  <xsl:call-template name="user-informations"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:if>

            <xsl:if test="@subpage = 'invite'">
              <xsl:call-template name="block">
                <xsl:with-param name="title">
                  <xsl:value-of select="lang:text('user.actions.view_invite')"/>
                </xsl:with-param>
                <xsl:with-param name="content">
                  <xsl:call-template name="action-row">
                    <xsl:with-param name="prefix" select="'portal.pages.user.actions'"/>
                    <xsl:with-param name="icon" select="'export'"/>
                    <xsl:with-param name="name" select="'export_link'"/>
                    <xsl:with-param name="href" select="@export_href"/>
                  </xsl:call-template>
                  <xsl:call-template name="action-row">
                    <xsl:with-param name="prefix" select="'portal.pages.user.actions'"/>
                    <xsl:with-param name="icon" select="'invite'"/>
                    <xsl:with-param name="name" select="'osiris_invite_link'"/>
                    <xsl:with-param name="href">
                      <xsl:call-template name="copy-paste-link">
                        <xsl:with-param name="text" select="@subscribe_href"/>
                      </xsl:call-template>
                    </xsl:with-param>
                  </xsl:call-template>
                  <xsl:call-template name="action-row">
                    <xsl:with-param name="prefix" select="'portal.pages.user.actions'"/>
                    <xsl:with-param name="icon" select="'invite'"/>
                    <xsl:with-param name="name" select="'isis_invite_link'"/>
                    <xsl:with-param name="href">
                      <xsl:call-template name="copy-paste-link">
                        <xsl:with-param name="text" select="@isis_subscribe_href"/>
                      </xsl:call-template>
                    </xsl:with-param>
                  </xsl:call-template>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:if>

            <xsl:if test="@subpage = 'reputations'">
              <xsl:call-template name="block">
                <xsl:with-param name="title">
                  <xsl:value-of select="lang:text('user.actions.view_reputations')"/>
                </xsl:with-param>
                <xsl:with-param name="content">
                  <div style="display:none" data-os-otype="tab">
                    <xsl:if test="$score">
                      <div data-os-tabType="header" data-os-tabPage="0">
                        <xsl:value-of select="lang:text('user.actions.change_reputation')"/>
                      </div>
                    </xsl:if>
                    <xsl:if test="reputation_forward">
                      <div data-os-tabType="header" data-os-tabPage="1">
                        <xsl:value-of select="lang:text('user.informations.reputations.forward')"/>
                      </div>
                    </xsl:if>
                    <xsl:if test="reputation_backward">
                      <div data-os-tabType="header" data-os-tabPage="2">
                        <xsl:value-of select="lang:text('user.informations.reputations.backward')"/>
                      </div>
                    </xsl:if>
                    <div data-os-tabType="header" data-os-tabPage="3">
                      <xsl:value-of select="lang:text('user.informations.reputations.direct_to')"/>
                    </div>
                    <div data-os-tabType="header" data-os-tabPage="4">
                      <xsl:value-of select="lang:text('user.informations.reputations.direct_from')"/>
                    </div>
                    <xsl:if test="$score">
                      <div data-os-tabType="body" data-os-tabPage="0">
                        <div class="os_content">
                          <xsl:value-of select="$score" disable-output-escaping="yes"/>
                        </div>
                      </div>
                    </xsl:if>
                    <xsl:if test="reputation_forward">
                      <div data-os-tabType="body" data-os-tabPage="1">
                        <div class="os_content">
                          <xsl:apply-templates select="reputation_forward"/>
                        </div>
                      </div>
                    </xsl:if>
                    <xsl:if test="reputation_backward">
                      <div data-os-tabType="body" data-os-tabPage="2">
                        <div class="os_content">
                          <xsl:apply-templates select="reputation_backward"/>
                        </div>
                      </div>
                    </xsl:if>
                    <div data-os-tabType="body" data-os-tabPage="3">
                      <div class="os_content">
                        <xsl:value-of select="$directReputation_toUser" disable-output-escaping="yes"/>
                      </div>
                    </div>
                    <div data-os-tabType="body" data-os-tabPage="4">
                      <div class="os_content">
                        <xsl:value-of select="$directReputation_fromUser" disable-output-escaping="yes"/>
                      </div>
                    </div>
                  </div>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:if>

            <xsl:if test="@subpage = 'search'">
              <xsl:call-template name="block">
                <xsl:with-param name="title">
                  <xsl:value-of select="lang:text('user.actions.view_search')"/>
                </xsl:with-param>
                <xsl:with-param name="content">
                  <xsl:call-template name="search"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:if>

          </div>

        </td>
      </tr>
    </table>

  </xsl:template>

  <xsl:template match="reputation_forward">
      <xsl:call-template name="reputations"/>
  </xsl:template>

  <xsl:template match="reputation_backward">
      <xsl:call-template name="reputations"/>
  </xsl:template>
  
  <xsl:template name="reputations">
    <table>
      <tr>
        <td>
          <div style="display:block; width:100px; border:1px solid gray; margin:2px; padding:8px; background-color: white; text-align:center;font-size:16pt;">            
            <img src="{system:resource-url(concat('images/reputation_score/line/',reputation/score/@value,'.png'))}" title="{reputation/score/@value}"/>
            <br/>
            <br/>
            <xsl:value-of select="reputation/score/@value"/>
          </div>
        </td>
        <td width="100%">
          <ul>
            <xsl:apply-templates select="reputation"/>            
          </ul>    
        </td>
      </tr>
    </table>    
  </xsl:template>

  <xsl:template match="reputation">
    
    <li class="os_reputation_{score/@type}" style="list-style-image:url({system:resource-url(concat('images/reputation_score/dot/',score/@value,'.png'))})">
      <xsl:apply-templates select="from"/>
      <span class="os_label">
        <xsl:text> </xsl:text>
        <xsl:value-of select="lang:text('user.reputation_versus')"/>
        <xsl:text> </xsl:text>
      </span>
      <xsl:apply-templates select="to"/>
      <span class="os_label">
        <xsl:text>, </xsl:text>
        <xsl:value-of select="lang:text('reputation.score')"/>
        <xsl:text>: </xsl:text>
      </span>
      <span class="os_value">
        <xsl:value-of select="score/@value"/>
      </span>
      <span class="os_label">
        <xsl:text>, </xsl:text>
        <xsl:value-of select="lang:text('reputation.comment')"/>
        <xsl:text>: </xsl:text>
      </span>
      <span class="os_value">
        <xsl:value-of select="score/@comment"/>
      </span>
      <ul>
        <xsl:apply-templates select="reputation"/>
      </ul>
    </li>
  </xsl:template>

  <xsl:template match="//reputation/from">
    <xsl:call-template name="user-row"/>
  </xsl:template>
  <xsl:template match="//reputation/to">
    <xsl:call-template name="user-row"/>
  </xsl:template>

  <xsl:template name="user-informations">    
    <div class="os_content_box">      
      <xsl:value-of select="system:parse(@misc)" disable-output-escaping="yes"/>
      <xsl:if test="@mark">
        <div class="os_signature">
          <xsl:value-of select="system:parse(@mark)" disable-output-escaping="yes"/>
        </div>
      </xsl:if>
    </div>
    
    <table class="os_table_properties">
      <xsl:call-template name="row-information">
        <xsl:with-param name="name" select="'user.join_date'"/>
        <xsl:with-param name="value" select="date:long-datetime(@join_date)"/>
      </xsl:call-template>
      <xsl:call-template name="row-information">
        <xsl:with-param name="name" select="'user.gender'"/>
        <xsl:with-param name="value">
          <xsl:choose>
            <xsl:when test="@gender = 0">
              <xsl:value-of select="lang:text('users.users.unspecified')"/>
            </xsl:when>
            <xsl:when test="@gender = 1">
              <xsl:value-of select="lang:text('users.gender_male')"/>
            </xsl:when>
            <xsl:when test="@gender = 2">
              <xsl:value-of select="lang:text('users.gender_female')"/>
            </xsl:when>
          </xsl:choose>
        </xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="row-information">
        <xsl:with-param name="name" select="'user.birth_date'"/>
        <xsl:with-param name="value" select="date:long-date(@birth_date)"/>
      </xsl:call-template>
      <xsl:call-template name="row-information">
        <xsl:with-param name="name" select="'user.language'"/>
        <xsl:with-param name="value" select="@language"/>
      </xsl:call-template>
      <xsl:call-template name="row-information">
        <xsl:with-param name="name" select="'user.location'"/>
        <xsl:with-param name="value" select="@location"/>
      </xsl:call-template>
      <xsl:call-template name="row-information">
        <xsl:with-param name="name" select="'user.time_offset'"/>
        <xsl:with-param name="value" select="lang:text(concat('timeTable_GMT_', @time_offset))"/>
      </xsl:call-template>
      <xsl:call-template name="row-information">
        <xsl:with-param name="name" select="'user.email'"/>
        <xsl:with-param name="content">
          <xsl:if test="@email">
            <a href="mailto:{@email}">
              <xsl:value-of select="@email"/>
            </a>
          </xsl:if>
        </xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="row-information">
        <xsl:with-param name="name" select="'user.website'"/>
        <xsl:with-param name="content">
          <xsl:if test="@website">
            <xsl:value-of select="system:parse(concat('[url]',@website,'[/url]'))" disable-output-escaping="yes"/>
          </xsl:if>
        </xsl:with-param>

      </xsl:call-template>      

      <tr>
        <td class="os_separator" colspan="2"/>
      </tr>
      <xsl:call-template name="row-information">
        <xsl:with-param name="name" select="'user.signature'"/>
        <xsl:with-param name="content">
          
        </xsl:with-param>
      </xsl:call-template>
      </table>

    <xsl:if test="@misc">
          <div class="os_separator" colspan="2"/>
        
          <div class="os_content_box">
            <xsl:value-of select="system:parse(@misc)" disable-output-escaping="yes"/>
          </div>        
      </xsl:if>
      
        <div class="os_separator" colspan="2"/>
      
        <div class="os_content_box" style="font-size:0.8em">
          <span class="os_label">
            <xsl:value-of select="lang:text('user.id')"/>
            <xsl:text> : </xsl:text>
          </span>
          <span class="os_value">
            <xsl:call-template name="hash">
              <xsl:with-param name="hash" select="@id"/>
            </xsl:call-template>            
          </span>
          <span class="os_label">
            <xsl:text> - </xsl:text>
            <xsl:value-of select="lang:text('user.publickey')"/>
            <xsl:text> : </xsl:text>
          </span>
          <xsl:call-template name="copy-paste">
            <xsl:with-param name="body" select="@publickey"/>
          </xsl:call-template>
        </div>
      
  </xsl:template>

  <xsl:template name="search">
    <xsl:call-template name="action-row">
      <xsl:with-param name="prefix" select="'portal.pages.user.actions'"/>
      <xsl:with-param name="icon" select="'search'"/>
      <xsl:with-param name="name" select="'search1'"/>
      <xsl:with-param name="href" select="@href_search1"/>
    </xsl:call-template>

    <xsl:call-template name="action-row">
      <xsl:with-param name="prefix" select="'portal.pages.user.actions'"/>
      <xsl:with-param name="icon" select="'search'"/>
      <xsl:with-param name="name" select="'search2'"/>
      <xsl:with-param name="href" select="@href_search2"/>
    </xsl:call-template>

    <xsl:call-template name="action-row">
      <xsl:with-param name="prefix" select="'portal.pages.user.actions'"/>
      <xsl:with-param name="icon" select="'search'"/>
      <xsl:with-param name="name" select="'search4'"/>
      <xsl:with-param name="href" select="@href_search4"/>
    </xsl:call-template>

    <xsl:call-template name="action-row">
      <xsl:with-param name="prefix" select="'portal.pages.user.actions'"/>
      <xsl:with-param name="icon" select="'search'"/>
      <xsl:with-param name="name" select="'search3'"/>
      <xsl:with-param name="href" select="@href_search3"/>
    </xsl:call-template>

    <xsl:call-template name="action-row">
      <xsl:with-param name="prefix" select="'portal.pages.user.actions'"/>
      <xsl:with-param name="icon" select="'search'"/>
      <xsl:with-param name="name" select="'search5'"/>
      <xsl:with-param name="href" select="@href_search5"/>
    </xsl:call-template>

    <xsl:call-template name="action-row">
      <xsl:with-param name="prefix" select="'portal.pages.user.actions'"/>
      <xsl:with-param name="icon" select="'search'"/>
      <xsl:with-param name="name" select="'search6'"/>
      <xsl:with-param name="href" select="@href_search6"/>
    </xsl:call-template>    
  </xsl:template>  
  
</xsl:stylesheet>