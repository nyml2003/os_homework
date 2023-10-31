#pragma once
class Block {
public:
	int getSize() {
		return size;
	}
	int getAddress() const {
		return address;
	}
	int getEndAddress() const {
		return address + size;
	}
	Block& setSize(int size) {
		this->size = size;
		return *this;
	}
	Block& setAddress(int address) {
		this->address = address;
		return *this;
	}
	bool operator == (const Block& block) {
		return this->size == block.size && this->address == block.address;
	}
private:
	int size;
	int address;
};