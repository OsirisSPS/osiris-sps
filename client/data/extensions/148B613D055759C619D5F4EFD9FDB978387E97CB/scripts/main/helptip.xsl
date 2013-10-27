<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="/helptip">

    <div id="helptipdialog" title="{lang:text('help.tip.title')}">
      <div class="os_content">
        <img style="float:right" src="{system:resource-url('images/icons/32x32/help.png')}"/>
        <xsl:value-of select="system:parse(lang:text(concat(@id,'.help')))" disable-output-escaping="yes"/>                                                
      </div>
    </div>

    <script type="text/javascript">
      <![CDATA[
      jQuery(function() 
      {
        jQuery("#helptipdialog").dialog(
        {
          show: 'fade',
          hide: 'fade',
          bgiframe: true,
          modal: true,
          width: '50%',          
          buttons: 
          {
            NoMore: function() 
            {                        
              Osiris.loadUrl("/main/helptip?id=]]><xsl:value-of select="@id"/><![CDATA[&hide=true");
              jQuery(this).dialog('close');
              
            },
            Ok: function() 
            {
              jQuery(this).dialog('close');
            }
          },
          open: function()
			    {                
                jQuery(":button:contains('Ok')").html("]]><xsl:value-of select="lang:text('help.tip.ok')"/><![CDATA[");                
                jQuery(":button:contains('NoMore')").html("]]><xsl:value-of select="lang:text('help.tip.nomore')"/><![CDATA[");                
          }
          
        });
      });
      ]]>
    </script>
    
  </xsl:template>

  
</xsl:stylesheet>