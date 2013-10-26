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

#ifndef _OS_P2P_PACKETS_HELLO_H
#define _OS_P2P_PACKETS_HELLO_H

#include "p2ppacketsipacket.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace p2p {
namespace packets {

//////////////////////////////////////////////////////////////////////

class EngineExport Hello : public IPacket
{
	typedef IPacket PacketBase;

// Construction
public:
	Hello();
	virtual ~Hello();

// Attributes
public:
	Buffer getMachineID() const;
	void setMachineID(const Buffer &id);

	Buffer getSignature() const;
	void setSignature(const Buffer &signature);

	uint32 getServerPort() const;
	void setServerPort(uint32 port);

// IPacket interface
public:
	virtual bool create();

protected:
	virtual bool parse();

private:
	static const String ID;
	static const String SIGNATURE;
	static const String PORT;
};

//////////////////////////////////////////////////////////////////////

} // packets
} // p2p
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_P2P_PACKETS_HELLO_H
