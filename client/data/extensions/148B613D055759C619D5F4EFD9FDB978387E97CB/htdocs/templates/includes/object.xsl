<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system"                 
                exclude-result-prefixes="os date lang system"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/user.xsl" />
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl" />
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/rating.xsl"/>  
    
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>
  
  <xsl:template match="object" mode="lite">
    <xsl:call-template name="object-lite"/>
  </xsl:template>

  <xsl:template match="object" mode="revision">
    <xsl:call-template name="object-revision"/>
  </xsl:template>

  <xsl:template match="object" mode="preview">
    <xsl:call-template name="object-lite"/>
  </xsl:template>

  <xsl:template match="object" mode="table">
    <xsl:call-template name="object-table" />
  </xsl:template>

  <xsl:template match="object" mode="row">
    <xsl:call-template name="object-row"/>
  </xsl:template>

  <xsl:template match="object" mode="row-detail">
    <xsl:call-template name="object-row-detail"/>
  </xsl:template>

  <xsl:template match="object" mode="row-tech">
    <xsl:call-template name="object-row-tech"/>
  </xsl:template>

  <xsl:template match="object" mode="blog">
    <xsl:call-template name="object-blog"/>
  </xsl:template>

  <xsl:template match="object" mode="box-tech">
    <xsl:call-template name="object-box-tech"/>
  </xsl:template>


  <xsl:template match="object">
    <!-- TODO: chiamare templates specifici in base al tipo -->
    <xsl:value-of select="@content" />
  </xsl:template>

  <xsl:template name="object-container">
    <xsl:param name="content" />
		<div data-os-entity="{@entity}">
      <xsl:attribute name="class">
        <xsl:if test="@showable = 'false'">
          <xsl:text> os_object_hidden </xsl:text>
        </xsl:if>
        <xsl:if test="@isResult = 'false'">
          <xsl:text> os_object_lowlight </xsl:text>
        </xsl:if>
      </xsl:attribute>
      <xsl:copy-of select="$content"/>
    </div>		
  </xsl:template>
  

  <!-- Modalità Preview -->
  <!-- Rappresentazione generica dell'oggetto come entità -->  
  
  <xsl:template name="object-lite">
    <xsl:param name="childs" select="true()"/>
    <xsl:call-template name="object-container">
      <xsl:with-param name="content">
        <div class="os_object_lite">
          <div style="width:50px;float:left;text-align:center;">
            <a class="os_nolink" href="{@view_href}">
              <xsl:call-template name="object-icon"/>              
            </a>
          </div>
          <div style="overflow:auto">
            <xsl:call-template name="details-right"/>
            <xsl:if test="@title">
              <span class="os_object_title">
                <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes" />
              </span>
            </xsl:if>
            <span class="os_object_type">
              <xsl:text> (</xsl:text>
              <xsl:call-template name="object-type"/>
              <xsl:text>)</xsl:text>
            </span>
            <xsl:if test="@entity_date">
              <xsl:call-template name="separator"/>
              <span class="os_nowrap">
                <span class="os_label">
                  <xsl:value-of select="lang:text('object.entity_date')"/>
                  <xsl:text>: </xsl:text>
                </span>
                <span class="os_value">
                  <xsl:value-of select="date:short-datetime(@entity_date)"/>
                </span>
              </span>
            </xsl:if>
            <xsl:if test="@revisioned">
              <xsl:call-template name="separator"/>
              <span class="os_nowrap">
                <span class="os_label">
                  <xsl:value-of select="lang:text('object.editor_date')"/>
                  <xsl:text>: </xsl:text>
                </span>
                <span class="os_value">
                  <xsl:value-of select="date:short-datetime(@submit_date)"/>
                </span>
              </span>
            </xsl:if>            
            <xsl:if test="@relevance">
              <xsl:call-template name="separator"/>
              <span class="os_nowrap">
                <span class="os_label">
                  <xsl:value-of select="lang:text('object.relevance')"/>
                  <xsl:text>: </xsl:text>
                </span>
                <span class="os_value">
                  <xsl:value-of select="@relevance" />%
                </span>
              </span>
            </xsl:if>
            <xsl:if test="author">
              <xsl:call-template name="separator"/>
              <xsl:call-template name="authors-row"/>
            </xsl:if>
            <div class="os_object_description">
              <xsl:value-of select="system:parse(@description)" disable-output-escaping="yes"/>
            </div>
            <xsl:if test="@content != ''">
              <div class="os_object_lite_content">
                <xsl:value-of select="system:parse(@content, false(), false(), false(), 0, @entity, @view_href)"  disable-output-escaping="yes" />
              </div>
            </xsl:if>            
          </div>
          <div style="clear:both"></div>
        </div>
        
      </xsl:with-param>
    </xsl:call-template>    
      
    <!-- Childs (Tree Mode) -->
    <xsl:if test="$childs">
      <xsl:if test="objects/*">
        <div class="os_object_lite_childs">
          <xsl:apply-templates select="objects/object" mode="lite"/>
        </div>
      </xsl:if>
    </xsl:if>
  </xsl:template>

  <!-- Modalità Revision -->
  <!-- Rappresentazione generica dell'oggetto come entry -->
  <xsl:template name="object-revision">
    <div data-os-entity="{@entity}" class="os_object_lite">
      <xsl:if test="@title">
        <span class="os_object_title">
          <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes" />
        </span>
      </xsl:if>
      <span class="os_object_type">
        <xsl:text> (</xsl:text>
        <xsl:call-template name="object-type"/>
        <xsl:text>)</xsl:text>
      </span>
      <xsl:text> </xsl:text>
      <span class="os_nowrap">
        <span class="os_label">
          <xsl:value-of select="lang:text('object.entity_date')"/>
          <xsl:text>: </xsl:text>
        </span>
        <span class="os_value">
          <xsl:choose>
            <xsl:when test="@revisioned">
              <xsl:value-of select="date:short-datetime(@submit_date)"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="date:short-datetime(@entity_date)"/>
            </xsl:otherwise>
          </xsl:choose>
        </span>
      </span>
      <xsl:text> </xsl:text>
      <xsl:call-template name="editor-row"/>
      <xsl:if test="@content != ''">
        <div class="os_object_lite_content">
          <xsl:value-of select="system:parse(@content, false(), false(), false(), 0, @entity, @view_href)"  disable-output-escaping="yes" />
        </div>
      </xsl:if>

      <xsl:if test="@comment">
        <hr/>
        <div>
          <xsl:value-of select="system:parse(@comment)"  disable-output-escaping="yes" />
        </div>
      </xsl:if>

      <div style="float:right;">
        <xsl:if test="actions/action[@name = 'restore']">
          <a class="os_button" href="{actions/action[@name = 'restore']/@href}">Restore</a>
        </xsl:if>
      </div>

      <div style="clear:both"></div>
    </div>
  </xsl:template>

  <!-- Modalità Table -->

  <xsl:template name="object-table">
    <xsl:param name="indent" select="0"/>
    
    <tr data-os-entity="{@entity}">
      
      <td style="padding-left:{$indent*10}px">
        <xsl:call-template name="object-container">
          <xsl:with-param name="content">											
						<table class="os_table_none">
              <tr>
                <td>
                  <xsl:call-template name="object-icon"/>
                </td>
                <td width="100%">
									<a href="{@view_href}">
                    <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes" />
                  </a>
                  <xsl:if test="@description != ''">
                    <div class="os_object_description">
                      <xsl:value-of select="system:parse(@description)" disable-output-escaping="yes"/>
                    </div>
                  </xsl:if>																		
								</td>
              </tr>
            </table>            						
						
          </xsl:with-param>
        </xsl:call-template>
      </td>
      <td align="center">
        <span class="os_nowrap">
          <xsl:value-of select="date:userfriendly-datetime(@entity_date)"/>
        </span>
      </td>
      <td align="center">
        <xsl:apply-templates select="author" mode="row"/>
      </td>
      <td align="center">
        <xsl:if test="stats/@votes">
          <xsl:call-template name="rating-view">
            <xsl:with-param name="votes" select="stats/@votes"/>
            <xsl:with-param name="votes_average" select="stats/@votes_average"/>
          </xsl:call-template>
        </xsl:if>
      </td>
      <td>
        <xsl:if test="stats/@last_entry_date">
          <span class="os_nowrap">
            <xsl:value-of select="stats/@posts"/>
            <xsl:text> </xsl:text>
            <xsl:value-of select="lang:text('object.posts.lower')"/>
            <xsl:text>, </xsl:text>
            <xsl:value-of select="lang:text('object.last_post_by_lower')"/>
            <xsl:text>: </xsl:text>
            <xsl:apply-templates select="stats/last_entry/author" mode="row"/>
            <br />
            <a class="os_nolink" href="{stats/@last_entry_href}">
              <img src="{system:resource-url('images/last_post.png')}"/>
            </a>
            <xsl:text> </xsl:text>
            <xsl:value-of select="date:userfriendly-datetime(stats/@last_entry_date)"/>
          </span>
        </xsl:if>
      </td>
    </tr>

    <!-- Childs (Tree Mode) -->
    <xsl:if test="objects/*">
      <xsl:for-each select="objects/object">
        <xsl:call-template name="object-table">
          <xsl:with-param name="indent" select="$indent+1"/>
        </xsl:call-template>
      </xsl:for-each>
    </xsl:if>
