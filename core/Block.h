#pragma once

namespace mj
{
	enum BlockType
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

	class Block
	{
	public:
		Block();
		~Block();

		bool IsActive() const { return m_active; }

	private:
		bool m_active = false;
		BlockType m_blockType;
	};
}
