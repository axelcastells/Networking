#pragma once

#define NEW_UID IDGenerator::Instance().NewId()

class IDGenerator {
public:
	static IDGenerator & Instance() {
		static IDGenerator instance;
		return instance;
	}
	unsigned int NewId() { return _id++; }
private:
	IDGenerator() : _id(1) {}

	unsigned int _id;
};