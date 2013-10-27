import os
import osiris

# Syntax:
# "<name>" : "<regex escaped>" # regex unescaped (for copy-paste) # Description

regexMacros = { "number"     : "[0-9]*", 											# [0-9]* 								# Number
								"unit"     	 : "[0-9]*%?", 											# [0-9]* 						  # Unit: "<number>", or "<number>%", or "<number>px" or "<number>em".
                "word"       : "[a-zA-Z]*",										# [a-zA-Z]*							# Word.
                "alphaword"  : "[0-9a-zA-Z]*",								# [0-9a-zA-Z]*					# Alphanumeric word.
                "string"     : "[0-9a-zA-Z\\.\\s\\-_]*",			# [0-9a-zA-Z\.\s\-_]*		# String, url-encoding NOT allowed.
                "urlstring"  : "[0-9a-zA-Z\\.\\s\\-_%]*",			# [0-9a-zA-Z\.\s\-_%]*	# String, url-encoding allowed.
                "login"      : "[0-9a-zA-Z\\.\\-_]*",     		# [0-9a-zA-Z\.\-_]*   	# Login fields.
                "domain"     : "[0-9a-zA-Z\\.\\-_]*",     		# [0-9a-zA-Z\.\-_]*   	# Domain fields.
                "text_html"  : "[^<>]*",											# [^<>]*								# Plain text, HTML encoded, that not have any HTML tag.
                "url_after"  : "[^\"'<>]*", 									# [^"'<>]* 							# Anything plain text until a " is reached. Use this in the final part of an url, normally after ?, always after domain name.                
                "attr_style" : "[^\"'<>\\(\\)*\"]", 					# [^"'<>\(\)*"]					# Used in "style" attributes.
                "attr_color" : "[a-zA-Z0-9\\#]+",							# [a-zA-Z0-9\#]+ 				# Used in color attributes.
                "apix"       : "['\\\"]?"											# ['\"]?								# Match ' or " or none, opening and closing attribute.
              }

# Remember: escape character \ and " (with a prefix \) when copy from comment to dictionary value.

# Useful regex samples, not under macro system:
# ['\"] 								- Match ' or ", opening and closing attribute.

