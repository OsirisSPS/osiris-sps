import os
import osiris
import re

import core

##############################################
# Name: YouTube
##############################################

class HtmlYouTube(core.ISnippet):	
	code = "youtube"
	name = "YouTube"
	category = ""
	url = "http://www.youtube.com"
	security = "iframe"
	reload = False
	action = "ask"
	notes = False
	debug = False
	matchRe = ["\\<iframe width\\=\"{@number}\" height\\=\"{@number}\" src\\=\"(http|https)\\://www\\.(youtube|youtube\-nocookie)\\.com/embed/[a-zA-Z0-9]*[^\"'<>]*\" frameborder\\=\"0\" allowfullscreen\\>\\</iframe\\>"]


##############################################
# Name: YouTube Old
##############################################

class HtmlYouTubeOld(core.ISnippet):	
	code = "youtubeold"
	name = "YouTube"
	category = ""
	url = "http://www.youtube.com"
	security = "script"
	reload = False
	action = "ask"
	notes = False
	debug = False
	matchRe = ["\\<object width\\=\"{@number}\" height\\=\"{@number}\"\\>\\<param name\\=\"movie\" value\\=\"(http|https)\\://www\\.(youtube|youtube\\-nocookie)\\.com/v/[a-zA-Z0-9]*[^\\\"<>]*\"\\>\\</param\\>\\<param name\\=\"allowFullScreen\" value\\=\"[a-zA-Z]*\"\\>\\</param\\>\\<param name\\=\"allowscriptaccess\" value\\=\"[a-zA-Z]*\"\\>\\</param\\>\\<embed src\\=\"(http|https)\\://www\\.(youtube|youtube\\-nocookie)\\.com/v/[a-zA-Z0-9]*[^\\\"<>]*\" type\\=\"application/x\\-shockwave\\-flash\" width\\=\"{@number}\" height\\=\"{@number}\" allowscriptaccess\\=\"[a-zA-Z]*\" allowfullscreen\\=\"[a-zA-Z]*\"\\>\\</embed\\>\\</object\\>"]
		
##############################################
# Name: Google Maps
##############################################

class HtmlGoogleMaps(core.ISnippet):	
	code = "googlemaps"
	name = "Google Maps"
	category = ""
	url = "http://maps.google.com"
	security = "iframe"
	reload = False
	action = "ask"
	notes = False
	debug = False
	matchRe = ["\\<iframe width\\=\"{@number}\" height\\=\"{@number}\" frameborder\\=\"{@number}\" scrolling\\=\"[a-zA-Z]*\" marginheight\\=\"{@number}\" marginwidth\\=\"{@number}\" src\\=\"http\\://maps\\.google\\.(com|at|com\\.au|com\\.ba|be|com\\.br|ca|ch|cz|de|dk|es|fi|fr|it|jp|nl|no|co\\.nz|pl|ru|se|tw|co\\.uk)/[^\"'<>]*\"\\>\\</iframe\\>\\<br /\\>\\<small\\>[0-9a-zA-Z\\s\\!]*\\<a href\\=\"http\\://maps\\.google\\.(com|at|com\\.au|com\\.ba|be|com\\.br|ca|ch|cz|de|dk|es|fi|fr|it|jp|nl|no|co\\.nz|pl|ru|se|tw|co\\.uk)/[^\"'<>]*\" style\\=\"[^\"'<>\\(\\)]*\"\\>[0-9a-zA-Z\\s\\!]*\\</a\\>[0-9a-zA-Z\\s\\!]*\\</small\\>",
             "\\<iframe width\\=\"{@number}\" height\\=\"{@number}\" frameborder\\=\"{@number}\" scrolling\\=\"[a-zA-Z]*\" marginheight\\=\"{@number}\" marginwidth\\=\"{@number}\" src\\=\"http\\://maps\\.google\\.(com|at|com\\.au|com\\.ba|be|com\\.br|ca|ch|cz|de|dk|es|fi|fr|it|jp|nl|no|co\\.nz|pl|ru|se|tw|co\\.uk)/[^\"'<>]*\"\\>\\</iframe\\>"]
		
##############################################
# Name: Vimeo
##############################################

