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

#ifndef _OS_NET_SOCKSSOCKETPROXY_H
#define _OS_NET_SOCKSSOCKETPROXY_H

#include "buffer.h"
#include "isocketproxy.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class NetExport SocksSocketProxy : public ISocketProxy<boost::asio::ip::tcp>
{
public:
	typedef ISocketProxy<boost::asio::ip::tcp> ProxyBase;
	typedef ProxyBase::socket_type socket_type;
	typedef ProxyBase::endpoint_type endpoint_type;
	typedef ProxyBase::callback_type callback_type;

// Enumerated types
public:
	enum Version
	{
		v4,
		v4a,
		v5
	};

	enum Command
	{
		connect,
		bind,
		udp_associate
	};

// Construction
public:
	SocksSocketProxy(shared_ptr<socket_type> socket, Version version = v5);
	virtual ~SocksSocketProxy();

// Attributes
public:
	inline Version getProxyVersion() const;
	inline void setProxyVersion(Version version);

	inline Command getProxyCommand() const;
	inline void setProxyCommand(Command proxyCommand);

	inline const std::string & getProxyUsername() const;
	inline void setProxyUsername(const std::string &username);

	inline const std::string & getProxyPassword() const;
	inline void setProxyPassword(const std::string &password);

	inline const endpoint_type * getBindEndpoint() const;

private:
	void setBindAddress(const boost::asio::ip::address &address);
	void setBindPort(uint32 port);

// Operations
private:
	bool createHelloRequestV5();
	bool prepareHelloResponseV5();
	bool parseHelloResponseV5(uint8 &method);

	void createAuthenticationRequest();
	bool prepareAuthenticationResponse();
	bool parseAuthenticationResponse();

	bool createConnectionRequest();
	bool createConnectionRequestV4(const endpoint_type &endpoint);
	bool createConnectionRequestV5(const endpoint_type &endpoint);
	bool prepareConnectionResponse();
	bool parseConnectionResponse(boost::system::error_code &e, uint8 &addressType, uint32 &pending);
	bool parseConnectionResponseV4(boost::system::error_code &e, uint8 &addressType, uint32 &pending);
	bool parseConnectionResponseV5(boost::system::error_code &e, uint8 &addressType, uint32 &pending);
	bool parseConnectionResponseData(boost::system::error_code &e, uint8 addressType);
	bool parseConnectionResponseDataV4(boost::system::error_code &e, uint8 addressType);
	bool parseConnectionResponseDataV5(boost::system::error_code &e, uint8 addressType);

	bool socksConnect(boost::system::error_code &e);
	bool socksAuthenticate(boost::system::error_code &e);

	bool socksConnectV4(boost::system::error_code &e);
	bool socksConnectV5(boost::system::error_code &e);

	void handshakeCallback1(const boost::system::error_code &e, shared_ptr<callback_type> callback);
	void handshakeCallback2(const boost::system::error_code &e, shared_ptr<callback_type> callback);
	void handshakeCallback3(const boost::system::error_code &e, shared_ptr<callback_type> callback);
	void handshakeCallback4(const boost::system::error_code &e, shared_ptr<callback_type> callback);		
	void connectedCallback1(const boost::system::error_code &e, shared_ptr<callback_type> callback);
	void connectedCallback2(const boost::system::error_code &e, shared_ptr<callback_type> callback);
	void connectedCallback3(const boost::system::error_code &e, uint8 addressType, shared_ptr<callback_type> callback);

	void asyncSocksConnect(shared_ptr<callback_type> callback);
	void asyncSocksConnectV4(shared_ptr<callback_type> callback);
	void asyncSocksConnectV5(shared_ptr<callback_type> callback);

	void readPacket(boost::system::error_code &e);
	void writePacket(boost::system::error_code &e);

	void reset();

// ISocketProxy interface
protected:
	virtual void proxy_connect(boost::system::error_code &e) OS_OVERRIDE;
	virtual void proxy_async_connect(const boost::system::error_code &e, shared_ptr<callback_type> callback) OS_OVERRIDE;

protected:
	Version m_proxyVersion;
	Command m_proxyCommand;
	std::string m_proxyUsername;
	std::string m_proxyPassword;
	scoped_ptr<endpoint_type, os_delete_t> m_bindEndpoint;
	scoped_ptr<Buffer> m_buffer;

private:
	static const endpoint_type null_endpoint;
};

//////////////////////////////////////////////////////////////////////

inline SocksSocketProxy::Version SocksSocketProxy::getProxyVersion() const { return m_proxyVersion; }
inline void SocksSocketProxy::setProxyVersion(Version proxyVersion) { m_proxyVersion = proxyVersion; }

inline SocksSocketProxy::Command SocksSocketProxy::getProxyCommand() const { return m_proxyCommand; }
inline void SocksSocketProxy::setProxyCommand(Command proxyCommand) { m_proxyCommand = proxyCommand; }

inline const std::string & SocksSocketProxy::getProxyUsername() const { return m_proxyUsername; }
inline void SocksSocketProxy::setProxyUsername(const std::string &proxyUsername) { m_proxyUsername = proxyUsername; }

inline const std::string & SocksSocketProxy::getProxyPassword() const { return m_proxyPassword; }
inline void SocksSocketProxy::setProxyPassword(const std::string &proxyPassword) { m_proxyPassword = proxyPassword; }

inline const SocksSocketProxy::endpoint_type * SocksSocketProxy::getBindEndpoint() const { return m_bindEndpoint.get(); }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_NET_SOCKSSOCKETPROXY_H
