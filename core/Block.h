#pragma once

namespace mj
{
class Block
{
public:
	enum class ESide
	{
		South,
		North,
		East,
		West,
		Top,
		Bottom,
	};

	enum class BlockType
	{
		BlockType_Default = 0,

		BlockType_Grass,
		BlockType_Dirt,
		BlockType_Water,
		BlockType_Stone,
		BlockType_Wood,
		BlockType_Sand,

		BlockType_NumTypes,
	};

	bool equals(const Block &other)
	{
		return this->m_enabled == other.m_enabled && this->type == other.type;
	}

	bool m_enabled = true;
	int32 type = 0;
	ESide side;
};
}