class HtmlVimeo(core.ISnippet):	
	code = "vimeo"
	name = "Vimeo"
	category = ""
	url = "http://vimeo.com"
	security = "iframe"
	reload = False
	action = "ask"
	notes = False
	debug = False
	matchRe = ["\\<iframe src\\=\"http\\://player\\.vimeo\\.com/video/{@number}\\?[^\"'<>]*\" width\\=\"{@number}\" height\\=\"{@number}\" frameborder\\=\"{@number}\"\\>\\</iframe\\>\\<p\\>\\<a href\\=\"http\\://vimeo\\.com/{@number}\"\\>[^\<>]*\\</a\\>[0-9a-zA-Z\\s]*\\<a href\\=\"http\\://vimeo\\.com/[0-9a-zA-Z]*\"\\>[^\<>]*\\</a\\>[^\<>]*\\<a href\\=\"http\\://vimeo\\.com\"\\>[^\<>]*\\</a\\>\\.\\</p\\>",
             "\\<iframe src\\=\"http\\://player\\.vimeo\\.com/video/{@number}\\?[^\"'<>]*\" width\\=\"{@number}\" height\\=\"{@number}\" frameborder\\=\"{@number}\"\\>\\</iframe\\>"]

##############################################
# Name: Vimeo Old
##############################################

class HtmlVimeoOld(core.ISnippet):	
	code = "vimeoold"
	name = "Vimeo"
	category = ""
	url = "http://vimeo.com"
	security = "script"
	reload = False
	action = "ask"
	notes = False
	matchRe = ["\\<object width\\=\"{@number}\" height\\=\"{@number}\"\\>\\<param name\\=\"allowfullscreen\" value\\=\"[a-zA-Z]*\" /\\>\\<param name\\=\"allowscriptaccess\" value\\=\"[a-zA-Z]*\" /\\>\\<param name\\=\"movie\" value\\=\"http\\://vimeo\\.com/moogaloop\\.swf\\?[^\\\"<>]*\" /\\>\\<embed src\\=\"http\\://vimeo\\.com/moogaloop\\.swf\\?[^\\\"<>]*\" type\\=\"application/x\\-shockwave\\-flash\" allowfullscreen\\=\"[a-zA-Z]*\" allowscriptaccess\\=\"[a-zA-Z]*\" width\\=\"{@number}\" height\\=\"{@number}\"\\>\\</embed\\>\\</object\\>\\<p\\>\\<a href\\=\"http\\://vimeo\\.com/{@number}\"\\>[^\<>]*\\</a\\>[0-9a-zA-Z\\s]*\\<a href\\=\"http\\://vimeo\\.com/[0-9a-zA-Z]*\"\\>[^\<>]*\\</a\\>[^\<>]*\\<a href\\=\"http\\://vimeo\\.com\"\\>[^\<>]*\\</a\\>\\.\\</p\\>",
             "\\<object width\\=\"{@number}\" height\\=\"{@number}\"\\>\\<param name\\=\"allowfullscreen\" value\\=\"[a-zA-Z]*\" /\\>\\<param name\\=\"allowscriptaccess\" value\\=\"[a-zA-Z]*\" /\\>\\<param name\\=\"movie\" value\\=\"http\\://vimeo\\.com/moogaloop\\.swf\\?[^\\\"<>]*\" /\\>\\<embed src\\=\"http\\://vimeo\\.com/moogaloop\\.swf\\?[^\\\"<>]*\" type\\=\"application/x\\-shockwave\\-flash\" allowfullscreen\\=\"[a-zA-Z]*\" allowscriptaccess\\=\"[a-zA-Z]*\" width\\=\"{@number}\" height\\=\"{@number}\"\\>\\</embed\\>\\</object\\>"]

##############################################
# Name: IntenseDebate Badge
##############################################

class HtmlIntenseDebateBadge(core.ISnippet):	
	code = "intensedebatebadge"
	name = "IntenseDebate Badge"
	category = ""
	url = "http://intensedebate.com"
	security = "external"
	reload = False
	action = "ask"
	notes = False
	matchRe = ["\\<a href\\=\"http\\://intensedebate\\.com/\\?badge\\=[0-9a-zA-Z_]*&user\\=[0-9a-zA-Z_]*\" title\\=\"[0-9a-zA-Z\\s_]*\"\\>\\<img src\\=\"http\\://s\\.intensedebate\\.com/images/[0-9a-zA-Z\\-_]*.png\\?\\=3\" alt\\=\"[0-9a-zA-Z\\s_]*\" /\\>\\</a\\>"]

##############################################
# Name: IntenseDebate Widget
##############################################