</xsl:template>

  <!-- Modalità Row -->

  <xsl:template name="object-row">
    <xsl:call-template name="object-container">
      <xsl:with-param name="content">
        <div class="os_object_row_title">
          <a href="{@view_href}">
            <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes" />
          </a>

        </div>
      </xsl:with-param>
    </xsl:call-template>    

    <!-- Childs (Tree Mode) -->
    <xsl:if test="objects/*">
      <div class="os_object_row_childs">
        <xsl:apply-templates select="objects/object" mode="row"/>
      </div>
    </xsl:if>
  </xsl:template>

  <!-- Modalità Row - Detail -->

  <xsl:template name="object-row-detail">
    <xsl:call-template name="object-container">
      <xsl:with-param name="content">
        <div class="os_object_rowdetail">          
          <div class="os_object_title">
            <a href="{@view_href}">
              <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes" />
            </a>
          </div>          
          <div class="os_object_description">
            <xsl:value-of select="system:parse(@description)" disable-output-escaping="yes"/>
          </div>
          <xsl:call-template name="details-right"/>
          <div class="os_clear" />
        </div>
      </xsl:with-param>
    </xsl:call-template>

    <!-- Childs (Tree Mode) -->
    <xsl:if test="objects/*">
      <div class="os_object_row_childs">
        <xsl:apply-templates select="objects/object" mode="row"/>
      </div>
    </xsl:if>
  </xsl:template>

  <!-- Modalità Row - Tech -->

  <xsl:template name="object-row-tech">
    <xsl:call-template name="object-container">
      <xsl:with-param name="content">
        <div class="os_object_rowtech">
          <div style="float:left">
            <xsl:call-template name="object-icon">
              <xsl:with-param name="size" select="32"/>
            </xsl:call-template>
          </div>
          <div style="float:right;padding:3px;">
            <a class="os_button_link" href="{@view_href}">
              <img src="{system:resource-url('images/icons/16x16/view.png')}"/>
            </a>
            <a class="os_button_link" href="{@edit_href}">
              <img src="{system:resource-url('images/icons/16x16/edit.png')}"/>
            </a>
          </div>
          <a href="{@view_href}">
            <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes" />
          </a>
          <span class="os_object_type">
            <xsl:text> (</xsl:text>
            <xsl:call-template name="object-type"/>
            <xsl:text>)</xsl:text>
          </span>
          <xsl:if test="author">
            <br />
            <xsl:call-template name="authors-row"/>
          </xsl:if>
          <div style="clear:both"/>
        </div>
          
      </xsl:with-param>
    </xsl:call-template>

    <!-- Childs (Tree Mode) -->
    <xsl:if test="objects/*">
      <div class="os_object_row_childs">
        <xsl:apply-templates select="objects/object" mode="row"/>
      </div>
    </xsl:if>
  </xsl:template>

  <!-- Modalità Blog -->

  <xsl:template name="object-blog">
    <xsl:param name="childs" select="true()"/>
    <xsl:call-template name="object-container">
      <xsl:with-param name="content">
        <div class="os_object_blog">
          <div class="os_object_blog_title">
            <h1>
              <a href="{@view_href}">
                <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes" />
              </a>
            </h1>
          </div>
          <div class="os_object_description">
            <xsl:value-of select="system:parse(@description)" disable-output-escaping="yes"/>
          </div>
          <div class="os_object_blog_details_left">
            <xsl:value-of select="lang:text('object.blog.author')"/>
            <xsl:text> </xsl:text>
            <xsl:apply-templates select="author" mode="row"/>
            <!--
        <xsl:text> </xsl:text>        
        <xsl:value-of select="lang:text('object.blog.where')"/>
        <xsl:text> </xsl:text>
        <xsl:text>Prova</xsl:text>
        -->
            <xsl:text> , </xsl:text>
            <xsl:value-of select="date:userfriendly-datetime(@entity_date)"/>
          </div>
          <xsl:call-template name="details-right"/>
          <div class="os_clear" />
          <xsl:if test="@content != ''">
            <div class="os_object_blog_content">
              <xsl:value-of select="system:parse(@content, false(), false(), false(), 0, @entity, @view_href)"  disable-output-escaping="yes" />
            </div>
          </xsl:if>
        </div>
        <!--
        <div class="os_separator"></div>
        -->
      </xsl:with-param>
    </xsl:call-template>

    <!-- Childs (Tree Mode) -->
    <xsl:if test="$childs">
      <xsl:if test="objects/*">
        <div class="os_object_blog_childs">
          <xsl:apply-templates select="objects/object" mode="blog"/>
        </div>
      </xsl:if>
    </xsl:if>

  </xsl:template>

  <!-- Modalità Box-Tech -->

  <xsl:template name="object-box-tech">
    <xsl:param name="childs" select="true()"/>
    <xsl:call-template name="object-container">
      <xsl:with-param name="content">
          <div style="float:right">
            <xsl:call-template name="object-icon"/>
          </div>
          <table class="os_table_properties">
            <xsl:call-template name="row-information">
              <xsl:with-param name="name" select="'object.title'"/>
              <xsl:with-param name="content">
                <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes" />
              </xsl:with-param>              
            </xsl:call-template>
            <xsl:call-template name="row-information">
              <xsl:with-param name="name" select="'object.type'"/>
              <xsl:with-param name="value">
                <xsl:call-template name="object-type"/>
              </xsl:with-param>
            </xsl:call-template>
            <xsl:if test="author">
              <xsl:call-template name="row-information">
                <xsl:with-param name="name" select="'object.author'"/>
                <xsl:with-param name="content">
                  <xsl:apply-templates select="author" mode="row"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:if>
            <xsl:if test="@entity_date">              
              <xsl:call-template name="row-information">
                <xsl:with-param name="name" select="'object.entity_date'"/>
                <xsl:with-param name="value" select="date:short-datetime(@entity_date)"/>                
              </xsl:call-template>
            </xsl:if>
            <xsl:if test="editor">
              <xsl:call-template name="row-information">
                <xsl:with-param name="name" select="'object.editor'"/>
                <xsl:with-param name="content">
                  <xsl:apply-templates select="editor" mode="row"/>
                </xsl:with-param>
              </xsl:call-template>
            </xsl:if>
            <xsl:if test="@revisioned">
              <xsl:call-template name="row-information">
                <xsl:with-param name="name" select="'object.submit_date'"/>
                <xsl:with-param name="value" select="date:short-datetime(@submit_date)"/>
              </xsl:call-template>
            </xsl:if>           
          </table>
          <div class="os_commands_right">
            <xsl:if test="@entity != ''">
              <xsl:call-template name="copy-paste">
                <xsl:with-param name="title" select="'common.labels.id'"/>
                <xsl:with-param name="body" select="@entity"/>
              </xsl:call-template>
            </xsl:if>
            <xsl:call-template name="copy-paste">
              <xsl:with-param name="title" select="'object.actions.oml.link'"/>
              <xsl:with-param name="body" select="@oml_code"/>
            </xsl:call-template>            
          </div>
          <div style="clear:both"/>
        
      </xsl:with-param>
    </xsl:call-template>

    <!-- Childs (Tree Mode) -->
    <xsl:if test="$childs">
      <xsl:if test="objects/*">
        <div style="margin-left:10px;">
          <xsl:apply-templates select="objects/object" mode="box-tech"/>
        </div>
      </xsl:if>
    </xsl:if>

  </xsl:template>



  <!-- Template vari di aiuto -->

  <xsl:template name="details-right">
    <xsl:if test="stats/@posts">
      <div class="os_object_blog_details_right">
        <xsl:value-of select="stats/@posts"/>
        <xsl:text> </xsl:text>
        <xsl:value-of select="lang:text('object.blog.comments')"/>
      </div>
    </xsl:if>
    <xsl:if test="stats/@votes">
      <div class="os_object_blog_details_right">
        <xsl:call-template name="rating-view">
          <xsl:with-param name="votes" select="stats/@votes"/>
          <xsl:with-param name="votes_average" select="stats/@votes_average"/>
        </xsl:call-template>
      </div>
    </xsl:if>
    <xsl:if test="@relevance">      
      <div class="os_object_blog_details_right">
        <span class="os_label">
          <xsl:value-of select="lang:text('object.relevance')"/>
          <xsl:text>: </xsl:text>
        </span>
        <span class="os_value">
          <xsl:value-of select="@relevance" />%
        </span>
      </div>
    </xsl:if>
  </xsl:template>

  <xsl:template name="object-icon">
    <!-- Restituisce un'immagine per un'esportazione lite di un oggetto -->
    <xsl:param name="object" select="."/>
    <xsl:param name="size" select="48"/>
    <xsl:variable name="typedesc" select="lang:text(concat('objects.types.', $object/@type))"/>
    <img data-os-tooltip="{$typedesc}" title="{$typedesc}" width="{$size}">      
      <xsl:attribute name="src">
        <xsl:choose>
          <xsl:when test="$object/@entity=''">            
            <xsl:value-of select="system:resource-url('images/objects/special_root.png')"/>
          </xsl:when>
          <xsl:when test="$object/@entity='0000000000000000000000000000000000000001'">
            <xsl:value-of select="system:resource-url('images/objects/special_recursive.png')"/>
          </xsl:when>
          <xsl:when test="$object/@entity='0000000000000000000000000000000000000002'">
            <xsl:value-of select="system:resource-url('images/objects/special_orphans.png')"/>
          </xsl:when>
          <xsl:when test="$object/@entity='0000000000000000000000000000000000000003'">
            <xsl:value-of select="system:resource-url('images/objects/special_skipped.png')"/>
          </xsl:when>          
          <xsl:otherwise>
            <xsl:value-of select="system:resource-url(concat('images/objects/', $object/@type, '.png'))"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
    </img>
  </xsl:template>

  <xsl:template name="object-type">
    <xsl:value-of select="lang:text(concat('objects.types.', @type))" />
    <xsl:if test="@component_name">
      <xsl:text>, </xsl:text>
      <xsl:value-of select="@component_name"/>
      
    </xsl:if>
    <xsl:if test="@module_name">
      <xsl:text>, </xsl:text>
      <xsl:value-of select="@module_name"/>      
    </xsl:if>
    
  </xsl:template>

  <xsl:template name="object-state">
    <!-- Restituisce un'immagine specifica per lo stato di un oggetto -->
    <!-- Il nome del file è calcolato come "type + <_new> + <_hot> + <_node> + .gif" -->

    <!-- nome di riferimento dell'oggetto (se non specificato viene ricavato dal nome del nodo) -->
    <xsl:param name="type" select="@type"/>
    <!-- indica se è un nuovo oggetto -->
    <xsl:param name="new" select="false()"/>
    <!-- indica se è un oggetto "molti figli" -->
    <xsl:param name="hot" select="false()"/>
    <!-- indica se è un oggetto con sottofigli dello stesso tipo -->
    <xsl:param name="node" select="false()"/>
    <!-- stile da applicare all'immagine -->
    <xsl:param name="style"/>

		<xsl:variable name="suffix">
			<xsl:if test="$new">
				<xsl:value-of select="'_new'"/>
			</xsl:if>
			<xsl:if test="$hot">
				<xsl:value-of select="'_hot'"/>
			</xsl:if>
			<xsl:if test="$node">
				<xsl:value-of select="'_node'"/>
			</xsl:if>
		</xsl:variable>

    <xsl:variable name="src">
      <xsl:value-of select="'images/states/'"/>
      <xsl:value-of select="$type"/>
			<xsl:value-of select="$suffix"/>
      <xsl:value-of select="'.gif'"/>
    </xsl:variable>    
    <img src="{system:resource-url(string($src))}" data-os-tooltip="{$suffix}">
      <xsl:if test="$style">
        <xsl:attribute name="style">
          <xsl:value-of select="$style"/>
        </xsl:attribute>
      </xsl:if>
    </img>
  </xsl:template>

  <!--
  <xsl:template name="object-actions">
    <xsl:param name="object" select="."/>
    <a class="os_button_micro os_nolink" href="{$object/@view_href}">
      <img title="{lang:text('object.actions.view')}" src="{system:resource-url('images/small/view.gif')}"/>
    </a>
    <xsl:text> </xsl:text>
    <a class="os_button_micro os_nolink" href="{$object/@edit_href}">
      <img title="{lang:text('object.actions.edit')}" src="{system:resource-url('images/small/edit.gif')}"/>
    </a>
    <xsl:text> </xsl:text>
    <a class="os_button_micro os_nolink" href="{$object/@browse_href}">
      <img title="{lang:text('object.actions.browse')}" src="{system:resource-url('images/small/browse.gif')}"/>
    </a>    
  </xsl:template>
  -->

</xsl:stylesheet>
