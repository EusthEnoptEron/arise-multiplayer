
//#include "../pch.h"
#include "SDK.h"

// Name: Arise, Version: 1.0.0

namespace SDK
{

	ProcessEventFn UObject::ProcessEventPtr = nullptr;
	FUObjectArray* UObject::GObjects = nullptr;
	TNameEntryArray* FName::GNames = nullptr;

	//---------------------------------------------------------------------------
	bool FWeakObjectPtr::IsValid() const
	{
		if (ObjectSerialNumber == 0)
		{
			return false;
		}
		if (ObjectIndex < 0)
		{
			return false;
		}
		auto ObjectItem = UObject::GetGlobalObjects().GetItemByIndex(ObjectIndex);
		if (!ObjectItem)
		{
			return false;
		}
		if (!SerialNumbersMatch(ObjectItem))
		{
			return false;
		}
		return !(ObjectItem->IsUnreachable() || ObjectItem->IsPendingKill());
	}
	//---------------------------------------------------------------------------
	UObject* FWeakObjectPtr::Get() const
	{
		if (IsValid())
		{
			auto ObjectItem = UObject::GetGlobalObjects().GetItemByIndex(ObjectIndex);
			if (ObjectItem)
			{
				return ObjectItem->Object;
			}
		}
		return nullptr;
	}
	//---------------------------------------------------------------------------
}
