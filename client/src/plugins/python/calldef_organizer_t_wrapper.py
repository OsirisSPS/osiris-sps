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

# TODO: dovrebbe essere stato sistemato nella versione svn del Py++
class calldef_organizer_t_wrapper(object):
	def __init__( self ):
		object.__init__( self )
		self.__cmp_unrelated = lambda d1, d2: cmp( d1.location.line, d2.location.line )
		
	def __build_groups( self, decls ):
		groups = { None: [] }
		for d in decls:
			# Berserker: modificato rispetto alla base che effettua il controllo che len( d.required_args ) == 1
			if not isinstance( d, declarations.calldef_t ) or len( d.required_args ) == 0:
				groups[ None ].append( d )
			else:
				if not groups.has_key( d.name ):
					groups[ d.name ] = []
				groups[ d.name ].append( d )
		return groups
				
	def __cmp_types(self, t1, t2):
		if declarations.is_pointer( t1 ) and declarations.is_pointer( t2 ):
			return registration_order.is_related( declarations.remove_pointer( t1 ), declarations.remove_pointer( t2 ) )
		elif declarations.is_pointer( t1 ) and not declarations.is_pointer( t2 ):
			t1 = declarations.remove_cv( declarations.remove_pointer( t1 ) )
			t2 = declarations.remove_cv( t2 )
			if declarations.is_same( t1, t2 ):
				return 1
		elif not declarations.is_pointer( t1 ) and declarations.is_pointer( t2 ):
			t1 = declarations.remove_cv( t1 )
			t2 = declarations.remove_cv( declarations.remove_pointer( t2 ) )
			if declarations.is_same( t1, t2 ):
				return -1
		else: #not is_pointer( t1 ) and not is_pointer( t2 ):
			# Berserker: modificato rispetto alla base che effettua solo il controllo su is_numeric al posto di is_arithmetic (da problemi sui double)
			if declarations.is_arithmetic( t1 ) and not declarations.is_bool( t1 )  and declarations.is_bool( t2 ):
				return -1
			elif declarations.is_bool( t1 ) and declarations.is_arithmetic( t2 ) and not declarations.is_bool( t2 ):
				return 1
			else:
				pass
			return None

	def __cmp( self, f1, f2 ):
		# Berserker: modificato rispetto alla base per effettuare il controllo sul primo parametro "diverso"
		result = None	
		args = len(f1.arguments)
		if args == len(f2.arguments):
			i = 0
			while i < args and result is None:
				if not declarations.is_same(f1.arguments[i].type, f2.arguments[i].type):
					result = self.__cmp_types(f1.arguments[i].type, f2.arguments[i].type)
				i = i + 1
		
		if result is None:
			result = self.__cmp_unrelated( f1, f2 )
		return result

	def __sort_groups( self, groups ):
		for group in groups.keys():
			if None is group:
				continue
			groups[ group ].sort( self.__cmp )

	def __join_groups( self, groups ):
		decls = []
		sorted_keys = groups.keys()
		sorted_keys.sort()
		for group in sorted_keys:
			decls.extend( groups[group] )
		return decls

	def sort( self, decls ):
		groups = self.__build_groups( decls )
		self.__sort_groups(groups)
		result = self.__join_groups(groups)
		return result

def sort_calldefs_wrapper( decls ):
	return calldef_organizer_t_wrapper().sort(decls)
	
creators_factory.sort_algorithms.sort_calldefs = sort_calldefs_wrapper