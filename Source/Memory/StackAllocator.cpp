#include "StackAllocator.h"

#include <new>
#include <memory>
#include <cstring>

namespace Engine
{
	namespace Memory
	{
		StackAllocator::StackAllocator() :
			Memory_(nullptr), CurrentSize_(0), MaxSize_(0)
		{};

		StackAllocator::~StackAllocator()
		{
			if (Memory_ != nullptr)
			{
				delete[] Memory_;
			}
		}

		bool StackAllocator::ReserveMemory(size_t Size)
		{
			if (Memory_ == nullptr && Size > 0)
			{
				MaxSize_ = Size;
				Memory_ = static_cast<void*>(new(std::nothrow) char[MaxSize_]);

				if (Memory_ == nullptr)
					return false;
			}

			return false;
		}

		bool StackAllocator::Resize(size_t Size)
		{
			void* NewMemory = static_cast<void*>(new(std::nothrow) char[Size]);

			if (NewMemory != nullptr)
			{
				size_t CopySize = (Size > MaxSize_) ? MaxSize_ : Size;
				memcpy(NewMemory, Memory_, CopySize);

				delete[] Memory_;
				Memory_ = NewMemory;

				return true;
			}

			return false;
		}

		void* StackAllocator::Allocate(size_t Size, size_t Alignment)
		{
			if (Memory_ != nullptr)
			{
				void* Start = static_cast<void*>(static_cast<char*>(Memory_) + CurrentSize_);
				size_t Space = MaxSize_ - CurrentSize_;

				void* Aligned = std::align(Alignment, Size, Start, Space);

				if (Aligned != nullptr)
				{
					size_t Offset = static_cast<char*>(Aligned) - static_cast<char*>(Start);
					CurrentSize_ += Offset + Size;

					return Aligned;
				}
			}

			return nullptr;
		}

		bool StackAllocator::Deallocate(size_t NewSize)
		{
			if (NewSize <= CurrentSize_)
			{
				CurrentSize_ = NewSize;

				return true;
			}

			return false;
		}

		size_t StackAllocator::GetCurrentSize() const
		{
			return CurrentSize_;
		}

		size_t StackAllocator::GetMaxSize() const
		{
			return MaxSize_;
		}
	}
}