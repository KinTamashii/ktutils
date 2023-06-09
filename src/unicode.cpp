#include <ktu/unicode.hpp>

template uint32_t ktu::u8::impl::read(const uint8_t*);
template uint32_t ktu::u8::impl::read(const uint8_t**);
template uint32_t ktu::u8::impl::rread(const uint8_t*);
template uint32_t ktu::u8::impl::rread(const uint8_t**);

uint32_t (*const ktu::u8::ptr::read)(const void*) = (uint32_t(*)(const void*))&ktu::u8::impl::read<const uint8_t*>; 
uint32_t (*const ktu::u8::ptr::read_ref)(const void**) = (uint32_t(*)(const void**))&ktu::u8::impl::read<const uint8_t**>; 
uint32_t (*const ktu::u8::ptr::rread)(const void*) = (uint32_t(*)(const void*))&ktu::u8::impl::rread<const uint8_t*>; 
uint32_t (*const ktu::u8::ptr::rread_ref)(const void**) = (uint32_t(*)(const void**))&ktu::u8::impl::rread<const uint8_t**>;


template size_t ktu::u8::impl::write<uint8_t>(uint32_t,uint8_t*);

size_t (*const ktu::u8::ptr::write)(uint32_t, void*) = (size_t(*)(uint32_t,void*))&ktu::u8::impl::write<uint8_t>;





template uint32_t ktu::u16::impl::read<true>(const uint16_t*);
template uint32_t ktu::u16::impl::read<true>(const uint16_t**);
template uint32_t ktu::u16::impl::read<false>(const uint16_t*);
template uint32_t ktu::u16::impl::read<false>(const uint16_t**);
template uint32_t ktu::u16::impl::rread<true>(const uint16_t*);
template uint32_t ktu::u16::impl::rread<true>(const uint16_t**);
template uint32_t ktu::u16::impl::rread<false>(const uint16_t*);
template uint32_t ktu::u16::impl::rread<false>(const uint16_t**);


uint32_t (*const ktu::u16::ptr::read_le)(const void*) = (uint32_t(*)(const void*))&ktu::u16::impl::read<false, const uint16_t*>;
uint32_t (*const ktu::u16::ptr::read_be)(const void*) = (uint32_t(*)(const void*))&ktu::u16::impl::read<true, const uint16_t*>;
uint32_t (*const ktu::u16::ptr::read_ref_le)(const void**) = (uint32_t(*)(const void**))&ktu::u16::impl::read<false, const uint16_t**>;
uint32_t (*const ktu::u16::ptr::read_ref_be)(const void**) = (uint32_t(*)(const void**))&ktu::u16::impl::read<true, const uint16_t**>;
uint32_t (*const ktu::u16::ptr::rread_le)(const void*) = (uint32_t(*)(const void*))&ktu::u16::impl::rread<false, const uint16_t*>;
uint32_t (*const ktu::u16::ptr::rread_be)(const void*) = (uint32_t(*)(const void*))&ktu::u16::impl::rread<true, const uint16_t*>;
uint32_t (*const ktu::u16::ptr::rread_ref_le)(const void**) = (uint32_t(*)(const void**))&ktu::u16::impl::rread<false, const uint16_t**>;
uint32_t (*const ktu::u16::ptr::rread_ref_be)(const void**) = (uint32_t(*)(const void**))&ktu::u16::impl::rread<true, const uint16_t**>;


template size_t ktu::u16::impl::write<true>(uint32_t,uint16_t*);
template size_t ktu::u16::impl::write<false>(uint32_t,uint16_t*);


size_t (*const ktu::u16::ptr::write_le)(uint32_t, void*) = (size_t(*)(uint32_t,void*))&ktu::u16::impl::write<false, uint16_t>;
size_t (*const ktu::u16::ptr::write_be)(uint32_t, void*) = (size_t(*)(uint32_t,void*))&ktu::u16::impl::write<true, uint16_t>;


uint32_t (*const ktu::u32::ptr::read_le)(const void*) = (uint32_t(*)(const void*))&ktu::u32::read<false, const uint32_t>;
uint32_t (*const ktu::u32::ptr::read_be)(const void*) = (uint32_t(*)(const void*))&ktu::u32::read<true, const uint32_t>;
uint32_t (*const ktu::u32::ptr::read_ref_le)(const void**) = (uint32_t(*)(const void**))&ktu::u32::read<false, const uint32_t*>;
uint32_t (*const ktu::u32::ptr::read_ref_be)(const void**) = (uint32_t(*)(const void**))&ktu::u32::read<true, const uint32_t*>;
uint32_t (*const ktu::u32::ptr::rread_le)(const void*) = (uint32_t(*)(const void*))&ktu::u32::rread<false, const uint32_t>;
uint32_t (*const ktu::u32::ptr::rread_be)(const void*) = (uint32_t(*)(const void*))&ktu::u32::rread<true, const uint32_t>;
uint32_t (*const ktu::u32::ptr::rread_ref_le)(const void**) = (uint32_t(*)(const void**))&ktu::u32::rread<false, const uint32_t*>;
uint32_t (*const ktu::u32::ptr::rread_ref_be)(const void**) = (uint32_t(*)(const void**))&ktu::u32::rread<true, const uint32_t*>;

size_t (*const ktu::u32::ptr::write_le)(uint32_t,void*) = (size_t(*)(uint32_t,void*))&ktu::u32::write<false, uint32_t, true>;
size_t (*const ktu::u32::ptr::write_be)(uint32_t,void*) = (size_t(*)(uint32_t,void*))&ktu::u32::write<true, uint32_t, true>;;
