#!/usr/bin/python
"""
nifdoc.py

Generates HTML documentation for the XML file.

To list command line options run:
    nifdoc.py -h

This file is part of nifxml <https://www.github.com/niftools/nifxml>
Copyright (c) 2017 NifTools

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, version 3.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
"""

from __future__ import unicode_literals

import re
import os
import io
import logging
import argparse
from shutil import copy2

from tools.nifxml import Compound, Block, Enum, parse_xml, version2number, Basic, Bitfield
from tools.nifxml import TYPES_BLOCK, TYPES_BASIC, TYPES_COMPOUND, TYPES_ENUM, TYPES_FLAG, TYPES_VERSION, TYPES_BITFIELD
from tools.nifxml import NAMES_BLOCK, NAMES_BASIC, NAMES_COMPOUND, NAMES_ENUM, NAMES_FLAG, NAMES_VERSION, NAMES_BITFIELD

from tools.nifdoc import nifdoc_tmpl as tmpl

#
# Globals
#

SCRIPT_PATH = os.path.dirname(os.path.realpath(__file__))
DOC_PATH = '/doc/'
DOC_FILE = '{}.html'
CSS_PATH = os.path.join(SCRIPT_PATH, 'doc/nifdoc.css')
ICO_PATH = os.path.join(SCRIPT_PATH, 'doc/favicon.ico')

def clean(string):
    """Removes everything but letters from a string"""
    pattern = re.compile(r'[\W_]+')
    return pattern.sub('', string)

def main():
    # Default to calling directory
    root_dir = '.'
    heading = True
    metadata = True
    minver = ''

    # Command line args
    parser = argparse.ArgumentParser(description="NIF Format XML Docs Generation")
    parser.add_argument('-p', '--path', help="The path where the doc folder will be generated.")
    parser.add_argument('-no-h1', '--no-heading', action='store_true',
                        help="Whether to not generate the main <h1> heading. Used by NifSkope for built-in help.")
    parser.add_argument('-no-meta', '--no-metadata-columns', action='store_true',
                        help="Whether to not generate the metadata attribute columns (arg, arr1, arr2, etc.)")
    parser.add_argument('-min-ver', '--minimum-version',
                        help="Hides attributes below this version. Format 'XX.X.X.XX'")
    args = parser.parse_args()
    if args.path:
        root_dir = args.path
    if args.no_heading:
        heading = False
    if args.no_metadata_columns:
        metadata = False
    if args.minimum_version:
        minver = args.minimum_version

    # Logger
    logging.basicConfig(level=logging.INFO)
    logger = logging.getLogger('nifdoc')


    """Parses the XML and generates all doc pages"""
    # Parse the XML and sort names
    if not parse_xml():
        return

    NAMES_BASIC.sort()
    NAMES_COMPOUND.sort()
    NAMES_BLOCK.sort()
    NAMES_ENUM.sort()
    NAMES_FLAG.sort()
    NAMES_BITFIELD.sort()

    # Create the document generator
    doc = DocGenerator(root_dir + DOC_PATH, heading, metadata, version2number(minver))
    logger.info("Generating NiObject Pages...")
    doc.gen_pages(NAMES_BLOCK, TYPES_BLOCK, tmpl.NIOBJECT if metadata else tmpl.NIOBJECT_NO_META)
    logger.info("Generating Compound Pages...")
    doc.gen_pages(NAMES_COMPOUND, TYPES_COMPOUND, tmpl.COMPOUND if metadata else tmpl.COMPOUND_NO_META)
    logger.info("Generating Basic Pages...")
    doc.gen_pages(NAMES_BASIC, TYPES_BASIC, tmpl.BASIC)
    logger.info("Generating Enum Pages...")
    enums = dict(TYPES_ENUM, **TYPES_FLAG)
    doc.gen_pages(sorted(enums), enums, tmpl.ENUM)
    logger.info("Generating Bitfield Pages...")
    doc.gen_pages(NAMES_BITFIELD, TYPES_BITFIELD, tmpl.BITFIELD)

    logger.info("Generating Index Pages...")
    doc.gen_list_page('Basic Data Types', NAMES_BASIC, TYPES_BASIC, 'basic_list')
    doc.gen_list_page('NIF Object List', NAMES_BLOCK, TYPES_BLOCK, 'niobject_list')
    doc.gen_list_page('Compound Data Types', NAMES_COMPOUND, TYPES_COMPOUND, 'compound_list')
    doc.gen_list_page('Enum Data Types', sorted(enums), enums, 'enum_list')
    doc.gen_list_page('NIF File Format Versions', NAMES_VERSION, TYPES_VERSION, 'version_list', tmpl.VERSION_ROW, 'Versions')
    doc.gen_list_page('Bitfield Data Types', NAMES_BITFIELD, TYPES_BITFIELD, 'bitfield_list')
    doc.gen_index()
    logger.info("Generation Complete.")

