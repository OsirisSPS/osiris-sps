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

#include "stdafx.h"
#include "streamfilter.h"

#include "istreamlayer.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
class pimpl<StreamFilter>
{
	typedef list<shared_ptr<IStreamLayer> >::type Layers;

// Internal
private:
	class StreamLayerNone : public IStreamLayer
	{
		typedef IStreamLayer LayerBase;

	// Construction
	public:
		StreamLayerNone(shared_ptr<IStream> stream);
		virtual ~StreamLayerNone();

	// IStreamLayer interface
	public:
		virtual uint32 read(void *v, uint32 size) const;
		virtual uint32 write(const void *v, uint32 size);

	// IStreamLayer overrides
	public:
		virtual bool seek(uint64 offset, SeekPosition from);
		virtual uint64 position() const;
		virtual uint64 size() const;
		virtual bool close();
		virtual bool flush();

	private:
		shared_ptr<IStream> m_stream;
	};

// Construction
public:
	pimpl(shared_ptr<IStream> stream);
	virtual ~pimpl();

// Operations
public:
	void addLayer(shared_ptr<IStreamLayer> layer);

	bool is_open() const;
	bool close();
	uint32 peek(void *v, uint32 size) const;
	uint32 read(void *v, uint32 size) const;
	uint32 write(const void *v, uint32 size);
	bool seek(uint64 offset, SeekPosition from) const;
	uint64 position() const;
	uint64 size() const;
	bool flush();
	bool eof() const;

private:
	shared_ptr<IStream> m_stream;
	Layers m_layers;
};

//////////////////////////////////////////////////////////////////////

pimpl<StreamFilter>::StreamLayerNone::StreamLayerNone(shared_ptr<IStream> stream) : m_stream(stream)
{
	OS_ASSERT(stream != null);
}

pimpl<StreamFilter>::StreamLayerNone::~StreamLayerNone()
{
	// N.B.: vedi pimpl<StreamFilter>::~pimpl()
}

uint32 pimpl<StreamFilter>::StreamLayerNone::read(void *v, uint32 size) const
{
	if(m_next != null)
		return m_next->read(v, size);

	return m_stream->read(v, size);
}

uint32 pimpl<StreamFilter>::StreamLayerNone::write(const void *v, uint32 size)
{
	if(m_next != null)
		return m_next->write(v, size);

	return m_stream->write(v, size);
}

bool pimpl<StreamFilter>::StreamLayerNone::seek(uint64 offset, SeekPosition from)
{
	return m_stream->seek(offset, from);
}

uint64 pimpl<StreamFilter>::StreamLayerNone::position() const
{
	return m_stream->position();
}

uint64 pimpl<StreamFilter>::StreamLayerNone::size() const
{
	return m_stream->size();
}

bool pimpl<StreamFilter>::StreamLayerNone::close()
{
	if(LayerBase::close() == false)
		return false;
		
	return m_stream->close();		// Se chiamata esplicitamente, chiude lo stream
}

bool pimpl<StreamFilter>::StreamLayerNone::flush()
{
	if(LayerBase::flush() == false)
		return false;

	return m_stream->flush();
}

//////////////////////////////////////////////////////////////////////

pimpl<StreamFilter>::pimpl(shared_ptr<IStream> stream) : m_stream(stream)
{
	OS_ASSERT(stream != null);
	OS_ASSERT(stream->is_open());
	addLayer(shared_ptr<StreamLayerNone>(OS_NEW StreamLayerNone(stream)));
}

pimpl<StreamFilter>::~pimpl()
{
	// N.B.: qui non bisogna richiamare la close altrimenti si chiuderebbe anche lo stream principale (potrebbe essere referenziato all'esterno e filtrato
	// temporaneamente, quindi qui non va chiuso): se lo stream non è più referenziato, verrà chiuso probabilmente in automatico nel suo distruttore		

	utils::flush_front(m_layers);
}

void pimpl<StreamFilter>::addLayer(shared_ptr<IStreamLayer> layer)
{
	if(layer == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	shared_ptr<IStreamLayer> next;
	if(m_layers.empty() == false)
		next = m_layers.front();

	// N.B.: se si modifica l'ordine di accodamento dei layers rivedere il distruttore
	m_layers.push_front(layer);
	layer->setNext(next);
}

bool pimpl<StreamFilter>::is_open() const
{
	return m_stream->is_open();
}

bool pimpl<StreamFilter>::close()
{
	OS_ASSERT(m_layers.empty() == false);

	return m_layers.front()->close();
}

uint32 pimpl<StreamFilter>::read(void *v, uint32 size) const
{
	OS_ASSERT(m_layers.empty() == false);
	return m_layers.front()->read(v, size);
}

uint32 pimpl<StreamFilter>::write(const void *v, uint32 size)
{
	OS_ASSERT(m_layers.empty() == false);
	return m_layers.front()->write(v, size);
}

bool pimpl<StreamFilter>::seek(uint64 offset, SeekPosition from) const
{
	return m_stream->seek(offset, from);
}

uint64 pimpl<StreamFilter>::position() const
{
	return m_stream->position();
}

uint64 pimpl<StreamFilter>::size() const
{
	return m_stream->size();
}

bool pimpl<StreamFilter>::flush()
{
	OS_ASSERT(m_layers.empty() == false);

	return m_layers.front()->flush();
}

bool pimpl<StreamFilter>::eof() const
{
	return m_stream->eof();
}

//////////////////////////////////////////////////////////////////////
	
StreamFilter::StreamFilter(shared_ptr<IStream> stream) : m_impl(stream)
{
	
}

StreamFilter::~StreamFilter()
{
	// N.B.: vedi pimpl<StreamFilter>::~pimpl()
}

void StreamFilter::addLayer(shared_ptr<IStreamLayer> layer)
{
	m_impl->addLayer(layer);
}

bool StreamFilter::is_open() const
{
	return m_impl->is_open();
}

bool StreamFilter::close()
{
	return m_impl->close();
}

uint32 StreamFilter::read(void *v, uint32 size) const
{
	return m_impl->read(v, size);
}

uint32 StreamFilter::write(const void *v, uint32 size)
{
	return m_impl->write(v, size);
}

bool StreamFilter::seek(uint64 offset, SeekPosition from) const
{
	return m_impl->seek(offset, from);
}

uint64 StreamFilter::position() const
{
	return m_impl->position();
}

uint64 StreamFilter::size() const
{
	return m_impl->size();
}

bool StreamFilter::flush()
{
	return m_impl->flush();
}

bool StreamFilter::eof() const
{
	return m_impl->eof();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