class HtmlIntenseDebateWidget(core.ISnippet):	
	code = "intensedebatewidget"
	name = "IntenseDebate Widget"
	category = ""
	url = "http://intensedebate.com"
	security = "script"
	reload = True
	action = "ask"
	notes = False
	matchRe = ["\\<script type\\='text/javascript' defer\\='defer' src\\='http\\://www\\.intensedebate\\.com/widgets/[^\"'<>]*'\\>\\</script\\>",
	           "\\<script src\\=\"http\\://www\\.intensedebate\\.com/widgets/[^\"'<>]*\" defer\\=\"defer\" type\\=\"text/javascript\"\\>\\</script\\>"]
	
##############################################
# Name: IntenseDebate Discussion
##############################################

class HtmlIntenseDebateDiscussion(core.ISnippet):
	code = "intensedebatediscussion"
	name = "IntenseDebate Discussion"
	category = ""
	url = "http://intensedebate.com"	
	security = "script"
	reload = True
	action = "ask"
	notes = True	
	matchRe = ["\\<script\\>var idcomments_acct\\=['\\\"][0-9a-zA-Z]*['\\\"];var idcomments_post_id(\\=['\\\"][0-9a-zA-Z]*['\\\"])?;var idcomments_post_url(\\=['\\\"][0-9a-zA-Z_\-\:\/\.]*['\\\"])?;\\</script\\>(\\<span id\\=['\\\"]IDCommentsPostTitle['\\\"] style\\=['\\\"]display\\:none['\\\"]\\>\\</span\\>)?\\<script type\\=['\\\"]text/javascript['\\\"] src\\=['\\\"]http\\://www\\.intensedebate\\.com/js/[^\"'<>]*['\\\"]\\>\\</script\\>"]

##############################################
# Name: Megavideo
##############################################

class HtmlMegaVideo(core.ISnippet):	
	code = "megavideo"
	name = "MegaVideo"
	category = ""
	url = "http://www.megavideo.com"
	security = "script"
	reload = False
	action = "ask"
	notes = False
	debug = False
	matchRe = ["\\<object width\\=\"{@number}\" height\\=\"{@number}\"\\>\\<param name\\=\"movie\" value\\=\"(http|https)\\://www\\.megavideo\\.com/v/[a-zA-Z0-9]*[^\\\"<>]*\"\\>\\</param\\>\\<param name\\=\"allowFullScreen\" value\\=\"[a-zA-Z]*\"\\>\\</param\\>\\<embed src\\=\"(http|https)\\://www\\.megavideo\\.com/v/[a-zA-Z0-9]*[^\\\"<>]*\" type\\=\"application/x\\-shockwave\\-flash\" allowfullscreen\\=\"[a-zA-Z]*\" width\\=\"{@number}\" height\\=\"{@number}\"\\>\\</embed\\>\\</object\\>"]
	
##############################################
# Name: Mibbit
##############################################

class HtmlMibbit(core.ISnippet):	
	code = "mibbit"
	name = "Mibbit"
	category = ""
	url = "http://www.mibbit.com"
	security = "iframe"
	reload = False
	action = "ask"
	notes = False	
	debug = False
	matchRe = ["\\<iframe width\\=\"{@number}\" height\\=\"{@number}\"src\\=\"http\\://widget\\.mibbit\\.com/\\?settings\\={@alphaword}&server\\={@domain}&channel\\={@urlstring}\"\\>\\</iframe\\>"]
	
##############################################
# Name: Jamendo
##############################################

#macro: K
class HtmlJamendo(core.ISnippet):	
	code = "jamendo"
	name = "Jamendo"
	category = ""
	url = "http://www.jamendo.com"
	security = "script"
	reload = False
	action = "ask"
	notes = False	
	matchRe = ["(\\<div style\\=\"text\\-align\\:center;\"\\>)?\\<object width\\=\"{@number}\" height\\=\"{@number}\" classid\\=\"clsid\\:d27cdb6e\\-ae6d\\-11cf\\-96b8\\-444553540000\" codebase\\=\"http\\://fpdownload\\.macromedia\\.com/pub/shockwave/cabs/flash/swflash\\.cab#version\\=7,0,0,0\" align\\=\"middle\"\\>\\<param name\\=\"allowScriptAccess\" value\\=\"always\" /\\>\\<param name\\=\"wmode\" value\\=\"transparent\" /\\>\\<param name\\=\"movie\" value\\=\"http\\://widgets\\.jamendo\\.com/{@url_after}\" /\\>\\<param name\\=\"quality\" value\\=\"high\" /\\>\\<param name\\=\"bgcolor\" value\\=\"#FFFFFF\" /\\>\\<embed src\\=\"http\\://widgets\\.jamendo\\.com/{@url_after}\" quality\\=\"high\" wmode\\=\"transparent\" bgcolor\\=\"#FFFFFF\" width\\=\"{@number}\" height\\=\"{@number}\" align\\=\"middle\" allowScriptAccess\\=\"always\" type\\=\"application/x\\-shockwave\\-flash\" pluginspage\\=\"http\\://www\\.macromedia\\.com/go/getflashplayer\"\\>&nbsp;\\</embed\\>&nbsp;\\</object\\>(\\<a href\\=\"http\\://pro\\.jamendo\\.com/\" style\\=\"display\\:block;font\\-size\\:8px \\!important;\"\\>{@text_html}\\</a\\>)?(\\</div\\>)?"]
	
