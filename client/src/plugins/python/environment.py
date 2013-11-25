# <osiris_sps_source_header>
# This file is part of Osiris Serverless Portal System.
# Copyright (C)2005-2012 Osiris Team (info@osiris-sps.org) / http:#www.osiris-sps.org )
#
# Osiris Serverless Portal System is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# Osiris Serverless Portal System is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with Osiris Serverless Portal System.  If not, see <http:#www.gnu.org/licenses/>.
# </osiris_sps_source_header>

#! /usr/bin/python

import os
import sys

class settings:
	gccxml_path = ""
	module_name = "osirislib"    
	root_dir = ""
	working_directory = ""
	output_directory = ""
	include_paths = []
	defined_symbols = ["OS_STATIC_LIBRARY", "BOOST_HAS_THREADS", "BOOST_NO_INITIALIZER_LISTS", "OS_GCCXML", "BOOST_COMMON_TYPE_DONT_USE_TYPEOF", "__GNUC__=3", "__GNUC_MINOR__=3"]

settings.root_dir = os.path.abspath(os.path.dirname(__file__))
settings.working_directory = settings.root_dir

if sys.platform == "linux2":
	settings.gccxml_path = os.path.join(settings.root_dir, "../../../../utils/gccxml/linux/bin")	
	settings.include_paths.append(os.path.join(settings.root_dir, "../../../dependencies/include/common"))
	settings.include_paths.append(os.path.join(settings.root_dir, "../../../dependencies/include/linux/i386"))
elif sys.platform == "win32":
	settings.gccxml_path = os.path.join(settings.root_dir, "../../../../utils/gccxml/win32/bin")
	settings.include_paths.append(os.path.join(settings.root_dir, "../../../dependencies/include/common"))
	settings.include_paths.append(os.path.join(settings.root_dir, "../../../dependencies/include/windows/i386"))
	settings.defined_symbols.append("WIN32")	
else:
    raise RuntimeError("There is no configuration for \"%s\" platform." % sys.platform)

settings.include_paths.append(os.path.join(settings.root_dir, "../../../dependencies/include"))
settings.include_paths.append(os.path.join(settings.root_dir, "../../archives"))
settings.include_paths.append(os.path.join(settings.root_dir, "../../core"))
settings.include_paths.append(os.path.join(settings.root_dir, "../../db"))
settings.include_paths.append(os.path.join(settings.root_dir, "../../engine"))
settings.include_paths.append(os.path.join(settings.root_dir, "../../http"))
settings.include_paths.append(os.path.join(settings.root_dir, "../../lang"))
settings.include_paths.append(os.path.join(settings.root_dir, "../../net"))
settings.include_paths.append(os.path.join(settings.root_dir, "../../web"))
settings.include_paths.append(os.path.join(settings.root_dir, "../../xml"))
settings.output_directory = os.path.join(settings.root_dir, "wrappers")
