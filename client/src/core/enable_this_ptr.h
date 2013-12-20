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

#ifndef _OS_CORE_ENABLE_THIS_PTR_H
#define _OS_CORE_ENABLE_THIS_PTR_H

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T> 
class enable_this_ptr
{
// Internal
private:
	class enable_this_ptr_deleter
	{
	// Construction
	public:
		enable_this_ptr_deleter()
		{

		}

		template <typename D> 
		void set_deleter(const shared_ptr<D> &deleter)
		{
			m_deleter = deleter;
		}

		template <typename D> 
		void operator()(D *)
		{
			BOOST_ASSERT(m_deleter.use_count() <= 1);
			m_deleter.reset();
		}

	private:
		shared_ptr<void> m_deleter;
	};

// Construction
protected:
    enable_this_ptr()
    {

    }

    enable_this_ptr(const enable_this_ptr &)
    {

    }

    enable_this_ptr & operator =(const enable_this_ptr &)
    {
        return *this;
    }

    ~enable_this_ptr()
    {
        OS_ASSERT(m_internal_shared_this.use_count() <= 1);	// Assicura che non ci siano altri shared_ptr che si riferiscono all'oggetto a parte l'eventuale m_internal_shared_this (vedi internal_accept_shared_owner)
    }

public:
    shared_ptr<T> get_this_ptr()
    {
        init_internal_weak_this();
		OS_ASSERT(m_internal_weak_this.expired() == false);
        return shared_ptr<T>(m_internal_weak_this);
    }

    shared_ptr<T const> get_this_ptr() const
    {
        init_internal_weak_this();
		OS_ASSERT(m_internal_weak_this.expired() == false);
        return shared_ptr<T>(m_internal_weak_this);
    }

	template <typename I>
	inline shared_ptr<I> get_this_ptr()
	{
		OS_ASSERT(boost::dynamic_pointer_cast<I>(get_this_ptr()) != nullptr);
		return boost::dynamic_pointer_cast<I>(get_this_ptr());
	}

	template <typename I>
	inline shared_ptr<I const> get_this_ptr() const
	{
		OS_ASSERT(boost::dynamic_pointer_cast<I const>(get_this_ptr()) != nullptr);
		return boost::dynamic_pointer_cast<I const>(get_this_ptr());
	}

private:
    void init_internal_weak_this() const
    {
		if(m_internal_weak_this.expired())
        {
			m_internal_shared_this.reset(static_cast<T *>(const_cast<enable_this_ptr *>(this)), enable_this_ptr_deleter());
            m_internal_weak_this = m_internal_shared_this;
        }
    }

public:
    template <typename X, typename Y> 
	void internal_accept_shared_owner(shared_ptr<X> *ppx, Y *py) const
    {
        OS_ASSERT(ppx != nullptr);

        if(m_internal_weak_this.use_count() == 0)
        {
            m_internal_weak_this = shared_ptr<T>(*ppx, py);
        }
        else if(m_internal_shared_this.use_count() != 0)
        {
            OS_ASSERT(ppx->unique());

			enable_this_ptr_deleter *pd = boost::get_deleter<enable_this_ptr_deleter>(m_internal_shared_this);
            OS_ASSERT(pd != 0);

            pd->set_deleter(*ppx);

            ppx->reset(m_internal_shared_this, ppx->get());
            m_internal_shared_this.reset();
        }
    }

private:
    mutable weak_ptr<T> m_internal_weak_this;
    mutable shared_ptr<T> m_internal_shared_this;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_ENABLE_THIS_PTR_H
