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

#ifndef _OS_NET_LIMITEDCONNECTION_H
#define _OS_NET_LIMITEDCONNECTION_H

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Bandwidth;

//////////////////////////////////////////////////////////////////////

class NetExport ILimitedConnection
{
// Construction
public:
	ILimitedConnection();
	virtual ~ILimitedConnection();

// Attributes
public:
	inline shared_ptr<Bandwidth> getDownload() const;
	inline shared_ptr<Bandwidth> getUpload() const;

// Operations
protected:
	uint32 limitDownload(uint32 bytes);
	uint32 limitUpload(uint32 bytes);

	void handleDownload(uint32 bytes);
	void handleUpload(uint32 bytes);

protected:
	shared_ptr<Bandwidth> m_download;
	shared_ptr<Bandwidth> m_upload;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<Bandwidth> ILimitedConnection::getDownload() const { return m_download; }
inline shared_ptr<Bandwidth> ILimitedConnection::getUpload() const { return m_upload; }

//////////////////////////////////////////////////////////////////////

template <typename T>
class LimitedConnection : public T,
						  public ILimitedConnection
{
	typedef T SocketBase;

// Construction
public:
	LimitedConnection();
	template <typename P>
	LimitedConnection(P p);
	virtual ~LimitedConnection();

// Socket overrides
public:
	virtual int32 read(void *v, uint32 bytes);
	virtual int32 write(const void *v, uint32 bytes);

protected:
	virtual void onDownload(uint32 bytes);
	virtual void onUpload(uint32 bytes);
};

//////////////////////////////////////////////////////////////////////

template <typename T>
LimitedConnection<T>::LimitedConnection()
{

}

template <typename T>
template <typename P>
LimitedConnection<T>::LimitedConnection(P p) : SocketBase(p)
{

}

template <typename T>
LimitedConnection<T>::~LimitedConnection()
{

}

template <typename T>
int32 LimitedConnection<T>::read(void *v, uint32 bytes)
{
	return SocketBase::read(v, limitDownload(bytes));
}

template <typename T>
int32 LimitedConnection<T>::write(const void *v, uint32 bytes)
{
	return SocketBase::write(v, limitUpload(bytes));
}

template <typename T>
void LimitedConnection<T>::onDownload(uint32 bytes)
{
	SocketBase::onDownload(bytes);

	handleDownload(bytes);
}

template <typename T>
void LimitedConnection<T>::onUpload(uint32 bytes)
{
	SocketBase::onUpload(bytes);

	handleUpload(bytes);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_LIMITEDCONNECTION_H
