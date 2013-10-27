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

  <xsl:template match="/assistant">
    <div id="assistant_dialog" title="TODO title assistant">
      <div class="os_content">
        bla bla bla
      </div>
    </div>

    <script type="text/javascript">
      <![CDATA[
      alert('xx');
      jQuery(function() 
      {
        jQuery("#assistant_dialog").dialog(
        {
          show: 'fade',
          hide: 'fade',
          bgiframe: true,
          modal: false,
          width: '50%'
        });
      });
      ]]>
    </script>
    
  </xsl:template>

  
</xsl:stylesheet>