##############################################
# Name: Disqus
##############################################

class HtmlDisqus(core.ISnippet):	
	code = "disqus"
	name = "Disqus"
	category = ""
	url = "http://disqus.com"
	security = "script"
	reload = False
	action = "ask"
	notes = False
	matchRe = ["\\<div id\\=\"disqus_thread\"\\>\\</div\\>\\<script type\\=\"text/javascript\"\\>var disqus_shortname\\='[0-9a-zA-Z_\\\\-]*';(var disqus_identifier\\='[0-9a-zA-Z]*';)?(var disqus_url\\='[0-9a-zA-Z]*';)?\\(function\\(\\)\\{var dsq\\=document\\.createElement\\('script'\\);dsq\\.type\\='text/javascript';dsq\\.async\\=true;dsq\\.src\\='http\\://' \\+ disqus_shortname \\+ '\\.disqus\\.com/embed\\.js';\\(document\\.getElementsByTagName\\('head'\\)\\[0\\] \\|\\| document\\.getElementsByTagName\\('body'\\)\\[0\\]\\)\\.appendChild\\(dsq\\);\\{\\)\\(\\);\\</script\\>(\\<a href\\=\"http\\://disqus\\.com\" class\\=\"dsq\\-brlink\"\\>blog comments powered by\\<span class\\=\"logo\\-disqus\"\\>Disqus\\</span\\>\\</a\\>)?", # Universal Code
	           "(\\<div id\\=\"[0-9a-zA-Z]*\" class\\=\"dsq\\-widget\"\\>)?(\\<h2 class\\=\"dsq\\-widget\\-title\"\\>[^<>]*\\</h2\\>)?\\<script type\\=\"text/javascript\" src\\=\"http\\://[0-9a-zA-Z_\\-]*\\.disqus\\.com/[0-9a-zA-Z_\\-]*\\.js\\?[^\"'<>]*\"\\>\\</script\\>(\\</div\\>)?(\\<a href\\=\"http\\://disqus\\.com/\"\\>Powered by Disqus\\</a\\>)?"] # Widget

##############################################
# Name: Daily Motion
##############################################

class HtmlDailyMotion(core.ISnippet):	
	code = "dailymotion"
	name = "Daily Motion"
	category = ""
	url = "http://www.dailymotion.com"
	security = "iframe"
	reload = False
	action = "ask"
	notes = False	
	debug = False
	matchRe = ["\\<iframe frameborder\\=\"{@number}\" width\\=\"{@number}\" height\\=\"{@number}\" src\\=\"http\\://www\\.dailymotion\\.com/embed/video/{@urlstring}\"\\>\\</iframe\\>",
	           "\\<iframe frameborder\\=\"{@number}\" width\\=\"{@number}\" height\\=\"{@number}\" src\\=\"http\\://www\\.dailymotion\\.com/embed/video/{@urlstring}\"\\>\\</iframe\\>\\<br /\\>\\<a href\\=\"http\\://www\\.dailymotion\\.com/video/{@urlstring}\" target\\=\"_blank\"\\>{@text_html}\\</a\\> \\<i\\>{@text_html}\\<a href\\=\"http\\://www\\.dailymotion\\.com/{@urlstring}\" target\\=\"_blank\"\\>{@text_html}\\</a\\>\\</i\\>"]
	
##############################################
# Name: MetaCafe
##############################################

