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

/*
 * DO NOT EDIT.  THIS FILE IS GENERATED FROM iosiris.idl
 */

#ifndef __gen_iosiris_h__
#define __gen_iosiris_h__


#ifndef __gen_nsISupports_h__
#include "nsISupports.h"
#endif

/* For IDL files that don't want to include root IDL files. */
#ifndef NS_NO_VTABLE
#define NS_NO_VTABLE
#endif

/* starting interface:    IOsiris */
#define IOSIRIS_IID_STR "bf09da54-a667-486b-be59-a972391358ea"

#define IOSIRIS_IID \
  {0xbf09da54, 0xa667, 0x486b, \
    { 0xbe, 0x59, 0xa9, 0x72, 0x39, 0x13, 0x58, 0xea }}

class NS_NO_VTABLE NS_SCRIPTABLE IOsiris : public nsISupports {
 public: 

  NS_DECLARE_STATIC_IID_ACCESSOR(IOSIRIS_IID)

  /* boolean init (in AString root); */
  NS_SCRIPTABLE NS_IMETHOD Init(const nsAString & root, PRBool *_retval NS_OUTPARAM) = 0;

  /* AString parse (in AString link); */
  NS_SCRIPTABLE NS_IMETHOD Parse(const nsAString & link, nsAString & _retval NS_OUTPARAM) = 0;

  /* readonly attribute boolean started; */
  NS_SCRIPTABLE NS_IMETHOD GetStarted(PRBool *aStarted) = 0;

  /* readonly attribute AString homepage; */
  NS_SCRIPTABLE NS_IMETHOD GetHomepage(nsAString & aHomepage) = 0;

};

  NS_DEFINE_STATIC_IID_ACCESSOR(IOsiris, IOSIRIS_IID)

/* Use this macro when declaring classes that implement this interface. */
#define NS_DECL_IOSIRIS \
  NS_SCRIPTABLE NS_IMETHOD Init(const nsAString & root, PRBool *_retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD Parse(const nsAString & link, nsAString & _retval NS_OUTPARAM); \
  NS_SCRIPTABLE NS_IMETHOD GetStarted(PRBool *aStarted); \
  NS_SCRIPTABLE NS_IMETHOD GetHomepage(nsAString & aHomepage); 

/* Use this macro to declare functions that forward the behavior of this interface to another object. */
#define NS_FORWARD_IOSIRIS(_to) \
  NS_SCRIPTABLE NS_IMETHOD Init(const nsAString & root, PRBool *_retval NS_OUTPARAM) { return _to Init(root, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD Parse(const nsAString & link, nsAString & _retval NS_OUTPARAM) { return _to Parse(link, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD GetStarted(PRBool *aStarted) { return _to GetStarted(aStarted); } \
  NS_SCRIPTABLE NS_IMETHOD GetHomepage(nsAString & aHomepage) { return _to GetHomepage(aHomepage); } 

/* Use this macro to declare functions that forward the behavior of this interface to another object in a safe way. */
#define NS_FORWARD_SAFE_IOSIRIS(_to) \
  NS_SCRIPTABLE NS_IMETHOD Init(const nsAString & root, PRBool *_retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->Init(root, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD Parse(const nsAString & link, nsAString & _retval NS_OUTPARAM) { return !_to ? NS_ERROR_NULL_POINTER : _to->Parse(link, _retval); } \
  NS_SCRIPTABLE NS_IMETHOD GetStarted(PRBool *aStarted) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetStarted(aStarted); } \
  NS_SCRIPTABLE NS_IMETHOD GetHomepage(nsAString & aHomepage) { return !_to ? NS_ERROR_NULL_POINTER : _to->GetHomepage(aHomepage); } 

#if 0
/* Use the code below as a template for the implementation class for this interface. */

/* Header file */
class _MYCLASS_ : public IOsiris
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_IOSIRIS

  _MYCLASS_();

private:
  ~_MYCLASS_();

protected:
  /* additional members */
};

/* Implementation file */
NS_IMPL_ISUPPORTS1(_MYCLASS_, IOsiris)

_MYCLASS_::_MYCLASS_()
{
  /* member initializers and constructor code */
}

_MYCLASS_::~_MYCLASS_()
{
  /* destructor code */
}

/* boolean init (in AString root); */
NS_IMETHODIMP _MYCLASS_::Init(const nsAString & root, PRBool *_retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString parse (in AString link); */
NS_IMETHODIMP _MYCLASS_::Parse(const nsAString & link, nsAString & _retval NS_OUTPARAM)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute boolean started; */
NS_IMETHODIMP _MYCLASS_::GetStarted(PRBool *aStarted)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* readonly attribute AString homepage; */
NS_IMETHODIMP _MYCLASS_::GetHomepage(nsAString & aHomepage)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* End of implementation class template. */
#endif


#endif /* __gen_iosiris_h__ */
