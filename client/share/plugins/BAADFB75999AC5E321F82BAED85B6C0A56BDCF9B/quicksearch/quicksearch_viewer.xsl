<!-- getRSS.xsl: retrieve RSS feed(s) and convert to HTML. -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xsl/os" 
                exclude-result-prefixes="lang date system os"
                version="1.0">
  
  <xsl:output method="html"/>

  <xsl:template match="quicksearch">
    <div style="padding:3px;">
      <table>
        <tr>
        	<xsl:variable name="start_text" select="lang:text('assistant.search')"/>
          <td style="width:100%;">            
            <input style="width:100%" id="quick_search" value="{$start_text}" onfocus="javascript:if (this.value == '{$start_text}') this.value = '';" onkeypress="javascript:if (osGetEvent(event).keyCode != 13)  return true; Osiris.Assistant.doSearch('quick_search', '{@search_href}', '{$start_text}', false); return false;"/>
          </td>
          <td style="vertical-align:middle;">
            <img style="padding-left:10px;cursor:pointer;" src="{system:resource-url('images/quick_search.gif')}" onclick="javascript:Osiris.Assistant.doSearch('quick_search', '{@search_href}', '{$start_text}', false);"/>
          </td>
        </tr>
      </table>
      <xsl:if test="//@style='full'">
        <div class="os_assistant_actions">
          <a class="os_button_link" href="{@search_href}">
            <img src="{system:resource-url('images/small/related.gif')}"/>
            <span data-os-tooltip="{lang:text('help.assistant.pages.search.actions.advanced')}">
              <xsl:value-of select="lang:text('assistant.pages.search.actions.advanced')"/>
            </span>
          </a>
        </div>
      </xsl:if>
    </div>
  </xsl:template>

</xsl:stylesheet>