class HtmlMetaCafe(core.ISnippet):	
	code = "metacafe"
	name = "MetaCafe"
	category = ""
	url = "http://www.metacafe.com"
	security = "script"
	reload = False
	action = "ask"
	notes = False	
	debug = False
	matchRe = ["\\<embed flashVars\\=\"playerVars\\=autoPlay\\={@word}\" src\\=\"http\\://www\\.metacafe\\.com/fplayer/{@number}/{@string}\\.swf\" width\\=\"{@number}\" height\\=\"{@number}\" wmode\\=\"{@word}\" allowFullScreen\\=\"{@word}\" allowScriptAccess\\=\"{@word}\" name\\=\"Metacafe_{@number}\" pluginspage\\=\"http\\://www\\.macromedia\\.com/go/getflashplayer\" type\\=\"application/x\\-shockwave\\-flash\"\\>\\</embed\\>",
	           "\\<embed flashVars\\=\"playerVars\\=autoPlay\\={@word}\" src\\=\"http\\://www\\.metacafe\\.com/fplayer/{@number}/{@string}\\.swf\" width\\=\"{@number}\" height\\=\"{@number}\" wmode\\=\"{@word}\" allowFullScreen\\=\"{@word}\" allowScriptAccess\\=\"{@word}\" name\\=\"Metacafe_{@number}\" pluginspage\\=\"http\\://www\\.macromedia\\.com/go/getflashplayer\" type\\=\"application/x\\-shockwave\\-flash\"\\>\\</embed\\>\\<div style\\=\"font\\-size\\:12px;\"\\>\\<a href\\=\"http\\://www\\.metacafe\\.com/watch/{@number}/{@string}/\"\\>{@text_html}\\</a\\>{@text_html}\\<a href\\=\"http\\://www\\.metacafe\\.com/topics/{@string}/\" title\\=\"{@string}\"\\>{@string}\\</a\\>, \\<a href\\=\"http\\://www\\.metacafe\\.com/topics/{@string}/\" title\\=\"{@string}\"\\>{@text_html}\\</a\\>\\</div\\>"]

##############################################
# Name: InternetArchive
##############################################

class HtmlInternetArchive(core.ISnippet):	
	code = "internetarchive"
	name = "InternetArchive"
	category = ""
	url = "http://www.archive.org"
	security = "iframe"
	reload = False
	action = "ask"
	notes = False	
	debug = False
	matchRe = ["\\<iframe src\\=\"http\\://www\\.archive\\.org/embed/[^\\\"'<>]*\" width\\=\"{@number}\" height\\=\"{@number}\" frameborder\\=\"{@number}\"\\>\\</iframe\\>"]	

##############################################
# Name: LiveStream
##############################################

class HtmlLiveStream(core.ISnippet):	
	code = "livestream"
	name = "LiveStream"
	category = ""
	url = "http://www.livestream.com"
	security = "iframe"
	reload = False
	action = "ask"
	notes = False	
	debug = False
	matchRe = ["\\<iframe width\\=\"{@number}\" height\\=\"{@number}\" src\\=\"http\\://cdn\\.livestream\\.com/embed/[^\\\"'<>]*\" style\\=\"border\\:0;outline\\:0\" frameborder\\=\"{@number}\" scrolling\\=\"no\"\\>\\</iframe\\>"]	
	
##############################################
# Name: SoundCloud
##############################################

class HtmlSoundCloud(core.ISnippet):	
	code = "soundcloud"
	name = "SoundCloud"
	category = ""
	url = "http://soundcloud.com"
	security = "iframe"
	reload = False
	action = "ask"
	notes = False	
	debug = False	
	matchRe = ["\\<iframe width\\=\"{@unit}\" height\\=\"{@unit}\" scrolling\\={@apix}no{@apix} frameborder\\={@apix}no{@apix} src\\=\"http\\://w\\.soundcloud\\.com/{@url_after}\"\\>\\</iframe\\>"]	

##############################################
# Name: XVideos
##############################################

class HtmlXVideos(core.ISnippet):	
	code = "xvideos"
	name = "XVideos"
	category = "xxx"
	url = "http://www.xvideos.com"
	security = "iframe"
	reload = False
	action = "ask"
	notes = False	
	debug = False
	matchRe = ["\\<iframe src\\={@apix}http\\://flashservice\\.xvideos\\.com/embedframe/{@number}{@apix} frameborder\\={@apix}{@number}{@apix} width\\={@apix}{@number}{@apix} height\\={@apix}{@number}{@apix} scrolling\\={@apix}no{@apix}\\>\\</iframe\\>"]
	
##############################################
# Name: PornHub
##############################################

