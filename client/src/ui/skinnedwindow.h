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

#ifndef _OS_SKIN_SKINNEDWINDOW_H
#define _OS_SKIN_SKINNEDWINDOW_H

#include "windowrenderer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T, typename R = WindowRenderer>
class SkinnedWindow : public T
{
	typedef T WindowBase;
	typedef R WindowRendererImpl;

// Construction
public:
	SkinnedWindow();
	template <typename P1>
	SkinnedWindow(P1 p1);
	template <typename P1, typename P2>
	SkinnedWindow(P1 p1, P2 p2);
	template <typename P1, typename P2, typename P3>
	SkinnedWindow(P1 p1, P2 p2, P3 p3);
	template <typename P1, typename P2, typename P3, typename P4>
	SkinnedWindow(P1 p1, P2 p2, P3 p3, P4 p4);
	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	SkinnedWindow(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5);
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	SkinnedWindow(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6);
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	SkinnedWindow(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7);
	template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	SkinnedWindow(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8);
	virtual ~SkinnedWindow();

private:
	void construct();

// Attributes
public:
	inline WindowRendererImpl * getRenderer() const;

protected:
	WindowRendererImpl *m_renderer;
};

//////////////////////////////////////////////////////////////////////

template <typename T, typename R>
SkinnedWindow<T, R>::SkinnedWindow()
{
	construct();
}

template <typename T, typename R>
template <typename P1>
SkinnedWindow<T, R>::SkinnedWindow(P1 p1) : WindowBase(p1)
{
	construct();
}

template <typename T, typename R>
template <typename P1, typename P2>
SkinnedWindow<T, R>::SkinnedWindow(P1 p1, P2 p2) : WindowBase(p1, p2)
{
	construct();
}

template <typename T, typename R>
template <typename P1, typename P2, typename P3>
SkinnedWindow<T, R>::SkinnedWindow(P1 p1, P2 p2, P3 p3) : WindowBase(p1, p2, p3)
{
	construct();
}

template <typename T, typename R>
template <typename P1, typename P2, typename P3, typename P4>
SkinnedWindow<T, R>::SkinnedWindow(P1 p1, P2 p2, P3 p3, P4 p4) : WindowBase(p1, p2, p3, p4)
{
	construct();
}

template <typename T, typename R>
template <typename P1, typename P2, typename P3, typename P4, typename P5>
SkinnedWindow<T, R>::SkinnedWindow(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) : WindowBase(p1, p2, p3, p4, p5)
{
	construct();
}

template <typename T, typename R>
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
SkinnedWindow<T, R>::SkinnedWindow(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) : WindowBase(p1, p2, p3, p4, p5, p6)
{
	construct();
}

template <typename T, typename R>
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
SkinnedWindow<T, R>::SkinnedWindow(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) : WindowBase(p1, p2, p3, p4, p5, p6, p7)
{
	construct();
}

template <typename T, typename R>
template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
SkinnedWindow<T, R>::SkinnedWindow(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) : WindowBase(p1, p2, p3, p4, p5, p6, p7, p8)
{
	construct();
}

template <typename T, typename R>
SkinnedWindow<T, R>::~SkinnedWindow()
{
	RemoveEventHandler(m_renderer);

	delete m_renderer;
	m_renderer = null;
}

template <typename T, typename R>
void SkinnedWindow<T, R>::construct()
{
	m_renderer = new WindowRendererImpl(this);

	PushEventHandler(m_renderer);
}

template <typename T, typename R>
inline typename SkinnedWindow<T, R>::WindowRendererImpl * SkinnedWindow<T, R>::getRenderer() const
{
	return m_renderer;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_SKIN_SKINNEDWINDOW_H
