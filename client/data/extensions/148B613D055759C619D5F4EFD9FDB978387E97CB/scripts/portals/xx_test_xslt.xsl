<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="/test">



    <a href="javascript:alert('alfa\%27beta');">test</a>
    <a href="javascript:alert('alfa\&#039;beta');">test</a>
    <br/>




    <h1>Dialog modal</h1>
    <div id="dialog" title="Download complete">
      <p>
        <span class="ui-icon ui-icon-circle-check" style="float:left; margin:0 7px 50px 0;"></span>
        Your files have downloaded successfully into the My Downloads folder.
      </p>
      <p>
        Currently using <b>36% of your storage space</b>.
      </p>
    </div>

    <script type="text/javascript">
      jQuery(function() {
      jQuery("#dialog").dialog({
      bgiframe: true,
      modal: true,
      buttons: {
      Ok: function() {
      jQuery(this).dialog('close');
      }
      }
      });
      });
    </script>

























    <h1>DatePicker test</h1>


    <SCRIPT type="text/javascript">
      jQuery(function() {
      jQuery("#datepicker").datepicker();
      });
    </SCRIPT>



    <DIV class="demo">

      <P>
        Date: <INPUT type="text" id="datepicker"/>
      </P>

    </DIV>
    <!-- End demo -->

    <DIV class="demo-description" style="display: none">

    </DIV>
    <!-- End demo-description -->





  </xsl:template>


</xsl:stylesheet>