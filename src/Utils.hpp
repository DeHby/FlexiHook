#ifndef __UTILS_HEADER__
#define __UTILS_HEADER__

template<typename dst_type, typename src_type>
dst_type pointer_cast(src_type src)
{
	return *static_cast<dst_type*>(static_cast<void*>(&src));
}

template<typename dst_type, typename src_type>
void pointer_set(src_type src, dst_type dst)
{
	*reinterpret_cast<void**>(src) = reinterpret_cast<void*>(dst);
}

#endif