#ifndef PATTERN16_PATTERN16_H
#define PATTERN16_PATTERN16_H
#include <string>

#include "scanners/base.h"

namespace Pattern16 {
	static void* find(void* regionStart, size_t regionSize, std::string signature, const Impl::Frequencies16& frequencies = Impl::loadFrequencyCache()) {
		auto signature_u8 = Impl::processSignatureString(signature);
		return (void*)Impl::scan(regionStart, regionSize, signature_u8, frequencies);
	}

	static const void* find(const void* regionStart, size_t regionSize, std::string signature, const Impl::Frequencies16& frequencies = Impl::loadFrequencyCache()) {
		auto signature_u8 = Impl::processSignatureString(signature);
		return Impl::scan(regionStart, regionSize, signature_u8, frequencies);
	}
}

#endif // PATTERN16_PATTERN16_H