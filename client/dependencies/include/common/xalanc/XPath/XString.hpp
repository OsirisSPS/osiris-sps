/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if !defined(XSTRING_HEADER_GUARD_1357924680)
#define XSTRING_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file.
#include <xalanc/XPath/XStringBase.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XString : public XStringBase
{
public:

    typedef XStringBase     ParentType;

    /**
     * Construct an XString object from a string.
     * 
     * @param val The string value to use.
     * @param theMemoryManager The MemoryManager instance.
     */
    XString(
            const XalanDOMString&   val,
            MemoryManager&          theManager);

    /**
     * Construct an XString object from a string.
     * 
     * @param val The string value to use.
     * @param theMemoryManager The MemoryManager instance.
     */
    XString(
            const XalanDOMChar*     val,
            MemoryManager&          theManager);

    /**
     * Construct an XString object from a string.
     * 
     * @param val The string value to use.
     * @param len The length of the string value.
     * @param theMemoryManager The MemoryManager instance.
     */
    XString(
            const XalanDOMChar*     val,
            XalanSize_t             len,
            MemoryManager&          theManager);

    /**
     * Construct an XString object from another string.
     * 
     * @param source The source XString.
     * @param theMemoryManager The MemoryManager instance.
     */
    XString(
            const XString&      source,
            MemoryManager&      theManager);

    virtual
    ~XString();

    void
    set(const XalanDOMString&   theString)
    {
        m_value = theString;
    }

    // These methods are inherited from XObject ...

    virtual const XalanDOMString&
    str(XPathExecutionContext&  executionContext) const;

    virtual const XalanDOMString&
    str() const;

    virtual void
    str(
            XPathExecutionContext&  executionContext,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const;

    virtual void
    str(
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const;

    virtual void
    str(
            XPathExecutionContext&  executionContext,
            XalanDOMString&         theBuffer) const;

    virtual void
    str(XalanDOMString&     theBuffer) const;

    virtual double
    stringLength(XPathExecutionContext&     executionContext) const;

private:

    //not implemented
    XString(const XString&  source);

    XalanDOMString  m_value;
};



XALAN_CPP_NAMESPACE_END



#endif  // XSTRING_HEADER_GUARD_1357924680
