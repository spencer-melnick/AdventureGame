#pragma once

namespace Engine
{
	namespace Memory
	{
		class StackAllocator
		{
		public:
			StackAllocator();
			~StackAllocator();

			// Reserves memory for future allocations -
			// returns false on failure
			bool ReserveMemory(size_t Size);

			// Allocates a new buffer and copies all crruent data -
			// returns false on failure and keeps existing buffer
			bool Resize(size_t Size);

			// Allocates memory on stack given size and alignment -
			// returns nullptr on failure
			void* Allocate(size_t Size, size_t Alignment);
			
			// Resets the buffer size to NewSize -
			// returns false if NewSize exceeds CurrentSize
			bool Deallocate(size_t NewSize);

			size_t GetCurrentSize() const;
			size_t GetMaxSize() const;

		private:
			void* Memory_;
			size_t CurrentSize_;
			size_t MaxSize_;
		};
	}
}