class HtmlPornHub(core.ISnippet):	
	code = "pornhub"
	name = "PornHub"
	category = "xxx"
	url = "http://www.pornhub.com"
	security = "script"
	reload = False
	action = "ask"
	notes = False	
	debug = False
	matchRe = ["\\<object type\\=\"application/x\\-shockwave\\-flash\" data\\=\"http\\://cdn1\\.static\\.pornhub\\.phncdn\\.com/flash/embed_player\\.swf\" width\\=\"{@number}\" height\\=\"{@number}\"\\>\\<param name\\=\"movie\" value\\=\"http\\://cdn1\\.static\\.pornhub\\.phncdn\\.com/flash/embed_player\\.swf\" /\\>\\<param name\\=\"bgColor\" value\\=\"#000000\" /\\>\\<param name\\=\"allowfullscreen\" value\\=\"true\" /\\>\\<param name\\=\"allowScriptAccess\" value\\=\"always\" /\\>\\<param name\\=\"FlashVars\" value\\=\"options\\=http\\://www\\.pornhub\\.com/embed_player\\.php\\?id\\={@number}\"/\\>\\</object\\>",
	           "\\<object type\\=\"application/x\\-shockwave\\-flash\" data\\=\"http\\://cdn1\\.static\\.pornhub\\.phncdn\\.com/flash/embed_player\\.swf\" width\\=\"{@number}\" height\\=\"{@number}\"\\>\\<param name\\=\"movie\" value\\=\"http\\://cdn1\\.static\\.pornhub\\.phncdn\\.com/flash/embed_player\\.swf\" /\\>\\<param name\\=\"bgColor\" value\\=\"#000000\" /\\>\\<param name\\=\"allowfullscreen\" value\\=\"true\" /\\>\\<param name\\=\"allowScriptAccess\" value\\=\"always\" /\\>\\<param name\\=\"FlashVars\" value\\=\"options\\=http\\://www\\.pornhub\\.com/embed_player\\.php\\?id\\={@number}\"/\\>\\</object\\>\\<br /\\>\\<a href\\=\"http\\://www\\.pornhub\\.com/view_video\\.php\\?viewkey\\={@number}\"\\>{@text_html}\\?\\</a\\> brought to you by \\<a href\\=\"http\\://www\\.pornhub\\.com/\"\\>PornHub\\</a\\>"]
	
##############################################
# Name: Tube8
##############################################

class HtmlTube8(core.ISnippet):	
	code = "tube8"
	name = "Tube8"
	category = "xxx"
	url = "http://www.tube8.com/"
	security = "iframe"
	reload = False
	action = "ask"
	notes = False	
	debug = False
	matchRe = ["\\<iframe src\\='http\\://www\\.tube8\\.com/embed/{@string}/{@string}/{@number}/' frameborder\\={@number} height\\={@number} width\\={@number} scrolling\\={@word} name\\='{@string}'\\>\\<a href\\='http\\://www\\.tube8\\.com/{@string}/{@string}/{@number}/'\\>{@text_html}\\</a\\> brought to you by \\<a href\\='http\\://www\\.tube8\\.com/'\\>Tube8\\</a\\>\\</iframe\\>",
	           "\\<iframe src\\='http\\://www\\.tube8\\.com/embed/{@string}/{@string}/{@number}/' frameborder\\={@number} height\\={@number} width\\={@number} scrolling\\={@word} name\\='{@string}'\\>\\<a href\\='http\\://www\\.tube8\\.com/{@string}/{@string}/{@number}/'\\>{@text_html}\\</a\\> brought to you by \\<a href\\='http\\://www\\.tube8\\.com/'\\>Tube8\\</a\\>\\</iframe\\>\\<br\\>\\<a href\\='http\\://www\\.tube8\\.com/{@string}/{@string}/{@number}/'\\>{@text_html}\\</a\\> brought to you by \\<a href\\='http\\://www\\.tube8\\.com/'\\>Tube8\\</a\\>"]
		
##############################################
# Name: PornTube
##############################################

class HtmlPornTube(core.ISnippet):	
	code = "porntube"
	name = "PornTube"
	category = "xxx"
	url = "http://www.porntube.com"
	security = "iframe"
	reload = False
	action = "ask"
	notes = False	
	debug = False
	matchRe = ["\\<iframe width\\=\"{@number}\" height\\=\"{@number}\" type\\=\"text/html\" src\\=\"http\\://embed\\.porntube\\.com/{@alphaword}\" frameborder\\=\"{@number}\" allowFullScreen id\\=\"{@alphaword}\" class\\=\"porntube\\-player\"\\>\\</iframe\\>",
	           "\\<iframe width\\=\"{@number}\" height\\=\"{@number}\" type\\=\"text/html\" src\\=\"http\\://embed\\.porntube\\.com/{@alphaword}\" frameborder\\=\"{@number}\" allowFullScreen id\\=\"{@alphaword}\" class\\=\"porntube\\-player\"\\>\\</iframe\\>\\<br /\\>Video brought to you by \\<a id\\=\"{@alphaword}\" href\\=\"http\\://www\\.porntube\\.com\"\\>PornTube\\.com\\</a\\>"]
	