class DocGenerator():
    """Methods for formatting and outputting the template strings with data from the XML"""

    def __init__(self, path, heading=True, metadata=True, minver=0):
        """Initialize generator"""
        self.doc_file = path + DOC_FILE
        self.main = tmpl.MAIN_H1 if heading else tmpl.MAIN_NO_H1
        self.attr_row = tmpl.ATTR if metadata else tmpl.ATTR_NO_META
        self.inherit = tmpl.INHERIT_ROW if metadata else tmpl.INHERIT_NO_META
        self.minver = minver
        self.blocks = dict(TYPES_BLOCK, **TYPES_COMPOUND)
        install_dir = os.path.abspath(path)
        if not os.path.exists(install_dir):
            os.makedirs(install_dir)
        # Install CSS and ICO
        if os.path.dirname(CSS_PATH) != install_dir:
            copy2(CSS_PATH, install_dir)
            copy2(ICO_PATH, install_dir)

    #
    # Template Helper functions
    #

    def list_attributes(self, compound):
        """Create Attribute List"""
        attrs = ''
        count = 0
        for mem in compound.members:
            if self.minver and mem.ver2 and mem.ver2 < self.minver:
                continue
            attr_type = tmpl.TYPE_LINK.format(clean(mem.type), mem.type)
            if mem.template:
                attr_type += tmpl.TMPL_LINK.format(clean(mem.template), mem.template)
            content = {
                'attr_name': mem.name,
                'attr_type': attr_type,
                'attr_arg': mem.arg,
                'attr_arr1': mem.arr1.lhs,
                'attr_arr2': mem.arr2.lhs,
                'attr_cond': mem.cond,
                'attr_desc': mem.description.replace('\n', '<br/>'),
                'attr_from': mem.orig_ver1,
                'attr_to': mem.orig_ver2,
                'row': 'even' if count % 2 == 0 else 'odd'
            }
            count += 1 # Manually increment because of 'continue' on skipped versioned rows
            attrs += self.attr_row.format(**content)
        return attrs

    @staticmethod
    def list_tags(names, types, template):
        """List each tag with a description"""
        tag_list = ''
        for count, tname in enumerate(names):
            tag = types[tname]
            content = {
                'list_name': tag.name,
                'list_cname': clean(tag.name),
                'list_desc': tag.description.replace('\n', '<br/>'),
                'row': 'even' if count % 2 == 0 else 'odd'
            }
            tag_list += template.format(**content)
        return tag_list

    @staticmethod
    def list_choices(tag):
        """Create Choice List"""
        choice_list = ''
        for count, opt in enumerate(tag.options):
            content = {
                # Display bitflags as hex
                'enum_number': opt.value if not hasattr(opt, 'bit') else '{0:#0{1}x}'.format(int(opt.value), 10),
                'enum_name': opt.name,
                'enum_desc': opt.description.replace('\n', '<br/>'),
                'row': 'even' if count % 2 == 0 else 'odd'
            }
            choice_list += tmpl.ENUM_ROW.format(**content)
        return choice_list

    @staticmethod
    def list_bitfield_members(tag):
        """Create bitfield members"""
        member_list = ''
        for count, member in enumerate(tag.bit_members):
            content = {
                'member_name' : member.name if member.name else '',
                'member_mask' : member.mask if member.mask else '',
                'member_type' : member.type if member.type else '',
                'member_default' :  member.default if member.default else '',
                'member_pos' : member.pos if member.pos else '',
                'member_width' : member.width if member.width else '',
                'member_desc': member.description.replace('\n', '<br/>'),
                'row': 'even' if count % 2 == 0 else 'odd'
            }
            member_list += tmpl.BITFIELD_ROW.format(**content)
        return member_list

    @staticmethod
    def list_child_blocks(block):
        """Create Child Block list"""
        return ''.join(tmpl.LI_LINK.format(clean(n), n) for n in NAMES_BLOCK if TYPES_BLOCK[n].inherit == block)

    def member_of(self, name):
        """Create Member Of list"""
        found = ''
        for b_name in NAMES_BLOCK + NAMES_COMPOUND:
            for bmem in self.blocks[b_name].members:
                if bmem.type == name:
                    found += tmpl.LI_LINK.format(clean(b_name), b_name)
                    break
        return found

    def list_ancestor_attributes(self, block):
        """Create list of attributes for all ancestors"""
        attr_list = ''
        for ancestor in reversed(block.ancestors()):
            content = {'inherit': ancestor.name, 'cinherit': clean(ancestor.name)}
            attr_list += self.inherit.format(**content) + self.list_attributes(ancestor)
        return attr_list

    def list_convertible(self, tag):
        """Create list of types this type can be converted to"""
        return ','.join([tmpl.TYPE_LINK.format(clean(n.name), n.name) for n in tag.convert])



    def list_object_tree(self, root):
        """Builds a hierarchical unordered list for a specified ancestor root."""
        tree = ''
        # Get truncated description
        lines = root.description.splitlines(False)
        # Add a new list for this ancestor
        tree += tmpl.LI_LINK_DESC.format(clean(root.name), root.name, '' if not lines else lines[0])
        # Create Child List
        children = [TYPES_BLOCK[n] for n in NAMES_BLOCK if TYPES_BLOCK[n].inherit == root]
        if children:
            tree += tmpl.UL_ITEM.format(''.join(self.list_object_tree(c) for c in children))
        return tree

    #
    # Generation Functions
    #

    def gen_pages(self, names, types, template):
        """Generate Pages for XML Tag"""
        for count, name in enumerate(names):
            tag = types[name]
            contents = {
                'name': tag.name,
                'description': tag.description.replace('\n', '<br/>'),
                'storage': '' if not isinstance(tag, Enum) else tag.storage,
                'count': bool(tag.count == 'true'),
                'member_of': self.member_of(name) if not isinstance(tag, Block) else '',
                'row': 'even' if count % 2 == 0 else 'odd',
                'integral' : bool(tag.integral == 'true'),
                'boolean' : bool(tag.boolean == 'true'),
                'size' : int(tag.size) if not tag.size == '' else None,
            }
            if isinstance(tag, Block):
                contents['attributes'] = self.list_ancestor_attributes(tag)
                contents['parent_of'] = self.list_child_blocks(tag)
            elif isinstance(tag, Compound):
                contents['attributes'] = self.list_attributes(tag)
                if tag.convert is not None:
                    contents['convertible'] = self.list_convertible(tag)
                else:
                    contents['convertible'] = 'None'
            elif isinstance(tag, Enum):
                contents['choices'] = self.list_choices(tag)
            elif isinstance(tag, Bitfield):
                contents['members'] = self.list_bitfield_members(tag)
            else:
                if tag.convert is not None:
                    contents['convertible'] = self.list_convertible(tag)
                else:
                    contents['convertible'] = 'None'
            

            page = {'title': tag.name, 'contents': template.format(**contents)}

            html = io.open(self.doc_file.format(clean(tag.name)), 'wt', 1, 'utf-8')
            html.write( self.main.format(**page) )
            html.close()

    def gen_list_page(self, title, names, types, pagename, rowtmpl=tmpl.LIST_ROW, header='Name'):
        """Generate List Page for XML Tag"""
        page = {'title': title}
        contents = {
            'title': title,
            'list_header': header,
            'list': self.list_tags(names, types, rowtmpl)
        }
        if isinstance(types[names[0]], Block):
            page['contents'] = tmpl.NAV_LIST.format(**contents)
        else:
            page['contents'] = tmpl.LIST.format(**contents)

        html = io.open(self.doc_file.format(pagename), 'wt', 1, 'utf-8')
        html.write( self.main.format(**page) )
        html.close()

    def gen_index(self):
        """Generate index.html"""
        page = {'title': 'NIF Object Hierarchy'}
        contents = {
            'title': page['title'],
            'object_tree': self.list_object_tree( TYPES_BLOCK['NiObject'] )
        }
        page['contents'] = tmpl.NAV_HIER.format(**contents)

        html = io.open(self.doc_file.format('index'), 'wt', 1, 'utf-8')
        html.write( self.main.format(**page) )
        html.close()

if __name__ == "__main__":
    main()
