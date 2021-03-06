#include <cstddef>
#include <cstdint>
#include <libriscv/machine.hpp>

extern "C"
void LLVMFuzzerTestOneInput(const uint8_t* data, size_t len)
{
	static std::vector<uint8_t> bin {};
	static riscv::Machine<riscv::RISCV32> machine32 { bin };
	static riscv::Machine<riscv::RISCV64> machine64 { bin };

	if (UNLIKELY(len == 0)) return;

	// Copy fuzzer data to 0x1000 and skip the zero-page.
	// Non-zero length guarantees that the page will be created.
	machine32.copy_to_guest(0x1000, data, len);
	machine32.cpu.jump(0x1000);
	try {
		// Let's avoid loops
		machine32.simulate(5000);
	} catch (std::exception& e) {
		//printf(">>> Exception: %s\n", e.what());
	}
	// Again for 64-bit
	machine64.copy_to_guest(0x1000, data, len);
	machine64.cpu.jump(0x1000);
	try {
		machine64.simulate(5000);
	} catch (std::exception& e) {
		//printf(">>> Exception: %s\n", e.what());
	}
}
