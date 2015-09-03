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
		class Face
		{
		public:
			bool m_enabled = true;
			int32 type = 0;
			int32 side = 0;

			bool equals( const Face &other )
			{
				return this->m_enabled == other.m_enabled && this->type == other.type;
			}
		private:

		};
		Block();
		~Block();

		bool IsActive() const { return m_active; }
		void SetActive( bool active ) { this->m_active = active; }

	private:
		bool m_active = true;
		BlockType m_blockType;
	};
}