##############################################
# Name: Slutload
##############################################

class HtmlSlutload(core.ISnippet):	
	code = "slutload"
	name = "SlutLoad"
	category = "xxx"
	url = "http://www.slutload.com"
	security = "script"
	reload = False
	action = "ask"
	notes = False	
	debug = False
	matchRe = ["\\<object type\\=\"application/x\\-shockwave\\-flash\" data\\=\"http\\://emb\\.slutload\\.com/{@alphaword}\" width\\=\"{@number}\" height\\=\"{@number}\"\\>\\<param name\\=\"AllowScriptAccess\" value\\=\"{@word}\"\\>\\<param name\\=\"movie\" value\\=\"http\\://emb\\.slutload\\.com/{@alphaword}\"\\>\\</param\\>\\<param name\\=\"allowfullscreen\" value\\=\"true\"\\>\\</param\\>\\<embed src\\=\"http\\://emb\\.slutload\\.com/{@alphaword}\" AllowScriptAccess\\=\"always\" allowfullscreen\\=\"true\" width\\=\"{@number}\" height\\=\"{@number}\"\\>\\</embed\\>\\</object\\>",
	           "\\<div style\\=\"margin\\:auto\" align\\=\"center\"\\>\\<object type\\=\"application/x\\-shockwave\\-flash\" data\\=\"http\\://emb\\.slutload\\.com/{@alphaword}\" width\\=\"{@number}\" height\\=\"{@number}\"\\>\\<param name\\=\"AllowScriptAccess\" value\\=\"{@word}\"\\>\\<param name\\=\"movie\" value\\=\"http\\://emb\\.slutload\\.com/{@alphaword}\"\\>\\</param\\>\\<param name\\=\"allowfullscreen\" value\\=\"true\"\\>\\</param\\>\\<embed src\\=\"http\\://emb\\.slutload\\.com/{@alphaword}\" AllowScriptAccess\\=\"always\" allowfullscreen\\=\"true\" width\\=\"{@number}\" height\\=\"{@number}\"\\>\\</embed\\>\\</object\\>\\<br /\\>\\<a href\\=\"http\\://www\\.slutload\\.com/\" target\\=\"_blank\"\\>Free Porn Movies\\. Unlimited upload porn for free\\.\\</a\\>\\</div\\>"]
	
##############################################
# Name: TnaFlix
##############################################

class HtmlTnaFlix(core.ISnippet):	
	code = "tnaflix"
	name = "TnaFlix"
	category = "xxx"
	url = "http://www.tnaflix.com/"
	security = "iframe"
	reload = False
	action = "ask"
	notes = False	
	debug = False
	matchRe = ["\\<iframe src\\=\"http\\://player\\.tnaflix\\.com/video/{@number}\" width\\=\"{@number}\" height\\=\"{@number}\" frameborder\\=\"{@number}\"\\>\\</iframe\\>",
	           "\\<iframe src\\=\"http\\://player\\.tnaflix\\.com/video/{@number}\" width\\=\"{@number}\" height\\=\"{@number}\" frameborder\\=\"{@number}\"\\>\\</iframe\\>\\<p\\>\\<a href\\=\"http\\://www\\.tnaflix\\.com/{@string}/{@string}/{@string}\"\\>{@text_html}\\</a\\> from \\<a href\\=\"http\\://www\\.tnaflix\\.com/uprofile\\.php\\?UID\\={@number}\"\\>bonez\\</a\\> on \\<a href\\=\"http\\://www\\.tnaflix\\.com\"\\>TnaFlix\\</a\\>\\.\\</p\\>\\<div style\\=\"height\\: 14px; border\\: 1px solid #b1b1b1; background\\: #e1e1e1; font\\-size\\: 11px; padding\\: 3px; font\\-family\\: Verdana; width\\:642px;_width\\:650px\"\\>\\<span style\\=\"float\\: left; color\\: #000;\"\\>\\<a href\\=\"http\\://www\\.tnaflix\\.com/{@string}\" style\\=\"color\\: #000;\"\\>{@text_html}\\</a\\>\\</span\\>\\<span style\\=\"float\\: right; color\\: #000;\"\\>\\<a href\\=\"http\\://www\\.tnaflix\\.com/\" style\\=\"color\\: #000;\"\\>Porn Tube\\</a\\>\\</span\\>\\</div\\>"]
	
##############################################
# Name: RedTube
##############################################

