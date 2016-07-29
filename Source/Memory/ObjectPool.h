#pragma once

#include <new>

namespace Engine
{
	namespace Memory
	{
		template <typename T>
		class ObjectPool
		{
			struct PoolData_;

		public:
			class Iterator;

			class Handle
			{
				friend class ObjectPool<T>;

			public:
				Handle(PoolData_* Reference) :
					Reference_(Reference), Version_(Reference_->Version)
				{}

				Handle(const Handle& Source) :
					Reference_(Source.Reference_), Version_(Source.Version_)
				{};

				Handle() :
					Reference_(nullptr), Version_(0)
				{};

				T* GetData() const
				{
					if (Reference_ != nullptr)
					{
						if (Reference_->Version == Version_)
						{
							return &(Reference_->Data);
						}
					}

					return nullptr;
				}

				bool IsValid() const
				{
					return (GetData() == nullptr) ? false : true;
				}

			protected:
				PoolData_* Reference_;
				unsigned int Version_;
			};

			class Iterator
			{
			public:
				Iterator(PoolData_* Data, unsigned int Length, unsigned int Location = 0) :
					Data_(Data), Length_(Length), Location_(Location)
				{};

				Iterator(const Iterator& Source) :
					Data_(Source->Data_), Length_(Source->Length), Location_(Source->Location_)
				{};

				Iterator& operator++()
				{
					while (Location_ < Length_)
					{
						++Location_;
						if (Location_ < Length_)
						{
							if (Data_[Location_].Used == true)
							{
								break;
							}
						}
					}
					return *this;
				}

				Iterator operator++(int)
				{
					Iterator Copy(*this);
					++(*this);
					return Copy;
				}

				Iterator& operator--()
				{
					while (Location_ > 0)
					{
						--Location;
						if (Data_[Location_].Used == true)
							break;
					}
				}

				Iterator operator--(int)
				{
					Iterator Copy(*this);
					--(*this);
					return Copy;
				}

				bool operator==(const Iterator& Other)
				{
					return (Data_ == Other.Data_ && Location_ == Other.Location_);
				}

				bool operator!=(const Iterator& Other)
				{
					return !(*this == Other);
				}

				T& operator*()
				{
					return Data_[Location_].Data;
				}

			protected:
				PoolData_* Data_;
				unsigned int Length_;
				unsigned int Location_;
			};

			ObjectPool() = default;

			bool Allocate(unsigned int Elements)
			{
				if (Data_ == nullptr && Elements > 0)
				{
					Elements_ = Elements;
					Data_ = new(std::nothrow) PoolData_[Elements];

					if (Data_ != nullptr)
					{
						FirstFree_ = Data_;

						for (unsigned int i = 0; i < Elements_; i++)
						{
							Data_[i].NextFree = &(Data_[i + 1]);
							Data_[i].Version = 0;
							Data_[i].Used = false;
						}

						Data_[Elements_ - 1].NextFree = nullptr;

						return true;
					}
				}

				return false;
			}

			Handle Create()
			{
				if (FirstFree_ != nullptr)
				{
					PoolData_* Current = FirstFree_;
					FirstFree_ = Current->NextFree;

					unsigned int Index = Data_ - Current;
					FurthestElement_ = (Index > FurthestElement_) ? Index : FurthestElement_;

					Current->Used = true;
					return Handle(Current);
				}

				return Handle();
			}

			void Free(Handle Object)
			{
				PoolData_* Current = Object.Reference_;

				if (Current != nullptr)
				{
					if (static_cast<unsigned int>(Current - Data_) < Elements_ && Current->Version == Handle.Version_)
					{
						++(Current->Version);
						Current->Used = false;
						Current->NextFree = FirstFree_;
						FirstFree_ = Current;
					}
				}
			}

			Iterator begin()
			{
				return Iterator(Data_, Elements_);
			}

			Iterator end()
			{
				return Iterator(Data_, Elements_, Elements_);
			}

		private:
			unsigned int Elements_ = 0;
			PoolData_* Data_ = nullptr;
			PoolData_* FirstFree_ = nullptr;

			struct PoolData_
			{
				T Data;
				bool Used;
				unsigned int Version;
				PoolData_* NextFree;
			};
		};

		template <typename T>
		typename ObjectPool<T>::Iterator begin(ObjectPool<T>& Pool)
		{
			return Pool.begin();
		}

		template <typename T>
		typename ObjectPool<T>::Iterator end(ObjectPool<T>& Pool)
		{
			return Pool.end();
		}
	}
}