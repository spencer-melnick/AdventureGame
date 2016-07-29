#pragma once

#include <new>

namespace Engine
{
	namespace Memory
	{
		template <typename T>
		class PackedObjectPool
		{
			struct PoolData_;
			struct PoolObjectID_;
			struct PoolObjectIndex_;

		public:
			class Iterator;

			class Handle
			{
				friend class PackedObjectPool<T>;

			public:
				Handle(PackedObjectPool<T>* Reference, PoolObjectID_ ID) :
					Reference_(Reference), ID_(ID)
				{}

				Handle(const Handle& Source) :
					Reference_(Source.Reference_), ID_(Source.ID_)
				{};

				Handle() :
					Reference_(nullptr)
				{};

				T* GetData() const
				{
					if (Reference_ != nullptr)
					{
						if (ID_.OuterIndex->Version == ID_.Version)
						{
							return &(Reference_->Data_[ID_.OuterIndex->InnerIndex].Data);
						}
					}

					return nullptr;
				}

				bool IsValid() const
				{
					return (GetData() == nullptr) ? false : true;
				}

			protected:
				PackedObjectPool<T>* Reference_;
				PoolObjectID_ ID_;
			};

			class Iterator
			{
			public:
				Iterator(PoolData_* Data, unsigned int Length, unsigned int Location = 0) :
					Data_(Data), Length_(Length),Location_(Location)
				{};

				Iterator(const Iterator& Source) :
					Data_(Source->Data_), Length_(Source->Length), Location_(Source->Location_)
				{};

				Iterator& operator++()
				{
					++Location_;
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
					if (Location_ > 0)
						--Location_;
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
				unsigned int EndValue_;
			};

			PackedObjectPool() = default;

			bool Allocate(unsigned int Elements)
			{
				if (Data_ == nullptr && Elements > 0)
				{
					MaxElements_ = Elements;
					Data_ = new(std::nothrow) PoolData_[Elements + 1];
					Indices_ = new(std::nothrow) PoolObjectIndex_[Elements];

					if (Data_ != nullptr && Indices_ != nullptr)
					{
						for (unsigned int i = 0; i < MaxElements_; i++)
						{
							Data_[i].OuterIndex = &(Indices_[i]);
							Indices_[i].InnerIndex = i;
							Indices_[i].Version = 0;
							Indices_[i].NextFreeIndex = &Indices_[i + 1];
						}

						Indices_[MaxElements_ - 1].NextFreeIndex = nullptr;
						FirstFreeIndex_ = &(Indices_[0]);

						return true;
					}
					else
					{
						if (Indices_ == nullptr && Data_ != nullptr)
							delete[] Data_;

						if (Data_ == nullptr && Indices_ != nullptr)
							delete[] Indices_;
					}
				}

				return false;
			}

			Handle Create()
			{
				if (FirstFreeIndex_ != nullptr && UsedElements_ < MaxElements_)
				{
					PoolData_& Object = Data_[UsedElements_];
					PoolObjectID_ ObjectID;
					ObjectID.OuterIndex = FirstFreeIndex_;
					Object.OuterIndex = FirstFreeIndex_;

					ObjectID.Version = ObjectID.OuterIndex->Version;
					ObjectID.OuterIndex->InnerIndex = UsedElements_;

					FirstFreeIndex_ = FirstFreeIndex_->NextFreeIndex;
					++UsedElements_;

					return Handle(this, ObjectID);
				}

				return Handle();
			}

			void Free(PoolObjectIndex_& OldIndex)
			{
				PoolData_& EndObject = Data_[UsedElements_ - 1];
				PoolObjectIndex_& EndIndex = *EndObject.OuterIndex;

				PoolData_& OldObject = Data_[OldIndex.InnerIndex];

				EndIndex.InnerIndex = OldIndex.InnerIndex;
				OldObject = EndObject;

				++OldIndex.Version;
				OldIndex.NextFreeIndex = FirstFreeIndex_;
				FirstFreeIndex_ = &OldIndex;

				--UsedElements_;
			}

			void Free(PoolObjectID_ ObjectID)
			{
				Free(*ObjectID.OuterIndex);
			}

			void Free(Handle Object)
			{
				Free(Object.ID_);
			}

			Iterator begin()
			{
				return Iterator(Data_, UsedElements_);
			}

			Iterator end()
			{
				return Iterator(Data_, UsedElements_, UsedElements_);
			}

		private:
			unsigned int MaxElements_ = 0;
			unsigned int UsedElements_ = 0;
			PoolObjectIndex_* FirstFreeIndex_ = 0;
			PoolObjectIndex_* Indices_ = nullptr;
			PoolData_* Data_ = nullptr;

			struct PoolData_
			{
				T Data;
				PoolObjectIndex_* OuterIndex;
			};

			struct PoolObjectIndex_
			{
				unsigned int InnerIndex;
				unsigned int Version;
				PoolObjectIndex_* NextFreeIndex;
			};

			struct PoolObjectID_
			{
				PoolObjectIndex_* OuterIndex;
				unsigned int Version;
			};
		};

		template <typename T>
		typename PackedObjectPool<T>::Iterator begin(PackedObjectPool<T>& Pool)
		{
			return Pool.begin();
		}

		template <typename T>
		typename PackedObjectPool<T>::Iterator end(PackedObjectPool<T>& Pool)
		{
			return Pool.end();
		}
	}
}