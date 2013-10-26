// <osiris_sps_source_header>
// This file is part of Osiris Serverless Portal System.
// Copyright (C)2005-2012 Osiris Team (info@osiris-sps.org) / http://www.osiris-sps.org )
//
// Osiris Serverless Portal System is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Osiris Serverless Portal System is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Osiris Serverless Portal System.  If not, see <http://www.gnu.org/licenses/>.
// </osiris_sps_source_header>

#ifndef _OS_CORE_CHARSET_H
#define _OS_CORE_CHARSET_H

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const uchar chNull                  = 0x00;
const uchar chHTab                  = 0x09;
const uchar chLF                    = 0x0A;
const uchar chVTab                  = 0x0B;
const uchar chFF                    = 0x0C;
const uchar chCR                    = 0x0D;
const uchar chAmpersand             = 0x26;
const uchar chAsterisk              = 0x2A;
const uchar chAt                    = 0x40;
const uchar chBackSlash             = 0x5C;
const uchar chBang                  = 0x21;
const uchar chCaret                 = 0x5E;
const uchar chCloseAngle            = 0x3E;
const uchar chCloseCurly            = 0x7D;
const uchar chCloseParen            = 0x29;
const uchar chCloseSquare           = 0x5D;
const uchar chColon                 = 0x3A;
const uchar chComma                 = 0x2C;
const uchar chDash                  = 0x2D;
const uchar chDollarSign            = 0x24;
const uchar chDoubleQuote           = 0x22;
const uchar chEqual                 = 0x3D;
const uchar chForwardSlash          = 0x2F;
const uchar chGrave                 = 0x60;
const uchar chNEL                   = 0x85;
const uchar chOpenAngle             = 0x3C;
const uchar chOpenCurly             = 0x7B;
const uchar chOpenParen             = 0x28;
const uchar chOpenSquare            = 0x5B;
const uchar chPercent               = 0x25;
const uchar chPeriod                = 0x2E;
const uchar chPipe                  = 0x7C;
const uchar chPlus                  = 0x2B;
const uchar chPound                 = 0x23;
const uchar chQuestion              = 0x3F;
const uchar chSingleQuote           = 0x27;
const uchar chSpace                 = 0x20;
const uchar chSemiColon             = 0x3B;
const uchar chTilde                 = 0x7E;
const uchar chUnderscore            = 0x5F;

const uchar chSwappedUnicodeMarker  = uchar(0xFFFE);
const uchar chUnicodeMarker         = uchar(0xFEFF);

const uchar chDigit_0               = 0x30;
const uchar chDigit_1               = 0x31;
const uchar chDigit_2               = 0x32;
const uchar chDigit_3               = 0x33;
const uchar chDigit_4               = 0x34;
const uchar chDigit_5               = 0x35;
const uchar chDigit_6               = 0x36;
const uchar chDigit_7               = 0x37;
const uchar chDigit_8               = 0x38;
const uchar chDigit_9               = 0x39;

const uchar chLatin_A               = 0x41;
const uchar chLatin_B               = 0x42;
const uchar chLatin_C               = 0x43;
const uchar chLatin_D               = 0x44;
const uchar chLatin_E               = 0x45;
const uchar chLatin_F               = 0x46;
const uchar chLatin_G               = 0x47;
const uchar chLatin_H               = 0x48;
const uchar chLatin_I               = 0x49;
const uchar chLatin_J               = 0x4A;
const uchar chLatin_K               = 0x4B;
const uchar chLatin_L               = 0x4C;
const uchar chLatin_M               = 0x4D;
const uchar chLatin_N               = 0x4E;
const uchar chLatin_O               = 0x4F;
const uchar chLatin_P               = 0x50;
const uchar chLatin_Q               = 0x51;
const uchar chLatin_R               = 0x52;
const uchar chLatin_S               = 0x53;
const uchar chLatin_T               = 0x54;
const uchar chLatin_U               = 0x55;
const uchar chLatin_V               = 0x56;
const uchar chLatin_W               = 0x57;
const uchar chLatin_X               = 0x58;
const uchar chLatin_Y               = 0x59;
const uchar chLatin_Z               = 0x5A;

const uchar chLatin_a               = 0x61;
const uchar chLatin_b               = 0x62;
const uchar chLatin_c               = 0x63;
const uchar chLatin_d               = 0x64;
const uchar chLatin_e               = 0x65;
const uchar chLatin_f               = 0x66;
const uchar chLatin_g               = 0x67;
const uchar chLatin_h               = 0x68;
const uchar chLatin_i               = 0x69;
const uchar chLatin_j               = 0x6A;
const uchar chLatin_k               = 0x6B;
const uchar chLatin_l               = 0x6C;
const uchar chLatin_m               = 0x6D;
const uchar chLatin_n               = 0x6E;
const uchar chLatin_o               = 0x6F;
const uchar chLatin_p               = 0x70;
const uchar chLatin_q               = 0x71;
const uchar chLatin_r               = 0x72;
const uchar chLatin_s               = 0x73;
const uchar chLatin_t               = 0x74;
const uchar chLatin_u               = 0x75;
const uchar chLatin_v               = 0x76;
const uchar chLatin_w               = 0x77;
const uchar chLatin_x               = 0x78;
const uchar chLatin_y               = 0x79;
const uchar chLatin_z               = 0x7A;

const uchar chYenSign               = 0xA5;
const uchar chWonSign               = 0x20A9;

const uchar chLineSeparator         = 0x2028;
const uchar chParagraphSeparator    = 0x2029;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_CHARSET_H