class HtmlRedTube(core.ISnippet):	
	code = "redtube"
	name = "RedTube"
	category = "xxx"
	url = "http://www.redtube.com/"
	security = "script"
	reload = False
	action = "ask"
	notes = False	
	debug = False
	matchRe = ["\\<object height\\=\"{@number}\" width\\=\"{@number}\"\\>\\<param name\\=\"allowfullscreen\" value\\=\"false\"\\>\\<param name\\=\"movie\" value\\=\"http\\://embed\\.redtube\\.com/player/\"\\>\\<param name\\=\"FlashVars\" value\\=\"id\\={@number}&style\\=redtube&autostart\\=false\"\\>\\<embed src\\=\"http\\://embed\\.redtube\\.com/player/\\?id\\={@number}&style\\=redtube\" allowfullscreen\\=\"false\" flashvars\\=\"autostart\\=false\" pluginspage\\=\"http\\://www\\.adobe\\.com/shockwave/download/download\\.cgi\\?P1_Prod_Version\\=ShockwaveFlash\" type\\=\"application/x\\-shockwave\\-flash\" height\\=\"{@number}\" width\\=\"{@number}\" /\\>\\</object\\>",
	           "\\<object height\\=\"{@number}\" width\\=\"{@number}\"\\>\\<param name\\=\"allowfullscreen\" value\\=\"false\"\\>\\<param name\\=\"movie\" value\\=\"http\\://embed\\.redtube\\.com/player/\"\\>\\<param name\\=\"FlashVars\" value\\=\"id\\={@number}&style\\=redtube&autostart\\=false\"\\>\\<embed src\\=\"http\\://embed\\.redtube\\.com/player/\\?id\\={@number}&style\\=redtube\" allowfullscreen\\=\"false\" flashvars\\=\"autostart\\=false\" pluginspage\\=\"http\\://www\\.adobe\\.com/shockwave/download/download\\.cgi\\?P1_Prod_Version\\=ShockwaveFlash\" type\\=\"application/x\\-shockwave\\-flash\" height\\=\"{@number}\" width\\=\"{@number}\" /\\>\\</object\\>\\<br /\\>\\<a href\\=\"http\\://www\\.redtube\\.com/{@number}\"\\>{@text_html}\\</a\\> brought to you by \\<a href\\=\"http\\://www\\.redtube\\.com/\"\\>RedTube \\- Home of free porn videos\\</a\\>"]
	
##############################################
# Name: Empflix
##############################################

class HtmlEmpflix(core.ISnippet):	
	code = "empflix"
	name = "Empflix"
	category = "xxx"
	url = "http://www.empflix.com"
	security = "iframe"
	reload = False
	action = "ask"
	notes = False	
	debug = False
	matchRe = ["\\<iframe src\\=\"http\\://player\\.empflix\\.com/video/{@number}\" width\\=\"{@number}\" height\\=\"{@number}\" frameborder\\=\"{@number}\"\\>\\</iframe\\>",
	           "\\<iframe src\\=\"http\\://player\\.empflix\\.com/video/{@number}\" width\\=\"{@number}\" height\\=\"{@number}\" frameborder\\=\"{@number}\"\\>\\</iframe\\>\\<div style\\=\"height\\: 14px; border\\: 1px solid  #b1b1b1; background\\: #e1e1e1;  font\\-size\\: 11px; padding\\: 3px; font\\-family\\: Verdana; width\\:642px;_width\\:650px\"\\>\\<span style\\=\"float\\: left; color\\: #000\"\\>\\<a href\\=\"http\\://www\\.empflix\\.com/\" style\\=\"color\\: #000\"\\>Free Sex\\</a\\> \\- \\<a href\\=\"http\\://www\\.empflix\\.com/\" style\\=\"color\\: #000\"\\>Porn Tube\\</a\\> \\- \\<a href\\=\"http\\://www\\.empflix\\.com/channels/{@string}\\.html\" style\\=\"color\\: #000\"\\>{@text_html}\\</a\\>\\</span\\>\\<span style\\=\"float\\: right; color\\: #000\"\\>\\<a href\\=\"http\\://www\\.empflix\\.com/browse\\.php\" style\\=\"color\\: #000\"\\>Free Porn Movies\\</a\\> at Empflix\\.com\\</span\\>\\</div\\>"]
	
##############################################
# Name: Generic HTML
##############################################

class HtmlGeneric(core.ISnippet):
	code = "generic"
	name = "Generic"
	category = ""
	url = ""
	security = "script"
	reload = False
	action = "no"
	notes = False
	#debug = True
	
	def match(self, html):
		return True